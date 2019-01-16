#ifndef   __FB_BMP_H__
#define   __FB_BMP_H__
#include <config.h>



/***************************structure definition*************************************/		
//BMP file header
typedef struct 
{ 
	unsigned char    bfType[2];// ���� //�ļ�����
	unsigned long    bfSize;//�������� //λͼ��С
	unsigned short   bfReserved1;//    //λ0 
	unsigned short   bfReserved2;//    //λ0
	unsigned long    bfOffBits;// ���� //������ƫ����
} __attribute__((packed)) BitMapFileHeader;// //�ֽڶ������⣬ʹ���СΪ14�ֽ�


//BMP message header
typedef struct 
{ 
	unsigned long    biSize;// ���� ���������� // BitMapFileHeader �ֽ���
	long 			 biWidth;// ������������   //λͼ��� 
	long 			 biHeight;// ��������������//λͼ�߶ȣ���λ���򣬷�֮Ϊ��ͼ 
	unsigned short 	 biPlanes;// ������������  //ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1
	unsigned short   biBitCount;// ��������    //˵��������/���أ�Ϊ1��4��8��16��24����32�� 
	unsigned long    biCompression;// ����     //ͼ������ѹ��������û��ѹ�������ͣ�BI_RGB 
	unsigned long    biSizeImage;// ��������   //˵��ͼ��Ĵ�С�����ֽ�Ϊ��λ 
	long 			 biXPelsPerMeter;//��������//˵��ˮƽ�ֱ��� 
	long 			 biYPelsPerMeter;//��������//˵����ֱ�ֱ��� 
	unsigned long    biClrUsed;//������������  //˵��λͼʵ��ʹ�õĲ�ɫ���е���ɫ������
	unsigned long    biClrImportant;//�������� //��ͼ����ʾ����ҪӰ�����������0����Ҫ�� 
} __attribute__((packed)) BitMapInfoHeader;// 


/***************************function definition*************************************/






#endif


