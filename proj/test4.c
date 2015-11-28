#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>

#include "i8042.h"
#include "mouse.h"
#include "timer.h"

static long int packet[3]; //Packet with 3 byte from mouse
static int counter = 0;
static unsigned int packet_counter = 0;

static long int config[3];
static unsigned int config_counter = 0;

static short temp_length = 0;
static short temp_tolerance = 0;
static int rb = 0;

typedef enum {INIT, DRAW, COMP} state_t;
typedef enum {RDOWN, RUP, MOVE, VERT_LINE, HOR_TOLERANCE} ev_type_t;

int mouse_int_handler(){
	packet[packet_counter]=mouse_output();
	if (packet_counter == 0)
		//Verifica se é o primeiro packet através da verificação do bit 3 (grande probabilidade)
		if ((packet[packet_counter] & ISFIRSTPACKET) == 0)
			return;

	//incrementa as variáveis referentes ao contador de packets e ao geral
	packet_counter ++;
	counter++;

	if(packet_counter == 3)
	{
		//Reset da variável contador dos packets
		packet_counter = 0;
		//Impressão da informação dos packets
		print_packet(packet);
	}
	return 0;
}

int test_packet(unsigned short cnt){
	int irq_set = mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	write_to_mouse();
	enable_packets();
	while(counter < (cnt*3)) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_int_handler();  /* process it */
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	mouse_unsubscribe_int();
	return 0;
}

int test_async(unsigned short idle_time) {
	int  timer_irq_set = timer_subscribe_int();
	int  mouse_irq_set = mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	timer_test_square(60); //força o timer a trabalhar a 60HZ
	int counter = 0; //Inicialização do contador
	write_to_mouse(); //escrever byte no mouse
	enable_packets(); //permitir a receção de packets

	while( counter < (idle_time * 60)) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouse_irq_set) { /* subscribed interrupt */
					counter = 0; //Reset da variável counter caso receba interrupt do mouse
					mouse_int_handler(); /* process it */
					break;
				}
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
					if (counter >= (idle_time * 60))
						printf("O tempo de espera terminou.");
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	timer_unsubscribe_int();
	mouse_unsubscribe_int();
	return 0;
}

int mouse_int_config_handler(){
	//similar à função mouse_int_handler() só que para os bytes de config

	config[config_counter]=mouse_output();

	config_counter ++;

	if(config_counter == 3)
	{
		//função que imprime num formato amigável para o utilizador a configuração do rato
		print_config(config);
	}
	return 0;
}

int test_config(void) {
	int irq_set = mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	write_to_mouse(); //escrever byte no mouse
	status_request(); //permite a receção do status do mouse (bytes de config)
	while(config_counter < 3) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_int_config_handler();  /* process it */
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	mouse_unsubscribe_int();
	return 0;
}


void check_hor_line(ev_type_t *evt) {
	static state_t st = INIT; // initial state; keep state
	switch (st) {
	case INIT:
		temp_tolerance= 0;
		temp_length = 0;
		if( evt == RDOWN )
			st = DRAW;
		break;
	case DRAW:
		if( evt == MOVE ) {
			if( evt == VERT_LINE )
				st = COMP;
			else if ( evt == HOR_TOLERANCE )
				st = INIT;
		} else if( evt == RUP )
			st = INIT;
		break;
	default:
		break;
	}
}


void gesture_aux(){
	long int temp;

	temp = packet[0] & RB;
	temp >>= 1;
	rb = temp;

	temp = packet[1] - 256 *((packet[0] & XSIGN) >> 4);
	temp_tolerance +=  temp;

	temp = packet[2] - 256 * ((packet[0] & YSIGN) >> 5);
	temp_length += temp;
}
void draw_line(unsigned int tolerance){
	//funcao auxiliar que desenha a variação da tolerancia
	if (temp_tolerance < -(tolerance) || temp_tolerance>tolerance){
		printf("|");
		int i;
		for(i = 0;i <tolerance*2; i++){
			if(i==tolerance)
				printf("|");
			else if(temp_tolerance != 0 && i == tolerance+temp_tolerance)
				printf("|");
			else printf(" ");
		}
		printf("| %d\n", temp_length);
	}

}
int mouse_int_gest_handler(){
	packet[packet_counter]=mouse_output();
	if (packet_counter == 0)
		if ((packet[packet_counter] & ISFIRSTPACKET) == 0)
			return;

	packet_counter ++;

	if(packet_counter == 3)
	{
		packet_counter = 0;
		//print_packet(packet);
		gesture_aux();
	}
	return 0;
}

int test_gesture(short length, unsigned short tolerance) {
	ev_type_t evt=INIT;
	int irq_set = mouse_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	write_to_mouse();
	enable_packets();
	while(temp_length > -length && temp_length < length) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) {
					mouse_int_gest_handler();  /* process it */
					if (packet_counter == 0){
						if (rb == 0)
						{
							//se o botão direito não estiver a ser premido a tolerancia e length temporárias são resetadas
							printf("Prima o botao direito do rato.\n");
							evt=RUP;
							check_hor_line(&evt);
						}
						else if (temp_tolerance + tolerance < 0 || temp_tolerance - tolerance > 0)
						{
							//se a tolerancia temporancia ultrapassar a tolerancia passada pelo argumento, a tolerancia e length temporárias são resetadas
							printf("A sua linha foi resetada pois excedeu a tolerancia horizontal.\n");
							evt=HOR_TOLERANCE;
							check_hor_line(&evt);
						}
						draw_line(tolerance);
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	printf("Conseguiu desenhar a linha vertical na perfeicao.");
	mouse_unsubscribe_int();
	return 0;
}
