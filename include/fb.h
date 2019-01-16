#ifndef __FB_H__
#define __FB_H__

#include <config.h>

// 宏定义
#define FBDEVICE	"/dev/fb0"

#define WIDTH		1024	
#define HEIGHT		600

#define WHITE		0xffffffff			// test ok
#define BLACK		0x00000000
#define RED			0xffff0000
#define GREEN		0xff00ff00			// test ok
#define BLUE		0xff0000ff			

#define GREENP		0x0000ff00			// 一样，说明前2个ff透明位不起作用


// 函数声明
int fb_open(void);
int fb_close(void);
void fb_draw_back(unsigned int width, unsigned int height, unsigned int color);
void fb_draw_picture(const unsigned char *Ppic);
void fb_draw_picture_test(pic_info *ppic);
void fb_draw_jpeg_test(pic_info *ppic);
void fb_draw_png(pic_info *ppic);





/*
void fb_draw_jpgpicture(void);
void fb_draw_smallpicture(void);
void fb_draw_picture_randomposition(unsigned int x0, unsigned int y0);
void fb_draw_picture_beyond_bounds(unsigned int x0, unsigned int y0);
*/





#endif






