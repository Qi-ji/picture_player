#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <fb_bmp.h>
#include <config.h>
#include <fb.h>


/*
 *function:charge if it is a jpg picture;
 *para: the path and name of the picture
 *return:0--not jpg,1--is jpg, -1--error
 */
int is_bmp(const char *path)
{
	FILE *file = NULL;
	size_t ret = 0;
	char buf[2] = {0};
	
	if(NULL == path)
	{
		fprintf(stderr, "uselee pathname\n");
		return -1;
	}


	file = fopen(path, "r");
	if(NULL == file)
	{
		fprintf(stderr, "open file %s error.\n", path);
		fclose(file);
		return -1;
	}
	
	ret = fread(buf, 1, 2, file);
	debug("the first two bite: 0X%x%x\n", buf[0], buf[1]);
	if((buf[0] != 'B') || (buf[1] != 'M'))
	{
		fclose(file);
		return 0;
	}
	
	fclose(file);
	return 1;
	
}


/*
 *function:
 *parameter:the path of your bmp path.eg:"123.bmp"
 *return:-1 is error.
 */
static int bmp_analyze(pic_info  *pbmp)
{
	int fd = -1;
	BitMapFileHeader bmp_header;
	BitMapInfoHeader bmp_info;
	ssize_t ret = 0;
	unsigned long bmp_date_len = 0;
	
	if(NULL == pbmp)
		return -1;

	fd = open(pbmp->pathname, O_RDONLY);
	if(fd == -1)
	{
		printf("open %s failed.\n", pbmp->pathname);
		return -1;
	}

	 ret = read(fd, &bmp_header, sizeof(bmp_header));
	 ret = read(fd, &bmp_info, sizeof(bmp_info)); 
	 if(ret < 0)
	 {
		printf("read %s head massage failed.\n", pbmp->pathname);
		close(fd);
		return -1;
	 }
	 
	 //lseek(fd, bmp_header.bfOffBits, SEEK_SET);
	 bmp_date_len = bmp_info.biWidth * bmp_info.biHeight * bmp_info.biBitCount / 3;
	 ret = read(fd, rgb_buf, bmp_date_len);

	 pbmp->width = bmp_info.biWidth;
	 pbmp->height = bmp_info.biHeight;
	 pbmp->bpp = bmp_info.biBitCount;
	 //pbmp->pic_date = rgb_buf;
/*
	if(bmp_head_buf[0]!='B' || bmp_head_buf[1]!='M')
	{
	    printf("%s is not a bmp picture.\n", path);
		return -1;	
	}
*/
	debug("the bmp bfSize is %ld\n", bmp_header.bfSize);
	debug("the bmp biHeight is %d\n",pbmp->height);
	debug("the bmp biWidth is %d\n",pbmp->width);	

	fb_draw_picture_test(pbmp);
	
	close(fd);
	
	return 0;
}


	
int bmp_display(const char *path)
{
	pic_info b_picture;
	int ret;
	
	if(NULL == path)
	{
		fprintf(stderr, "uselee pathname\n");
		return -1;
	}

	ret = is_bmp(path);
	if(ret != 1)
	{
		fprintf(stderr, "%s is not a bmp picture.\n", path);
		return -1;
	}
	
	b_picture.pathname = path;
	b_picture.pic_date= rgb_buf;

	ret = bmp_analyze(&b_picture);
	if(ret == -1)
	{
		fprintf(stderr, "jpeg_analyze error.\n");
		return -1;
	}

	return 0;
	
}











