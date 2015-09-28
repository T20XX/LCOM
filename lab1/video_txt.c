#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {

	int i;
	char *ptr;
	ptr = video_mem;
	for(i = 0; i< scr_width*scr_lines; i++, ptr++)
	{
		*ptr = ch;
		ptr++;
		*ptr =attr;
	}
  
}

void vt_blank() {
	int i;
	char *ptr;
	ptr = video_mem;
	for(i = 0; i< scr_width*scr_lines; i++, ptr++)
	{
		*ptr=0x00;
		ptr++;
		*ptr=0x00;
	}

}

int vt_print_char(char ch, char attr, int r, int c) {
	char *ptr;
	ptr = video_mem + (c + r * scr_width)*2;
	*ptr = ch;
	ptr++;
	*ptr =attr;
}

int vt_print_string(char *str, char attr, int r, int c) {
	char *ptr;
	ptr = video_mem + (c + r * scr_width)*2;
	while (*str != 0)
	{
		*ptr = *str;
		ptr++;
		*ptr = attr;
		ptr++;
		str++;
	}
}

int vt_print_int(int num, char attr, int r, int c) {
	char *ptr;
	ptr = video_mem + (c + r * scr_width)*2;
	while (num > 0)
	{
		*ptr = num%10 + 0x30;
		ptr++;
		*ptr = attr;
		ptr++;
		num = num/10;
	}
}


int vt_draw_frame(int width, int height, char attr, int r, int c) {
	int i;
	char *ptr;
	ptr = video_mem + (c + r * scr_width)*2;
	//Barra superior
	*ptr = UL_CORNER;
	ptr++;
	*ptr = attr;
	ptr++;
	for(i = 0; i< width-2; i++, ptr++)
		{
			*ptr = HOR_BAR;
			ptr++;
			*ptr = attr;
		}
	*ptr = UR_CORNER;
	ptr++;
	*ptr = attr;
	ptr++;
	//Barras laterais
	ptr += (scr_width - width + c)*2;
	*ptr=VERT_BAR;
	ptr++;
	*ptr = attr;
	ptr++;

	//Barra inferior
	*ptr = UL_CORNER;
	ptr++;
	*ptr = attr;
	ptr++;
	for(i = 0; i< width-2; i++, ptr++)
		{
			*ptr = HOR_BAR;
			ptr++;
			*ptr = attr;
		}
	*ptr = UR_CORNER;
	ptr++;
	*ptr = attr;
	ptr++;
}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
