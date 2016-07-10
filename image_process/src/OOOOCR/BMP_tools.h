#ifndef BMP_TOOLS_H
#define BMP_TOOLS_H

#include <stdio.h>


#define	_32_BIT_	4		// 32 bit
#define _24_BIT_	3		// 24 bit
#define _8_BIT_		1		// 8  bit

#define BLUE_OFF_SET	0	// Ū���C�⪺ offset
#define	GREEN_OFF_SET	1
#define RED_OFF_SET		2
#define	ALPHA_OFF_SET	3



typedef	unsigned char	BYTE;


//#pragma pack(1)
typedef struct BMP_FILE_HEADER_DEFINE
{
   unsigned short 	type;
   unsigned int   	size;
   unsigned short 	reserved1;
   unsigned short 	reserved2;
   unsigned int   	off_bits;
} BMP_FILE_HEADER ;
//#pragma pack()


//#pragma pack(1)
typedef struct	BMP_INF_HEADER_DEFINE
{
   unsigned int   	size;					// header size
   unsigned int   	width;
   unsigned int   	height;
   unsigned short 	planes;
   unsigned short 	bit_count;
   unsigned int   	compression;
   unsigned int   	size_image;				// width * height
   unsigned int   	x_pels_per_meter;
   unsigned int   	y_pels_per_meter;
   unsigned int   	clr_used;
   unsigned int   	clr_important;
} BMP_INF_HEADER ;
//#pragma pack()



typedef struct	BMP_DATA_DEFINE
{
	BMP_FILE_HEADER		file_header;
	BMP_INF_HEADER		inf_header;

	BYTE				*data;

} BMP_DATA;













BMP_DATA*			BMP_open( char *filename );													// �}���ɮ�
void				BMP_save( char *filename, BMP_DATA *bmp_data );								// �g�J�ɮ�
void				BMP_free( BMP_DATA *bmp_data );												// ����O����Ŷ�
BMP_DATA*			BMP_new( int width, int height, int depth );								// �s�ؤ@�� BMP �ɮ�  (��ƪ�l�Ƭ�0)

BMP_FILE_HEADER		BMP_file_load( FILE *p );													// Ū�� file header
void				BMP_file_save( FILE *p, BMP_FILE_HEADER file_header );						// �g�J file header
BMP_INF_HEADER		BMP_inf_load( FILE *p );													// Ū�� information header
void				BMP_inf_save( FILE *p, BMP_INF_HEADER inf_header );							// �g�J information header
void				BMP_data_load( FILE *p, BMP_DATA *bmp_data );								// Ū�����ɸ�� 
void				BMP_data_save( FILE *p, BMP_DATA *bmp_data );								// �g�J���ɸ��

BYTE*				BMP_create_data( int width, int height, int depth );						// �|�b�o�� malloc, �إߦs����ɸ�ƪ��Ŷ�

BYTE*				BMP_red_channel(   BMP_DATA *bmp_data, int x, int y );						// Ū��/�g�J ����q�D���
BYTE*				BMP_green_channel( BMP_DATA *bmp_data, int x, int y );						// Ū��/�g�J ���q�D���
BYTE*				BMP_blue_channel(  BMP_DATA *bmp_data, int x, int y );						// Ū��/�g�J �Ŧ�q�D���
BYTE*				BMP_alpha_channel( BMP_DATA *bmp_data, int x, int y );						// Ū��/�g�J alpha �q�D���
BYTE*				BMP_gray_channel(  BMP_DATA *bmp_data, int x, int y );						// Ū��/�g�J �Ƕ��� (�Ƕ��Ϩϥ�)

int					BMP_gray_channel_read(  BMP_DATA *bmp_data, int x, int y );					// Ū�� �Ƕ��� 
void				BMP_gray_channel_write( BMP_DATA *bmp_data, int x, int y, int gray );		// �g�J �Ƕ��� 
void				BMP_pixel( BMP_DATA *bmp_data, int x, int y, int r, int g, int b );			// �g�J�@��pixel (rgb�T��)

int					BMP_width( BMP_DATA *bmp_data );											// ���o�e
int					BMP_height( BMP_DATA *bmp_data );											// ���o��
int					BMP_depth( BMP_DATA *bmp_data );											// ���o�`�� (�줸/8)
				
BMP_DATA*			BMP_copy( BMP_DATA *src );													// �N��Ʊq src �ƻs�� dest



#endif