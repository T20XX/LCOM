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
#include "sprite.h"
#include "lmlib.h"

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

void wait_for_esc(){
	int irq_set = kbd_subscribe_int();
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

	float deltax, deltay;
	float decl;
	float b;
	float tempy, tempx;
	unsigned int i;

	if(xi < 0 || xi > 1023 ||
			xf < 0 || xf > 1023 ||
			yi < 0 || yi > 767 ||
			yf < 0 || yf > 767 ||
			color > 256)
		return -1;

	unsigned short x1 = xi;
	unsigned short x2 = xf;
	unsigned short y1 = yi;
	unsigned short y2 = yf;

	deltax = x2 - x1;
	deltay = y2 - y1;

	if(deltax < 0){
		x1 = xf;
		x2 = xi;
		deltax = x2 - x1;
	}

	if(deltay < 0){
		y1 = yf;
		y2 = yi;
		deltay = y2 - y1;
	}

	decl = (float)(deltay/deltax);
	b = y1-x1*decl;
	vg_init(0x105);


	if(deltax >= deltay){
		for(i = x1; i <= x2; i++){
			tempy = i*decl + b;
			vg_pixel(i, (int)tempy, color);
		}
	}
	else{
		for(i  = y1; i <= y2; i++){
			tempx = (i-b);
			tempx /= decl;
			vg_pixel((int)tempx, i, color);
		}
	}

	wait_for_esc();

	vg_exit();
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {



	unsigned int xpmw , xpmh; //pixmap dimensions

	char * pixmap = read_xpm(xpm, &xpmw, &xpmh);

	if(xi < 0 || xi+xpmw > 1023 ||
			yi < 0 || yi+xpmh > 767)
		return -1;

	vg_init(0x105);

	unsigned int i,j;
	for(i = yi; i< yi + xpmh;i++){
		for(j = xi; j < xi + xpmw;j++){
			vg_pixel(j,i,*pixmap);
			pixmap++;
		}
	}

	wait_for_esc();
	vg_exit();
	return 0;
}	


void moveSprite(Sprite * s){
	unsigned int i,j;

	for(i = s->y; i< s->y + s->height;i++){
		for(j = s->x; j < s->x + s->width;j++){
			vg_pixel(j,i,0);
		}
	}
	s->x += s->xspeed;
	s->y += s->yspeed;

	char *pixmap = s->map;
	for(i = s->y; i< s->y + s->height;i++){
		for(j = s->x; j < s->x + s->width;j++){
			vg_pixel(j,i,*pixmap);
			pixmap++;
		}
	}
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
		unsigned short hor, short delta, unsigned short time) {



	Sprite sprite;
	sprite.map = read_xpm(xpm, &sprite.width,&sprite.height);
	sprite.x = xi;
	sprite.y = yi;
	if (hor){
		sprite.xspeed = delta;
		sprite.yspeed = 0;
	}
	else{
		sprite.xspeed = 0;
		sprite.yspeed = delta;
	}

	if(xi < 0 || sprite.x + sprite.width  > 1023 ||
			yi < 0 || sprite.y + sprite.height > 767)
		return -1;
	vg_init(0x105);
	int timer_irq_set = timer_subscribe_int();
	int kbd_irq_set = kbd_subscribe_int();
	int ipc_status;
	message msg;
	int r;
	unsigned int code,counter = 0;

	timer_test_square(60);

	while( code != BREAKCODE  ) { /* You may want to use a different condition */
		/* Get a request message. */
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & kbd_irq_set) {
					code = kbd_output();
				}
				if (msg.NOTIFY_ARG & timer_irq_set && counter < time* 60) {
					counter++;
					if((sprite.x + sprite.width + sprite.xspeed) <= 1023 &&  (sprite.y + sprite.height + sprite.yspeed) <= 767)
						moveSprite(&sprite);

				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	timer_unsubscribe_int();
	kbd_unsubscribe_int();

	vg_exit();

}					

int test_controller() {
	mmap_t map;

	char * base;
	base = lm_init();
	lm_alloc(sizeof(vbe_controller_info_t), &map);

	vbe_get_controller_info((vbe_controller_info_t *)map.phys);

	vbe_controller_info_t *controller_info = map.virtual;

	if (*controller_info->Capabilities & DAC){
		printf("DAC width is switchable to 8 bits per primary color.\n");
	}
	else{
		printf("DAC is fixed width, with 6 bits per primary color.\n");
	}

	if ((*controller_info->Capabilities & VGA) >> 1){
		printf("Controller is not VGA compatible.\n");
	}
	else{
		printf("Controller is VGA compatible.\n");
	}

	if ((*controller_info->Capabilities & RAMDAC) >> 2){
		printf("When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h.\n");
	}
	else{
		printf("Normal RAMDAC operation.\n");
	}

	printf("List of modes:\n");


	/*printf("Video Modes PTR: 0x%X\n",controller_info->VideoModePtr);
	printf("Video Modes SEGMENT: 0x%X\n",PB2SEGMENT(controller_info->VideoModePtr));
	printf("Video Modes OFFSET: 0x%X\n",PB2OFF(controller_info->VideoModePtr));
	printf("Base: 0x%X\n",base);*/

	uint16_t * ptr =  (uint16_t  *)(base + PB2SEGMENT(controller_info->VideoModePtr));
	do{
		if ((*ptr) != 0)
		{
			printf("0x%X, ",(*ptr));
		}
		ptr++;
	}while((*ptr)!=0xFFFF);
	printf ("\n");

	printf("VRAM Total Memory: %d kb (%d MB)\n", controller_info->TotalMemory * 64,  (controller_info->TotalMemory * 64) / 1024);

	lm_free(&map);

	return 0;
}					

