#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	phys_bytes buf = (phys_bytes)vmi_p;

	struct reg86u r;
	 r.u.w.ax = GET_VBE_INFO; /* VBE get mode info */
	/* translate the buffer linear address to a far pointer */
	r.u.w.es = PB2BASE(buf); /* set a segment base */
	r.u.w.di = PB2OFF(buf); /* set the offset accordingly */
	r.u.w.cx = mode;
	r.u.b.intno = VIDEO_CARD;


	if( sys_int86(&r) != OK ) { /* call BIOS */
		//lm_free(&map);
		return 1;
	}
	else
	{
		//*vmi_p = map.virtual;

		return 0;
	}


}


