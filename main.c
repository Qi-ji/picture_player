#include <stdio.h>
#include <unistd.h>


#include <fb.h>
#include <config.h>
#include <fb_bmp.h>

char rgb_buf[RGB_BUF_SIZE];

int main(void)
{
    int ret = -1;
	
	printf("image decode player....\n");

	ret = fb_open();
	if (ret == -1)
	{
        printf("fb open error.\n");
		return -1;
	}
	
/*******************show bmp picture*****************************/	
#if 0 

    fb_draw_back(WIDTH, HEIGHT, WHITE);
	picture_showed.pathname = "./image/motorbike.bmp";
	bmp_analyze(&picture_showed);
	picture_showed.pathname = "./image/medish.bmp";
	bmp_analyze(&picture_showed);
	
#endif

/*******************show jped picture*****************************/
#if 0

    fb_draw_back(WIDTH, HEIGHT, WHITE);
	picture_showed.pathname = "./image/motorbike.jpg";
	picture_showed.pic_date= rgb_buf;
	ret = is_jpeg("./image/motorbike.jpg");
	if(ret == 1)
	{
		debug("is jpg\n");
	}
	jpeg_analyze(&picture_showed);
	picture_showed.pathname = "./image/medish.jpg";
	jpeg_analyze(&picture_showed);
	
#endif

	//display_jpeg("./image/motorbike.jpg");

	//sleep(2);
	
	//display_bmp("./image/12.bmp");
	display_png("./image/motorbike2.png");
	//picture_scan("./image");
	
	fb_close();

	return 0;
}





























