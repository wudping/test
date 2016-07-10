#include "BMP_tools.h"


/*************************************************************************************
	�}���ɮ�
/**************************************************************************************/
BMP_DATA*	BMP_open( char *filename )
{
	FILE		*p;
	BMP_DATA	*bmp_data;

	p	=	fopen( filename, "rb" );

	if( p == NULL )
	{
		printf("error...");
		return	0;
	}

	// ���͸�� 
	bmp_data	=	malloc( sizeof(BMP_DATA) );

	// load header
	//fread( &(bmp_data->file_header), sizeof(BMP_FILE_HEADER), 1, p );
	bmp_data->file_header	=	BMP_file_load(p);

	// load infomation
	//fread( &(bmp_data->inf_header), sizeof(BMP_INF_HEADER), 1, p );
	bmp_data->inf_header	=	BMP_inf_load(p);

	// Ū�����ɸ��
	BMP_data_load( p, bmp_data );

	fclose(p);

	return	bmp_data;
}


/*************************************************************************************
	���o�e
/**************************************************************************************/
int		BMP_width( BMP_DATA *bmp_data )
{
	return	bmp_data->inf_header.width;
}


/*************************************************************************************
	���o��
/**************************************************************************************/
int		BMP_height( BMP_DATA *bmp_data )
{
	return	bmp_data->inf_header.height;
}


/*************************************************************************************
	���o�`�� (���W8)
/**************************************************************************************/
int		BMP_depth( BMP_DATA *bmp_data )
{
	return	bmp_data->inf_header.bit_count/8;
}



/*************************************************************************************
	Ū�����ɸ��
/**************************************************************************************/
void	BMP_data_load( FILE *p, BMP_DATA *bmp_data )
{
	BYTE	*buff;
	BYTE	*data;
	int		width,	height,	depth;
	int		add_4;		// �N�C��ɺ��ܦ� 4 ������ 	
	int		row_width;	// �C�@�檺�e�� = width * depth
	int		i,	j;

	width		=	(int)bmp_data->inf_header.width;
	height		=	(int)bmp_data->inf_header.height;
	depth		=	(int)bmp_data->inf_header.bit_count/8;		// �줸�`��  ��8  �ন�X��byte)  �o��u��B�z 32 24 8bit�Ƕ�

	add_4		=	width*depth + 4 - (width*depth)%4;			// ���C���ܦ��|������
	row_width	=	width*depth;								// �C��e�� = width * depth
	
	// �C�楲���O add_4 ������
	buff	=	malloc( sizeof(BYTE) * add_4 );

	// �|�b�o�� malloc
	bmp_data->data	=	BMP_create_data( width, height, depth );
	data			=	bmp_data->data;		// �����b���o�Ŷ���~��o�˼g ���M�|�z��

	// �}�l�i��Ū����� �@���@��
	for( j = 0; j < height; j++ )
	{
		fread( buff, sizeof(BYTE), add_4, p );		// �C�@�楲��Ū�� add_4 ���e�� (���L���� 4 ������)

		for( i = 0; i < row_width; i++ )
			          // ��ƥ������� (�]�����ɮ榡) 
					  // �C�@�� offset = width*depth �C�@�檺�e��
			*( data + (height-j-1)*row_width + i )	=	*( buff + i );
	}

	free(buff);
}



/*************************************************************************************
	�g�J���ɸ��
/**************************************************************************************/
void	BMP_data_save( FILE *p, BMP_DATA *bmp_data )
{
	BYTE	*buff;
	BYTE	*data;
	int		width,	height,	depth;
	int		add_4;		// �N�C��ɺ��ܦ� 4 ������ 	
	int		row_width;	// �C�@�檺�e�� = width * depth
	int		i,	j;

	width		=	(int)bmp_data->inf_header.width;
	height		=	(int)bmp_data->inf_header.height;
	depth		=	(int)bmp_data->inf_header.bit_count/8;		// �줸�`��  ��8  �ন�X��byte)  �o��u��B�z 32 24 8bit�Ƕ�

	add_4		=	width*depth + 4 - (width*depth)%4;			// ���C���ܦ��|������
	row_width	=	width*depth;								// �C��e�� = width * depth
	
	// �C�楲���O add_4 ������
	buff	=	malloc( sizeof(BYTE) * add_4 );
	data	=	bmp_data->data;		

	// �}�l�i��g�J��� �@���@��
	for( j = 0; j < height; j++ )
	{

		for( i = 0; i < row_width; i++ )
			          // ��ƥ������� (�]�����ɮ榡) 
					  // �C�@�� offset = width*depth �C�@�檺�e��
			*( buff + i )	=	*( data + (height-j-1)*row_width + i );

		fwrite( buff, sizeof(BYTE), add_4, p );		// �C�@�楲��Ū�� add_4 ���e�� (���L���� 4 ������)
	}

	free(buff);
}



/*************************************************************************************
	�g�J�ɮ�
/**************************************************************************************/
void	BMP_save( char *filename, BMP_DATA *bmp_data )
{
	FILE	*p;
	
	p	=	fopen( filename, "wb+" );

	if( p == NULL )
	{
		printf("error...");
		return	0;
	}

	// save header
	BMP_file_save( p, bmp_data->file_header );

	// save infomation
	BMP_inf_save( p, bmp_data->inf_header );

	// �g�J���ɸ��
	BMP_data_save( p, bmp_data );

	fclose(p);
}




/*************************************************************************************
	�s�ؤ@�� BMP �ɮ�  (��ƪ�l�Ƭ�0)
/**************************************************************************************/
BMP_DATA*	BMP_new( int width, int height, int depth )
{
	BMP_DATA	*bmp_data;

	bmp_data	=	malloc( sizeof(BMP_DATA) );

	switch( depth )
	{
		case _24_BIT_ :

			bmp_data->file_header.type				=	19778;
			bmp_data->file_header.size				=	2230;
			bmp_data->file_header.reserved1			=	0;
			bmp_data->file_header.reserved2			=	0;
			bmp_data->file_header.off_bits			=	54;

			bmp_data->inf_header.size				=	40;
			bmp_data->inf_header.width				=	width;
			bmp_data->inf_header.height				=	height;
			bmp_data->inf_header.planes				=	1;
			bmp_data->inf_header.bit_count			=	24;			
			bmp_data->inf_header.compression		=	0;
			bmp_data->inf_header.size_image			=	0;
			bmp_data->inf_header.x_pels_per_meter	=	3780;
			bmp_data->inf_header.y_pels_per_meter	=	3780;
			bmp_data->inf_header.clr_important		=	0;
			bmp_data->inf_header.clr_used			=	0;

			break;

		default:
			printf("error");
	}

	// ��l�ƪŶ��t�m
	bmp_data->data	=	BMP_create_data( width, height, depth );
	// �k�s
	memset( bmp_data->data, 0, width*height*depth );

	return	bmp_data;
}




/*************************************************************************************
	�N��Ʊq src �ƻs�� dest
/**************************************************************************************/
BMP_DATA*	BMP_copy( BMP_DATA *src )
{
	BMP_DATA	*dest;

	int		width;
	int		height;
	int		depth;

	width	=	src->inf_header.width;
	height	=	src->inf_header.height;
	depth	=	src->inf_header.bit_count/8;

	dest	=	malloc( sizeof(BMP_DATA) );

	// �ƻs file header
	memcpy( &(dest->file_header), &src->file_header, sizeof(BMP_FILE_HEADER) );

	// �ƻs information header
	memcpy( &(dest->inf_header), &src->inf_header, sizeof(BMP_INF_HEADER) );

	// ���o�Ŷ�
	dest->data	=	BMP_create_data( width, height, depth );

	// �ƻs���
	memcpy( dest->data, src->data, sizeof(BYTE)*width*height*depth );

	return	dest;
}


/*************************************************************************************
	�g�J�@��pixel (rgb�T��)
/**************************************************************************************/
void	BMP_pixel( BMP_DATA *bmp_data, int x, int y, int r, int g, int b )
{	
	int		depth	=	bmp_data->inf_header.bit_count/8;
	int		gray;
	
	if( depth >= _24_BIT_ )
	{
		*BMP_red_channel( bmp_data, x, y )		=	r;
		*BMP_green_channel( bmp_data, x, y )	=	g;
		*BMP_blue_channel( bmp_data, x, y )		=	b;
	}
	else
	{
		gray	=	( r*11 + g*16 + b*5 )/32;
		*BMP_gray_channel( bmp_data, x, y )		=	gray;
	}
}



/*************************************************************************************
	Ū��/�g�J ����q�D���
/**************************************************************************************/
BYTE*	BMP_red_channel( BMP_DATA *bmp_data, int x, int y )
{
	BYTE	*r;		// �^�Ǹ��

	BYTE	*data	=	bmp_data->data;
	int		width	=	bmp_data->inf_header.width;
	int		height	=	bmp_data->inf_header.height;
	int		depth	=	bmp_data->inf_header.bit_count/8;

	        // �첾�p�� �Ϥ���ư��]�O BGR BGR BGR  ��   BGRA BGRA BGRA ��  �Ƕ�
	r	=	data + y*width*depth + x*depth;
	r	+=	depth >= _24_BIT_ ? RED_OFF_SET : 0;

	return	r;
}



/*************************************************************************************
	Ū��/�g�J ���q�D���
/**************************************************************************************/
BYTE*	BMP_green_channel( BMP_DATA *bmp_data, int x, int y )
{
	BYTE	*g;		// �^�Ǹ��

	BYTE	*data	=	bmp_data->data;
	int		width	=	bmp_data->inf_header.width;
	int		height	=	bmp_data->inf_header.height;
	int		depth	=	bmp_data->inf_header.bit_count/8;

	        // �첾�p�� �Ϥ���ư��]�O BGR BGR BGR  ��   BGRA BGRA BGRA ��  �Ƕ�
	g	=	data + y*width*depth + x*depth;
	g	+=	depth >= _24_BIT_ ? GREEN_OFF_SET : 0;

	return	g;
}


/*************************************************************************************
	Ū��/�g�J �Ŧ�q�D���
/**************************************************************************************/
BYTE*	BMP_blue_channel( BMP_DATA *bmp_data, int x, int y )
{
	BYTE	*b;		// �^�Ǹ��

	BYTE	*data	=	bmp_data->data;
	int		width	=	bmp_data->inf_header.width;
	int		height	=	bmp_data->inf_header.height;
	int		depth	=	bmp_data->inf_header.bit_count/8;

	        // �첾�p�� �Ϥ���ư��]�O BGR BGR BGR  ��   BGRA BGRA BGRA ��  �Ƕ�
	b	=	data + y*width*depth + x*depth;
	b	+=	depth >= _24_BIT_ ? BLUE_OFF_SET : 0;

	return	b;
}



/*************************************************************************************
	Ū��/�g�J alpha �q�D���
/**************************************************************************************/
BYTE*	BMP_alpha_channel( BMP_DATA *bmp_data, int x, int y )
{
	BYTE	*alpha;		// �^�Ǹ��

	BYTE	*data	=	bmp_data->data;
	int		width	=	bmp_data->inf_header.width;
	int		height	=	bmp_data->inf_header.height;
	int		depth	=	bmp_data->inf_header.bit_count/8;

	if( depth < _32_BIT_ )
		printf( "error..." );

	        // �첾�p�� �Ϥ���ư��]�O BGR BGR BGR  ��   BGRA BGRA BGRA ��  �Ƕ�
	alpha	=	data + y*width*depth + x*depth;
	alpha	+=	depth >= _32_BIT_ ? ALPHA_OFF_SET : 0;

	return	alpha;		// �p�G�榡���� �o��O�H�K��@�Ӧ�m�^�h
}



/*************************************************************************************
	Ū��/�g�J �Ƕ� �q�D��� (�Ƕ��ϱM��)
/**************************************************************************************/
BYTE*	BMP_gray_channel( BMP_DATA *bmp_data, int x, int y )
{
	BYTE	*gray;		// �^�Ǹ��

	BYTE	*data	=	bmp_data->data;
	int		width	=	bmp_data->inf_header.width;
	int		height	=	bmp_data->inf_header.height;
	int		depth	=	bmp_data->inf_header.bit_count/8;

	if( depth > _8_BIT_ )
		printf( "error..." );

	        // �첾�p�� �Ϥ���ư��]�O BGR BGR BGR  ��   BGRA BGRA BGRA ��  �Ƕ�
	gray	=	data + y*width*depth + x*depth;

	return	gray;		// �p�G�榡���� �o��O�H�K��@�Ӧ�m�^�h
}



/*************************************************************************************
	Ū�� �Ƕ��� 
/**************************************************************************************/
int		BMP_gray_channel_read(  BMP_DATA *bmp_data, int x, int y )
{
	int		gray;		// �^�Ǹ��
	int		red,	green,	blue;

	BYTE	*data	=	bmp_data->data;
	int		width	=	bmp_data->inf_header.width;
	int		height	=	bmp_data->inf_header.height;
	int		depth	=	bmp_data->inf_header.bit_count/8;

	if( depth > _8_BIT_ )
	{
		red		=	*BMP_red_channel( bmp_data, x, y );
		green	=	*BMP_green_channel( bmp_data, x, y );
		blue	=	*BMP_blue_channel( bmp_data, x, y );
		gray	=	( red*11 + green*16 + blue*5 )/32;
	}
	else
	{
		gray	=	*BMP_gray_channel( bmp_data, x, y );
	}

	return	gray;
}


/*************************************************************************************
	�g�J �Ƕ��� 
/**************************************************************************************/
void	BMP_gray_channel_write( BMP_DATA *bmp_data, int x, int y, int gray )
{
	BYTE	*data	=	bmp_data->data;
	int		width	=	bmp_data->inf_header.width;
	int		height	=	bmp_data->inf_header.height;
	int		depth	=	bmp_data->inf_header.bit_count/8;

	if( depth > _8_BIT_ )
	{
		*BMP_red_channel( bmp_data, x, y )		=	gray;
		*BMP_green_channel( bmp_data, x, y )	=	gray;
		*BMP_blue_channel( bmp_data, x, y )		=	gray;
	}
	else
	{
		*BMP_gray_channel( bmp_data, x, y )		=	gray;
	}

	return	gray;
}





/*************************************************************************************
	Ū�� file header
/**************************************************************************************/
BMP_FILE_HEADER		BMP_file_load( FILE *p )
{
	BMP_FILE_HEADER		file_header;

	fread( &file_header.type,      sizeof(unsigned short), 1, p );
	fread( &file_header.size,      sizeof(unsigned int),   1, p );
	fread( &file_header.reserved1, sizeof(unsigned short), 1, p );
	fread( &file_header.reserved2, sizeof(unsigned short), 1, p );
	fread( &file_header.off_bits,  sizeof(unsigned int),   1, p );

	return	file_header;
}





/*************************************************************************************
	�g�J file header
/**************************************************************************************/
void	BMP_file_save( FILE *p, BMP_FILE_HEADER file_header )
{
	fwrite( &file_header.type,      sizeof(unsigned short), 1, p );
	fwrite( &file_header.size,      sizeof(unsigned int),   1, p );
	fwrite( &file_header.reserved1, sizeof(unsigned short), 1, p );
	fwrite( &file_header.reserved2, sizeof(unsigned short), 1, p );
	fwrite( &file_header.off_bits,  sizeof(unsigned int),   1, p );
}




/*************************************************************************************
	Ū�� information header
/**************************************************************************************/
BMP_INF_HEADER		BMP_inf_load( FILE *p )
{
	BMP_INF_HEADER	inf_header;

	fread( &inf_header.size,             sizeof(unsigned int),   1, p );
	fread( &inf_header.width,            sizeof(unsigned int),   1, p );
	fread( &inf_header.height,           sizeof(unsigned int),   1, p );
	fread( &inf_header.planes,           sizeof(unsigned short), 1, p );
	fread( &inf_header.bit_count,        sizeof(unsigned short), 1, p );
	fread( &inf_header.compression,      sizeof(unsigned int),   1, p );
	fread( &inf_header.size_image,       sizeof(unsigned int),   1, p );
	fread( &inf_header.x_pels_per_meter, sizeof(unsigned int),   1, p );
	fread( &inf_header.y_pels_per_meter, sizeof(unsigned int),   1, p );
	fread( &inf_header.clr_used,         sizeof(unsigned int),   1, p );
	fread( &inf_header.clr_important,    sizeof(unsigned int),   1, p );

	return	inf_header;
}




/*************************************************************************************
	�g�� information header
/**************************************************************************************/
void	BMP_inf_save( FILE *p, BMP_INF_HEADER inf_header )
{
	fwrite( &inf_header.size,             sizeof(unsigned int),   1, p );
	fwrite( &inf_header.width,            sizeof(unsigned int),   1, p );
	fwrite( &inf_header.height,           sizeof(unsigned int),   1, p );
	fwrite( &inf_header.planes,           sizeof(unsigned short), 1, p );
	fwrite( &inf_header.bit_count,        sizeof(unsigned short), 1, p );
	fwrite( &inf_header.compression,      sizeof(unsigned int),   1, p );
	fwrite( &inf_header.size_image,       sizeof(unsigned int),   1, p );
	fwrite( &inf_header.x_pels_per_meter, sizeof(unsigned int),   1, p );
	fwrite( &inf_header.y_pels_per_meter, sizeof(unsigned int),   1, p );
	fwrite( &inf_header.clr_used,         sizeof(unsigned int),   1, p );
	fwrite( &inf_header.clr_important,    sizeof(unsigned int),   1, p );
}



/*************************************************************************************
	�|�b�o�� malloc
/**************************************************************************************/
BYTE*	BMP_create_data( int width, int height, int depth )
{
	BYTE	*data;

	data	=	malloc( sizeof(BYTE) * width * height * depth );
	memset( data, 0, sizeof(BYTE)*width*height*depth );

	return	data;
}



/*************************************************************************************
	����O����Ŷ�
/**************************************************************************************/
void	BMP_free( BMP_DATA *bmp_data )
{
	free( bmp_data->data );
	bmp_data->data	=	NULL;

	free( bmp_data );
	bmp_data	=	NULL;

}