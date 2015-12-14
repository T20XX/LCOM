#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <stdint.h>
#include "sprite.h"

#define MIN_CHAR 33
#define MAX_CHAR 126

char * getPhysicalAdress();
unsigned getH_res();
unsigned getV_res();
uint16_t * getBuffer();

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
int vg_pixel(unsigned short x, unsigned short y, uint16_t  color);


int vg_sprite(Sprite* sprite, uint16_t transparent_color);

int vg_map(uint16_t * map, unsigned int x, unsigned int y, unsigned int width, unsigned int height);

int vg_map_transparent(uint16_t * map, unsigned int x, unsigned int y, unsigned int width, unsigned int height, uint16_t color);

int vg_string(char * string,unsigned int x, unsigned int y,  uint16_t color);

void vg_buffer();


#endif /* __VIDEO_GR_H */
