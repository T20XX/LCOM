#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"
#include "lmlib.h"
#include "sprite.h"

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
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */
static char *physical_adress;

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

char * getPhysicalAdress(){return physical_adress;};

void change_variables(vbe_mode_info_t *info){
	int r;
	struct mem_range mr;


	h_res = info->XResolution;
	v_res = info->YResolution;
	bits_per_pixel = info->BitsPerPixel;

	physical_adress = (char *)info->PhysBasePtr;

	/* Allow memory mapping */

	mr.mr_base = info->PhysBasePtr;
	mr.mr_limit = mr.mr_base + h_res*v_res* bits_per_pixel/8;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, h_res*v_res* bits_per_pixel/8);

	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");
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

int vg_pixel(unsigned short x, unsigned short y, unsigned long color){
	char * ptr;
	ptr= (video_mem + x + y*h_res);//*bits_per_pixel/8;
	*ptr = color;
}

int vg_sprite(Sprite * sprite){
	unsigned int i,j;
		for(i = sprite->y; i< sprite->y + sprite->height;i++){
			for(j = sprite->x; j < sprite->x + sprite->width;j++){
				vg_pixel(j,i,*(sprite->map));
				sprite->map++;
			}
		}
		return 0;
}
