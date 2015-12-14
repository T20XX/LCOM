#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "lmlib.h"
//#include "sprite.h"
#include <stdio.h>
//#include <stdint.h>
#include <string.h>
#include "video_gr.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  16


/* Private global variables */

static uint16_t *video_mem;		/* Process address to which VRAM is mapped */
static char *physical_adress;
static uint16_t *buffer = NULL;

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

char * getPhysicalAdress(){return physical_adress;}
unsigned getH_res(){return h_res;}
unsigned getV_res(){return v_res;}
uint16_t * getBuffer(){return buffer;}

void change_variables(vbe_mode_info_t *info){
	int r;
	struct mem_range mr;


	h_res = info->XResolution;
	v_res = info->YResolution;
	bits_per_pixel = info->BitsPerPixel;

	physical_adress = (char *)info->PhysBasePtr;

	/* Allow memory mapping */

	mr.mr_base = info->PhysBasePtr;
	mr.mr_limit = mr.mr_base + h_res*v_res* bits_per_pixel;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, h_res*v_res* bits_per_pixel);

	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");
	else
		buffer = malloc(h_res * v_res * bits_per_pixel / 8);
}

void *vg_init(unsigned short mode){
	struct reg86u r;
	r.u.w.ax = SET_VBE_MODE; // VBE call, function 02 -- set VBE mode
	r.u.w.bx =  mode | LINEAR_FRAMEBUFFER_BIT; // set bit 14: linear framebuffer
	r.u.b.intno = VIDEO_CARD;
	if( sys_int86(&r) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		//return 1;
	}
	else

	{

		mmap_t map;

		lm_init();
		lm_alloc(sizeof(vbe_mode_info_t), &map);

		vbe_get_mode_info(mode, (vbe_mode_info_t *)map.phys);

		vbe_mode_info_t *vbe_info = map.virtual;


		change_variables(vbe_info);

		lm_free(&map);

		return video_mem;
	}
}

int vg_exit() {
	free(buffer);
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00;    /* Set Video Mode function */
	reg86.u.b.al = 0x03;    /* 80x25 text mode*/

	if( sys_int86(&reg86) != OK ) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

int vg_pixel(unsigned short x, unsigned short y, uint16_t color){
	/*uint16_t * ptr;
	ptr= (buffer + x + y*h_res);//*bits_per_pixel/8;
	 *ptr = (uint16_t)color;*/
	*(buffer + (x + y*h_res)) = color;
}

int vg_sprite(Sprite * sprite, uint16_t transparent_color){
	uint16_t * map = sprite->map;

	unsigned int i,j;
	for(i = sprite->y; i< sprite->y + sprite->height;i++){
		for(j = sprite->x; j < sprite->x + sprite->width;j++){
			if ((*map) != transparent_color){
				vg_pixel(j,i,*map);
			}
			map++;
		}
	}
	return 0;
}

int vg_map(uint16_t * map, unsigned int x, unsigned int y, unsigned int width, unsigned int height){
	uint16_t * bufferStartPos;
	uint16_t * imgStartPos;

	int i;
	for (i = 0; i < height; i++) {
		int pos = y + height - 1 - i;

		if (pos < 0 || pos >= v_res)
			continue;

		bufferStartPos = buffer;
		bufferStartPos += x + pos * h_res;

		imgStartPos = map + (height - 1 - i)* width;

		memcpy(bufferStartPos, imgStartPos, width*2);
	}
	return 0;
}

int vg_map_transparent(uint16_t * map, unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint16_t color){
	uint16_t * ptr = map;

	unsigned int i,j;
	for(i = y; i< y + height;i++){
		for(j = x; j < x + width;j++){
			if ((*ptr) != color){
				vg_pixel(j,i,*ptr);
			}
			ptr++;
		}
	}
	return 0;
}

int vg_string(char * string,unsigned int x, unsigned int y, unsigned int spacing,  uint16_t color){
	uint16_t * map;
	unsigned int width, height;
	map = map_Bitmap("/home/lcom/proj/code/img/font.bmp", &width, &height);
	uint16_t * ptr = map;
	char counter;
	unsigned int letter_width;
	unsigned int i, j,tempx = x,tempy = y;
	while(*string != NULL){
		if(*string >= MIN_CHAR && *string <= MAX_CHAR){
			counter = MIN_CHAR - 2;
			while(counter != *string){
				if(*ptr == 0)
					counter++;
				ptr++;
			}
			letter_width = 1;
			while(*ptr != 0xFFFF){
				letter_width++;
				ptr++;
			}
			letter_width++;
			for(i = 0; i < height-1; i++){
				ptr += width - letter_width-1;
				tempy++;
				tempx=x;
				for(j = 0; j < letter_width;j++)
				{
					if ((*ptr) != 0x07E0){
						vg_pixel(tempx,tempy,color);
					}
					ptr++;
					tempx++;
				}
			}
			tempy =y;
			x = tempx;
		} else if (*string == ' '){
			x += 20;
		}
		string++;
		ptr = map;
		x += spacing;
	}
	return 0;
}

void vg_buffer()
{
	memcpy(video_mem, buffer, h_res * v_res * bits_per_pixel/ 8);
}
