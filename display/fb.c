/*
 * file name:fb.c
 * function:open framebuffer,and use ioctl to get some information of decives
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <fb_bmp.h>
#include <fb.h>
#include <config.h>



//#include "1024600.h"
//#include "19201080.h"
//#include "500281.h"


// 全局变量
unsigned int *pfb = NULL;  // pointer to freambuffer
int fbfd = -1;

int fb_open(void)
{
	int ret = -1;
	
	
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	
	// 第1步：打开设备
	fbfd = open(FBDEVICE, O_RDWR);
	if (fbfd < 0)
	{
		perror("open error");
		return -1;
	}
	debug("open %s success.\n", FBDEVICE);
	
	// 第2步：获取设备的硬件信息
	ret = ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);
	if (ret < 0)
	{
		perror("ioctl error");
		return -1;
	}
	debug("smem_start = 0x%lx, smem_len = %u.\n", finfo.smem_start, finfo.smem_len);
	
	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
	if (ret < 0)
	{
		perror("ioctl error");
		return -1;
	}
	debug("xres = %u, yres = %u.\n", vinfo.xres, vinfo.yres);
	debug("xres_virtual = %u, yres_virtual = %u.\n", vinfo.xres_virtual, vinfo.yres_virtual);
	debug("bpp = %u.\n", vinfo.bits_per_pixel);
	
	// 第3步：进行mmap
	unsigned long len = vinfo.xres_virtual * vinfo.yres_virtual * vinfo.bits_per_pixel / 8;
	debug("len = %ld\n", len);
	pfb = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (NULL == pfb)
	{
		perror("mmap");
		return -1;
	}
	debug("pfb = %p.\n", pfb);
	
	return 0;
}

int fb_close(void)
{
	close(fbfd);
	
	return 0;
}



void fb_draw_back(unsigned int width, unsigned int height, unsigned int color)
{
	unsigned int x, y;
	
	for (y=0; y<height; y++)
	{
		for (x=0; x<width; x++)
		{
			*(pfb + y * WIDTH + x) = color;
		}
	}
}


/*
 *function:show bmp the picture
 *         the screen:1020*600 and the picture:1020*600
 */
void fb_draw_picture(const unsigned char *Ppic)
{
    unsigned int *p = pfb;
	const unsigned char *pdate = Ppic;
	unsigned int x, y;
    unsigned int cnt_fb;
	unsigned int cnt_pic=HEIGHT * WIDTH *3;
	
	for (y=0; y<HEIGHT; y++)
	{
        for (x=0; x<WIDTH; x++)
        {
			cnt_fb = WIDTH* y + x;

			*(p+cnt_fb) = ((pdate[cnt_pic-0]<<0) |(pdate[cnt_pic-1]<<8) | (pdate[cnt_pic-2]<<16));
			cnt_pic -= 3;
			
		}
	}
}


/*
 *function:
 *         
 */
void fb_draw_picture_test(pic_info *ppic)
{
    unsigned int *p = pfb;
	const char *pdate = ppic->pic_date;
	unsigned int x, y;
    unsigned int cnt_fb;
	//unsigned int cnt_pic= ppic->width * ppic->height * 3 -3;
	unsigned int cnt_pic= ppic->width * ppic->height * 3 -1;//这里的-1是因为
															//从0开始计算,所以最后会多一位
	
	for (y=0; y < ppic->height; y++)
	{
        for (x=0; x < ppic->width; x++)
        {
			cnt_fb = WIDTH* y + x;  //注意这里是width!!!!是屏幕的宽，在这里犯错误了

			*(p+cnt_fb) = ((pdate[cnt_pic-2]<<0) |(pdate[cnt_pic-1]<<8) | (pdate[cnt_pic-0]<<16));
//			*(p+cnt_fb) = ((pdate[cnt_pic+0]<<0) |(pdate[cnt_pic+1]<<8) | (pdate[cnt_pic+2]<<16));

			cnt_pic -= 3;
			
		}
	}
}
/*
 *function:
 *         
 */
void fb_draw_jpeg_test(pic_info *ppic)
{
    unsigned int *p = pfb;
	const char *pdate = ppic->pic_date;
	unsigned int x, y;
    unsigned int cnt_fb ;
	unsigned int cnt_pic = 0;
	//unsigned int cnt_pic= ppic->width * ppic->height * 3 -1;//这里的-1是因为
															//从0开始计算,所以最后会多一位
	
	for (y=0; y < ppic->height; y++)
	{
        for (x=0; x < ppic->width; x++)
        {
			cnt_fb = WIDTH* y + x;  //注意这里是width!!!!是屏幕的宽，在这里犯错误了

//			*(p+cnt_fb) = ((pdate[cnt_pic-2]<<0) |(pdate[cnt_pic-1]<<8) | (pdate[cnt_pic-0]<<16));
			*(p+cnt_fb) = ((pdate[cnt_pic+2]<<0) |(pdate[cnt_pic+1]<<8) | (pdate[cnt_pic+0]<<16));

			//cnt_pic -= 3;
			cnt_pic += 3;
		}
	}
}


void fb_draw_png(pic_info *ppic)
{
    unsigned int *p = pfb;
	const char *pdate = ppic->pic_date;
	unsigned int x, y;
    unsigned int cnt_fb ;
	unsigned int cnt_pic = 0;
	//unsigned int cnt_pic= ppic->width * ppic->height * 3 -1;//这里的-1是因为
															//从0开始计算,所以最后会多一位
	
	for (y=0; y < ppic->height; y++)
	{
        for (x=0; x < ppic->width; x++)
        {
			cnt_fb = WIDTH* y + x;  //注意这里是width!!!!是屏幕的宽，在这里犯错误了

//			*(p+cnt_fb) = ((pdate[cnt_pic-2]<<0) |(pdate[cnt_pic-1]<<8) | (pdate[cnt_pic-0]<<16));
			*(p+cnt_fb) = ((pdate[cnt_pic+2]<<0) |(pdate[cnt_pic+1]<<8) | (pdate[cnt_pic+0]<<16));

			//cnt_pic -= 3;
			cnt_pic += 3;
		}
	}
}


#if 0
/*
 *function:show the picture which is created by Img2Lcd.
 *        
 */
void fb_draw_jpgpicture(void)
{
    unsigned int *p = pfb;
	const unsigned char *pdate = gImage_1024600;
	//const unsigned char *pdate = gImage_19201080;
	unsigned int i, j;
    unsigned int cnt;
	
	for (i=0; i<HEIGHT; i++)
	{
        for (j=0; j<WIDTH; j++)
        {
			cnt = WIDTH* i + j;
			cnt *= 3;

			*p = ((pdate[cnt+0]<<0) |(pdate[cnt+1]<<8) | (pdate[cnt+2]<<16));
			p++;
		}
	}
}

/*
 *function:show the picture which is smaller than screen.
 *         the screen:1020*600 and the picture:500*281
 */
void fb_draw_smallpicture(void)
{
    unsigned int *p = pfb;
	const unsigned char *pdate = gImage_500281;
	unsigned int x, y;
    unsigned int cnt_fb, cnt_pic=0;
	
	for (y=0; y<281; y++)
	{
        for (x=0; x<500; x++)
        {
			cnt_fb = WIDTH* y + x;

			*(p+cnt_fb) = ((pdate[cnt_pic+0]<<0) |(pdate[cnt_pic+1]<<8) | (pdate[cnt_pic+2]<<16));
			cnt_pic += 3;
			
		}
	}
}

/*
 *function:show the small picture in the random position.
 *         the screen:1020*600 and the small picture:500*281
 *parameter:(x0,y0),starting coordinates of the picture
 */
void fb_draw_picture_randomposition(unsigned int x0, unsigned int y0)
{
    unsigned int *p = pfb;
	const unsigned char *pdate = gImage_500281;
	unsigned int x, y;
    unsigned int cnt_fb, cnt_pic=0;
	
	for (y=0; y<281; y++)
	{
        for (x=0; x<500; x++)
        {
			cnt_fb = WIDTH* (y+y0) + (x+x0);

			*(p+cnt_fb) = ((pdate[cnt_pic+0]<<0) |(pdate[cnt_pic+1]<<8) | (pdate[cnt_pic+2]<<16));
			cnt_pic += 3;
		}

	}
}

/*
 *function:when showing the picture if it beyond the screen's bounds,
 *			skip the pixels that beyond the bounds.
 *         the screen:1020*600 and the small picture:500*281
 *parameter:(x0,y0),starting coordinates of the picture
 */
void fb_draw_picture_beyond_bounds(unsigned int x0, unsigned int y0)
{
    unsigned int *p = pfb;
	const unsigned char *pdate = gImage_500281;
	unsigned int x, y;
    unsigned int cnt_fb, cnt_pic=0;
	
	for (y=0; y<281; y++)
	{

		if((y+y0) >= HEIGHT)
			break;				//刚开始自己写的continue，但是这里直接break就可以了
								//，因为已经到边界外了，剩下的直接退出就可以了		
        for (x=0; x<500; x++)
        {
			if((x+x0)>=WIDTH)
			{
				cnt_pic += 3; 		//skip the pixels that beyond the bounds
				continue;
			}
	
			cnt_fb = WIDTH* (y+y0) + (x+x0);

			*(p+cnt_fb) = ((pdate[cnt_pic+0]<<0) |(pdate[cnt_pic+1]<<8) | (pdate[cnt_pic+2]<<16));
			cnt_pic += 3;
		}

	}
}
#endif















