#include <stdio.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "vbe.h"
#include "test5.h"
#include "timer.h"
#include "kbd.h"
#include "video_gr.h"
#include "read_xpm.h"
#include "pixmap.h"
#include "i8254.h"
#include "i8042.h"

void *test_init(unsigned short mode, unsigned short delay) {

	char *vram_adress=vg_init(mode);

	//variavel que guardara a informação toda relativa ao modo vbe
	vbe_mode_info_t vbe_info;
	vbe_get_mode_info(mode, &vbe_info);

	int  timer_irq_set = timer_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	timer_test_square(60); //força o timer a trabalhar a 60HZ
	int counter = 0; //Inicialização do contador

	while( counter < (delay * 60)) {
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & timer_irq_set) { /* subscribed interrupt */
					counter++;
				}
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	timer_unsubscribe_int();

	vg_exit();

	printf("VRAM ADRESS: 0x%02x \n", vram_adress);
	printf("PHYSICAL ADRESS: 0x%02x \n", getPhysicalAdress());
	return 0;

}

void wait_for_esc(){int irq_set = kbd_subscribe_int();
int ipc_status;
message msg;
int r;
unsigned int code;

while( code != BREAKCODE ) { /* You may want to use a different condition */
	/* Get a request message. */
	if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		printf("driver_receive failed with: %d", r);
		continue;
	}
	if (is_ipc_notify(ipc_status)) { /* received notification */
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE: /* hardware interrupt notification */
			if (msg.NOTIFY_ARG & irq_set) {
				code = kbd_output();
			}
			break;
		default:
			break; /* no other notifications expected: do nothing */
		}
	} else { /* received a standard message, not a notification */
		/* no standard messages expected: do nothing */
	}
}
kbd_unsubscribe_int();
}

int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {
	vg_init(0x105);

	unsigned int i,j;
	for(i = y; i< y+size;i++){
		for(j = x; j < x+size;j++){
			vg_pixel(j,i,color);
		}
	}

	wait_for_esc();

	vg_exit();
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, 
		unsigned short xf, unsigned short yf, unsigned long color) {

	/*short deltax, deltay;
	short decl;
	printf ("%g \n", decl);
	short b = yi-xi*decl;
	short tempy, tempx;
	unsigned int i;
	deltax = xf - xi;
	deltay = yf - yi;
	vg_init(0x105);


	if(deltax >= deltay){
		for(i = xi; i < xf; i++){
			tempy = i*decl + b;
			printf ("%g \n", tempy);
			vg_pixel(i, (int)tempy, color);
		}
	}
	else{
		for(i  = yi; i < yf; i++){
			tempx = (i-b)/decl;
			printf("%g \n", tempx);
			vg_pixel(i, (int)tempx, color);
		}
	}*/
	//metodo de bresenham
	int slope;
	int dx, dy, incE, incNE, d, x, y;
	int x1, x2, y1, y2;
	x1 = xi;
	x2 = xf;
	y1 = yi;
	y2 = yf;
	if(xi < 0 || xi > 1024)
		return;
	if(xf < 0 || xf > 1024)
		return;
	if(yf < 0 || yf > 768)
		return;
	if(yi < 0 || yi > 768)
		return;
	if(color > 	256)
		return;
	// Onde inverte a linha xi > xf
	if (xi > xf){
		x1 = xf;
		x2 = xi;
		y1 = yf;
		y2 = yi;
	}
	dx = x2 - x1;
	dy = y2 - y1;

	if (dy < 0){
		slope = -1;
		dy = -dy;
	}
	else{
		slope = 1;
	}
	vg_init(0x105);
	// Constante de Bresenham
	incE = 2 * dy;
	incNE = 2 * dy - 2 * dx;
	d = 2 * dy - dx;
	y = y1;
	for (x = x1; x <= x2; x++){
		vg_pixel(x, y, color);
		if (d <= 0){
			d += incE;
		}
		else{
			d += incNE;
			y += slope;
		}
	}

	//wait_for_esc();

	//vg_exit();
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	/*xpm = pic1;

	char * pixmap=read_xpm(xpm, 32, 13);

	unsigned int i,j;
	for(i = 0; i< 13;i++){
		for(j = 0; j < 32;j++){
			vg_pixel(j,i,*pixmap);
			pixmap++;
		}
	}*/
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
		unsigned short hor, short delta, unsigned short time) {

	/* To be completed */

}					

int test_controller() {

	/* To be completed */

}					

