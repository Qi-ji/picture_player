#include <stdio.h>
#include <setjmp.h>
#include <string.h>

#include <config.h>
#include <fb.h>
#include "jpeglib.h"
#include "jerror.h"



struct my_error_mgr {
  struct jpeg_error_mgr pub;    /* "public" fields */
  jmp_buf setjmp_buffer;        /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}

/*
 *function:charge if it is a jpg picture;
 *para: the path and name of the picture
 *return:0--not jpg,1--is jpg, -1--error
 */

static int is_jpeg(const char *path)
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
	if((buf[0] != 0xff) || (buf[1] != 0xd8))
	{
		fclose(file);
		return 0;
	}
	/*不用检查后面两位了，因为发现自己下载的jpg文件，最后是ffd9 0a；
	ret = fseek(file, -3, SEEK_END);
	ret = fread(buf, 1, 2, file);
	debug("the last two bite: 0X%x%x\n", buf[0], buf[1]);
	if((buf[0] != 0xff) || (buf[1] != 0xd9))
	{
		fclose(file);
		return 0;
	}
	*/
	
	fclose(file);
	return 1;
	
}


static int jpeg_analyze (pic_info  *pjpeg)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	
	FILE * infile;                /* source file */
    //JSAMPARRAY buffer;            /* Output row buffer */
    char *buffer;					//只有这样才能正常显示
    int row_stride;               /* physical row width in output buffer */

	if ((infile = fopen(pjpeg->pathname, "rb")) == NULL) {
	  fprintf(stderr, "can't open %s\n", pjpeg->pathname);
	  return -1;
     }
	/* Step 1: allocate and initialize JPEG decompression object */
	/* We set up the normal JPEG error routines, then override error_exit. */
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	/* Establish the setjmp return context for my_error_exit to use. */
	if (setjmp(jerr.setjmp_buffer)) {
	  /* If we get here, the JPEG code has signaled an error.
	   * We need to clean up the JPEG object, close the input file, and return.
	   */
	  jpeg_destroy_decompress(&cinfo);
	  fclose(infile);
	  return -1;
	}
								
	jpeg_create_decompress(&cinfo);				/* initialize the JPEG decompression object. */
    jpeg_stdio_src(&cinfo, infile);				/* Step 2: specify data source (eg, a file) */
    (void) jpeg_read_header(&cinfo, TRUE);		/* Step 3: read file parameters with jpeg_read_header() */  
   												/* Step 4: set parameters for decompression */	
    (void) jpeg_start_decompress(&cinfo);		/* Step 5: Start decompressor */
	row_stride = cinfo.output_width * cinfo.output_components;/* JSAMPLEs per row in output buffer */
	/* output_width: scaled image width */ /*  nominal image: width (from SOF marker)   */
	 /* Make a one-row-high sample array that will go away when done with image */
	//buffer = (*cinfo.mem->alloc_sarray)
			//((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	buffer = (char *)malloc(row_stride);
   
	 /* Step 6: while (scan lines remain to be read) */
	while (cinfo.output_scanline < cinfo.output_height) {
		 (void) jpeg_read_scanlines(&cinfo, &buffer, 1);
		 /* Assume put_scanline_someplace wants a pointer and sample count. */
		 memcpy(pjpeg->pic_date + \
		 (cinfo.output_scanline * row_stride), buffer, row_stride);//这句代码要好好看一下，很关键
		 															//是每次将pic_date后移一行数据的大小
		// put_scanline_someplace(buffer[0], row_stride);
	 }
	 
    (void) jpeg_finish_decompress(&cinfo);
	
	pjpeg->height = cinfo.output_height;
	pjpeg->width = cinfo.output_width;
	pjpeg->bpp = cinfo.output_components * 8;
	
    jpeg_destroy_decompress(&cinfo);   
	fclose(infile);

	debug("the jpg height is %d.\n",pjpeg->height);
	debug("the jpg width is %d.\n",pjpeg->width);
	debug("the jpg bpp is %d.\n",pjpeg->bpp);
	
	fb_draw_jpeg_test(pjpeg);

	return 0;
}
	
int display_jpeg(const char *path)
{
	pic_info j_picture;
	int ret;
	
	if(NULL == path)
	{
		fprintf(stderr, "uselee pathname\n");
		return -1;
	}

	ret = is_jpeg(path);
	if(ret != 1)
	{
		fprintf(stderr, "%s is not a jpg picture.\n", path);
		return -1;
	}
	
	j_picture.pathname = path;
	j_picture.pic_date= rgb_buf;

	ret = jpeg_analyze(&j_picture);
	if(ret == -1)
	{
		fprintf(stderr, "jpeg_analyze error.\n");
		return -1;
	}

	return 0;
	
}




