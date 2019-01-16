#ifndef __IMAGE_MANAGE_H__
#define __IMAGE_MANAGE_H__

#define PATHNAMW_MAX_NUM 256


typedef enum pic_type
{
	PIC_BMP,
	PIC_JPG,
	PIC_PNG,
	PIC_UNKOWN,
}pic_type_t;

typedef struct pic_info
{
	char 		pathname[PATHNAMW_MAX_NUM];
	pic_type_t 	 type;
}pic_info_t;





#endif 

