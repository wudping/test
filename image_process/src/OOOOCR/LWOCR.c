#include "LWOCR.h"



#ifdef _OCR_USE_FLOAT_
double		add_value	=	-1;		// �ɭȸ��
#else
short		add_value	=	-1;
#endif



/******************************************************************************
	�^�ǹϤ����̤j��
*******************************************************************************/
short	LWO_max( BYTE *img )
{
	short	i;
	short	max;

	max		=	-1;
	for( i = 0; i < LWO_IMAGE_SIZE; i++ )
	{
		if( max < LWO_IMAGE_PIXEL( img, i ) )
			max	=	LWO_IMAGE_PIXEL( img, i );
	}
	return	max;
}



/******************************************************************************
	�^�ǹϤ������p��
*******************************************************************************/
short	LWO_min( BYTE *img )
{
	short	i;
	short	min;

	min		=	9999;
	for( i = 0; i < LWO_IMAGE_SIZE; i++ )
	{
		if( min > LWO_IMAGE_PIXEL( img, i ) )
			min	=	LWO_IMAGE_PIXEL( img, i );
	}

	return	min;
}







/******************************************************************************
	��Ϥ���binarized
	img   = ��l�Ϥ�
	bin   = �s��G�ȹϪ���m
	value = �G�Ȥƫ� �D�s���ȸө�h�� (��K�ݹ�)

	�� cetner�h��@�M��p�⥭�� �зǮt 
*******************************************************************************/
void		LWO_binary( BYTE *img, BYTE *bin )
{
	short	i;

#ifdef _OCR_USE_FLOAT_
	double	sum,	mean,	devia;
	double	sum2,	mean2,	devia2;		// �Ⱚ�Ȫ�mean �ΨӸɭ�
#else
	int		sum;
	short	mean,	devia;
	int		sum2;
	short	mean2,	devia2;
#endif

	short	itemp;
#ifdef _OCR_USE_FLOAT_
	double	dtemp;
#else
	int		dtemp;
#endif

	short	max,	min,	count;
	short	count2;		//�Ⱚ�Ȫ�count
#ifdef _OCR_USE_FLOAT_
	double	center;
#else
	short	center;
#endif

#ifdef _OCR_USE_FLOAT_
	double	THR;
#else
	short	THR;
#endif

	add_value	=	-1;		// �Ψ��ˬd�� �P�_�ɭȸ�ƬO�_���Q�קﱼ

	max		=	LWO_max( img );
	min		=	LWO_min( img );

	// �γ̤j�̤p�������h����@�M �A��ѤU������
#ifdef _OCR_USE_FLOAT_
	center	=	0.5 * (max + min);
#else
	center	=	(max+min)/2;
#endif

	// �p�⥭��
	sum		=	0;
	sum2	=	0;
	count	=	0;
	count2	=	0;
	for( i = 0; i < LWO_IMAGE_SIZE; i++ )
	{
		itemp	=	LWO_IMAGE_PIXEL( img, i );
#ifdef _OCR_USE_FLOAT_
		if( itemp < center )
#else
		if( itemp <= center )		// �]����short �n�h����
#endif
		{
			sum	+=	itemp;
			count++;
		}
		else	// �Ⱚ�ȰϪ�mean
		{
			sum2	+=	itemp;
			count2++;
		}
	}
	mean	=	sum / count;
	mean2	=	sum2 / count2;

	// �зǮt
	sum		=	0;
	sum2	=	0;
	count	=	0;
	count2	=	0;
	for( i = 0; i < LWO_IMAGE_SIZE; i++ )
	{
		itemp	=	LWO_IMAGE_PIXEL( img, i );
#ifdef _OCR_USE_FLOAT_
		if( itemp < center )
#else
		if( itemp <= center )
#endif
		{
			dtemp	=	( itemp - mean ) * ( itemp - mean );
			sum		+=	dtemp;
			count++;
		}
		else
		{
			dtemp	=	( itemp - mean2 ) * ( itemp - mean2 );
			sum2		+=	dtemp;
			count2++;
		}
	}
	devia	=	sqrt( 1.0 * sum / count );
	devia2	=	sqrt( 1.0 * sum2 / count2 );

#ifdef _OCR_USE_FLOAT_
	set_bg_data( mean2 + 0.5*devia2 );		// �Q�ΰ��Ȫ������h�ɭ�
#else
	set_bg_data( mean2 + devia2/2 );
#endif

#ifdef _OCR_USE_FLOAT_
	THR		=	mean + 0.5*devia;		// �G�Ȥƪ� threshold
#else
	THR		=	mean + devia / 2;
#endif
	// �G�Ȥ�
	for( i = 0; i < LWO_IMAGE_SIZE; i++ )
	{
		itemp	=	LWO_IMAGE_PIXEL( img, i );
#ifdef _OCR_USE_FLOAT_
		if( itemp < THR )
#else
		if( itemp <= THR )	// ���ƭn�[����
#endif
			LWO_IMAGE_PIXEL( bin, i )	=	LWO_BIN_VALUE;
		else
			LWO_IMAGE_PIXEL( bin, i )	=	0;
	}
}





/******************************************************************************
	����
*******************************************************************************/
void		LWO_bin_rev( BYTE *bin )
{
	short	i,	j;

	for( i = 0; i < LWO_IMAGE_SIZE; i++ )
	{
		if( bin[i] == 0 )
			bin[i]	=	LWO_BIN_VALUE;
		else
			bin[i]	=	0;
	}
}




/******************************************************************************
	�j�M�X�@�Ӥj���� �N�r�ئ� 
	�o��O�����j�M�� �W �k �U �� ���I
	�|�s���T�@�_�ئ�
*******************************************************************************/
void	LWO_crop_big_boxing( CROP_DATA *crop_data )
{
	short	top		=	-1,					// �� -1 ��ܨS�j�M��
			bottom	=	-1,	
			left	=	-1,	
			right	=	-1;

	short	i,	j;

	BYTE	*bin	=	crop_data->bin;		// �G�ȤƹϤ����

	// top
	for( j = 0; j < LWO_IMAGE_HEIGHT; j++ )
	{
		for( i = 0; i < LWO_IMAGE_WIDTH; i++ )
		{
			if( LWO_IMAGE_XY( bin, i, j ) == LWO_BIN_VALUE )
			{
				top		=	j;
				break;
			}
		}
		if( top >= 0 )		// �w�g�j�M���� �������X
			break;
	}
	// bottom
	for( j = LWO_IMAGE_HEIGHT - 1; j >= 0; j-- )
	{
		for( i = 0; i < LWO_IMAGE_WIDTH; i++ )
		{
			if( LWO_IMAGE_XY( bin, i, j ) == LWO_BIN_VALUE )
			{
				bottom	=	j;
				break;
			}
		}
		if( bottom >= 0 )
			break;
	}
	// left
	for( i = 0; i < LWO_IMAGE_WIDTH; i++ )
	{
		for( j = 0; j < LWO_IMAGE_HEIGHT; j++ )
		{
			if( LWO_IMAGE_XY( bin, i, j ) == LWO_BIN_VALUE )
			{
				left	=	i;
				break;
			}
		}
		if( left >= 0 )
			break;
	}
	// right
	for( i = LWO_IMAGE_WIDTH - 1; i >= 0; i-- )
	{
		for( j = 0; j < LWO_IMAGE_HEIGHT; j++ )
		{
			if( LWO_IMAGE_XY( bin, i, j ) == LWO_BIN_VALUE )
			{
				right	=	i;
				break;
			}
		}
		if( right >= 0 )
			break;
	}

	if( top == -1 )		top		=	0;
	if( bottom == -1 )	bottom	=	LWO_IMAGE_HEIGHT - 1;
	if( left == -1 )	left	=	0;
	if( right == -1 )	right	=	LWO_IMAGE_WIDTH - 1;		// �B�z�S�r�����p

	// �^�g���
	crop_data->top		=	top;
	crop_data->left		=	left;
	crop_data->bottom	=	bottom;
	crop_data->right	=	right;

}


/******************************************************************************
	�]�w�ɭȸ��
*******************************************************************************/
#ifdef _OCR_USE_FLOAT_
static void		set_bg_data( double value )
#else
static void		set_bg_data( short value )
#endif
{
	add_value	=	value;
}




/******************************************************************************
	�N�ة����Y (�I��Y�@�泣�S���I �N�����Y)
	���S���I�o�I�i�H�վ�
	�P�ɷ|�j�M�X�@�����S�����T�� ��/�C �����ɭȸ�� 
	(�j�M����N���ɭ� �������p)
	�p�G�j�M�L�{�I��Y�@ ��/�C �I�ƫܦh �N�������� 
	(����r�_�� �[�t ����  �� L �o�ز���r�L�ĪG)
*******************************************************************************/
void	LWO_crop_shrink_boxing( CROP_DATA *crop_data )
{
	short	top			=	crop_data->top,			// ���X�j�ت����
			bottom		=	crop_data->bottom,	
			left		=	crop_data->left,	
			right		=	crop_data->right;

	BYTE	*img		=	crop_data->img;			// �Ϥ����
	BYTE	*bin		=	crop_data->bin;			// �G�ȤƹϤ����

	short	i,	j;
	short	itemp;
	short	count;

	// �]���Q�h�ˬd�ۤv  �ҥH���� left + 1   �H�U�H������
	// left
	for( i = left; i < LWO_IMAGE_WIDTH; i++ )
	{
		count	=	0;
		for( j = 0; j < LWO_IMAGE_HEIGHT; j++ )
		{
			itemp	=	LWO_IMAGE_XY( bin, i, j ) > 0 ? 1 : 0;		// �o���r�]�p���զ�  �I���¦�   �p�G�A�˷|��
			count	+=	itemp;
		}
		if( count == 0 )
			left	=	i + 1;			// �`�N�o�� + 1  !!

		if( count > LWO_IS_WORD )
			break;
	}
	// right
	for( i = right; i >= 0; i-- )
	{
		count	=	0;
		for( j = 0; j < LWO_IMAGE_HEIGHT; j++ )
		{
			itemp	=	LWO_IMAGE_XY( bin, i, j ) > 0 ? 1 : 0;
			count	+=	itemp;
		}
		if( count == 0 )
			right	=	i - 1;

		if( count > LWO_IS_WORD )
			break;
	}
	// top
	for( j = top; j < LWO_IMAGE_HEIGHT; j++ )		// �o�䰲�]�@�w���r  �ҥH�j�M�d��q�Y���  �S���r���ܷ|�X���D
	{
		count	=	0;
		for( i = 0; i < LWO_IMAGE_WIDTH; i++ )
		{
			itemp	=	LWO_IMAGE_XY( bin, i, j ) > 0 ? 1 : 0;		// �ন 0/1
			count	+=	itemp;
		}
		if( count == 0 )				// ���Y
			top	=	j + 1;				// �� j �� ����0, �ҥH���Y�@��

		if( count > LWO_IS_WORD )		// �{�����r  �O�]���o��code �ҥH�j�M�d��~�i�H�쩳  �n���M�|��bug   (�򥻤W�N�O���_������)
			break;
	}
	// bottom
	for( j = bottom; j >= 0; j-- )
	{
		count	=	0;
		for( i = 0; i < LWO_IMAGE_WIDTH; i++ )
		{
			itemp	=	LWO_IMAGE_XY( bin, i, j ) > 0 ? 1 : 0;
			count	+=	itemp;
		}
		if( count == 0 )
			bottom	=	j - 1;

		if( count > LWO_IS_WORD )
			break;
	}

	// �ˬd�O�_�����D
	if( top >= bottom )		
	{
		top		=	0;
		bottom	=	LWO_IMAGE_HEIGHT - 1;
	}
	if( left >= right )
	{
		left	=	0;
		right	=	LWO_IMAGE_WIDTH - 1;
	}

	// �^�g���
	crop_data->top		=	top;
	crop_data->bottom	=	bottom;
	crop_data->left		=	left;
	crop_data->right	=	right;
}



/******************************************************************************
	���o�ɭȸ��
	�o��ĥδ`���覡 (�Ϥ���V)
*******************************************************************************/
#ifdef _OCR_USE_FLOAT_
static double		get_bg_data()
#else
static short		get_bg_data()
#endif
{
	if( add_value > 0 )
		return	add_value;
	else
	{
		printf("error...\n");
		return	0;
	}
}






/******************************************************************************
	�N�Ϥ��� resize �ʧ@ 
	�e�� �� (w,h) -> (nw, nh)

	�o��Obilinear������ 

	clip_img �O�ŵ������ �e�� = (w,h)
	�Ʊ��ন (nw, nh)
	�åB�N��Ʀs�b img

	http://tech-algorithm.com/articles/bilinear-image-scaling/  �Ѧҳo�䪺code�h��g
    int[] temp = new int[w2*h2] ;
    int A, B, C, D, x, y, index, gray ;

    float x_ratio = ((float)(w-1))/w2 ;
    float y_ratio = ((float)(h-1))/h2 ;
    float x_diff, y_diff, ya, yb ;

    int offset = 0 ;
    for (int i=0;i<h2;i++) {
        for (int j=0;j<w2;j++) {
            x = (int)(x_ratio * j) ;
            y = (int)(y_ratio * i) ;
            x_diff = (x_ratio * j) - x ;
            y_diff = (y_ratio * i) - y ;
            index = y*w+x ;

            // range is 0 to 255 thus bitwise AND with 0xff
            A = pixels[index] & 0xff ;
            B = pixels[index+1] & 0xff ;
            C = pixels[index+w] & 0xff ;
            D = pixels[index+w+1] & 0xff ;
            
            // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
            gray = (int)(
                    A*(1-x_diff)*(1-y_diff) +  B*(x_diff)*(1-y_diff) +
                    C*(y_diff)*(1-x_diff)   +  D*(x_diff*y_diff)
                    ) ;

            temp[offset++] = gray ;                                   
        }
    }
    return temp ;

*******************************************************************************/
void		LWO_scaled_bilinear( BYTE *img, BYTE *clip_img, short w, short h, short nw, short nh )
{
	// note: bilinear�S����float  �ݭn�A��

	// src :  pixels ���� clip_img
	// temp ���� img (�̫�s�b�o��)
	int		i,	j;

	int		A,	B,	C,	D,	x,	y,	index,	gray;
	double	x_ratio	=	((double)(w-1))/nw;
	double	y_ratio	=	((double)(h-1))/nh;
	double	x_diff,	y_diff,	ya,	yb;

	int		offset	=	0;

	for( i = 0; i < nh; i++ )
	{
		for( j = 0; j < nw; j++ )	// j = �e  i = ��  �򥭱`�ߺD���P �n�`�N
		{
			x		=	(int)( x_ratio * j );
			y		=	(int)( y_ratio * i );
			x_diff	=	( x_ratio * j ) - x;
			y_diff	=	( y_ratio * i ) - y;
			index	=	y * w + x;

			// �N����ন 0~255
			A		=	*( clip_img + index )         & 0xff ;
			B		=	*( clip_img + index + 1 )     & 0xff ;
			C		=	*( clip_img + index + w )     & 0xff ;
			D		=	*( clip_img + index + w + 1 ) & 0xff ;

			// Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
			gray	=	(int)( A*(1-x_diff)*(1-y_diff) + B*(x_diff)*(1-y_diff) + C*(y_diff)*(1-x_diff) + D*(x_diff*y_diff) ) ;	

			*( img + offset++ )	=	gray;	// �g�^�h
		}

	}
}


/******************************************************************************
	crop �� w/h �L�p
	�o�ɭԿ���X�i �m�� ������resize ����resize
	�j�����O I + ż��
	�ֳ����O L T �_��
*******************************************************************************/
void	LWO_wh_small( CROP_DATA *crop_data )
{
	LWO_crop_big_boxing( crop_data );	// �ؤj�@�I
}



/******************************************************************************
	�p�� histogram (��binary data)
*******************************************************************************/
static void		get_histogram( CROP_DATA *crop_data )
{
	BYTE	*bin	=	crop_data->bin;

	short	i,	j;
	short	count;

	// ����
	for( i = 0; i < LWO_IMAGE_WIDTH; i++ )
	{
		count	=	0;
		for( j = 0; j < LWO_IMAGE_HEIGHT; j++ )
		{
			if( LWO_IMAGE_XY( bin, i, j ) == LWO_BIN_VALUE )
				count++;
		}
		crop_data->histogram_horizon[i]		=	count;
	}
	// ����
	for( j = 0; j < LWO_IMAGE_HEIGHT; j++ )
	{
		count	=	0;
		for( i = 0; i < LWO_IMAGE_WIDTH; i++ )
		{
			if( LWO_IMAGE_XY( bin, i, j ) == LWO_BIN_VALUE )
				count++;
		}
		crop_data->histogram_vertical[j]	=	count;
	}
}


/******************************************************************************
	�j��Y
	�n�`�N���ऺ�Y�L�Y (�S�����T�S���_��)
	���j�Mhistogram, �D�̤j�����߹j�}
	                 ���j��̤j�t���h �h���������� (�Ҧp U )
					 �åB����X�{�b��t
	�����]���I���e���X�{�b�W�U �H���k���D
	�S�����߹j�} �|�o�ͦ��Y�L�ת����D
*******************************************************************************/
void		LWO_force_pinch( CROP_DATA *crop_data )
{
	short	top		=	crop_data->top,
			left	=	crop_data->left,
			bottom	=	crop_data->bottom,
			right	=	crop_data->right;

	short	histogram_horizon[LWO_IMAGE_WIDTH];
	short	histogram_vertical[LWO_IMAGE_HEIGHT];

	short	i;
	short	tmp;
	short	max,		max_id;
	short	center_1,	center_2;	// ���� 1, 2���y��
	short	max_1,		max_2;		// �̤j ���j

	memcpy( histogram_horizon, crop_data->histogram_horizon, sizeof(short)*LWO_IMAGE_WIDTH );			// ���F��K�j�M �ƻs���
	memcpy( histogram_vertical, crop_data->histogram_vertical, sizeof(short)*LWO_IMAGE_HEIGHT );		// ���F��K�j�M �ƻs���

	// �j�Mhistogram max
	max		=	0;
	for( i = 2; i < LWO_IMAGE_WIDTH - 2; i++ )		// �]���b��t���i��O���I
	{
		if( histogram_horizon[i] > max )
		{
			max		=	histogram_horizon[i];
			max_id	=	i;
		}
	}
	center_1					=	max_id;
	max_1						=	max;
	histogram_horizon[center_1]	=	-1;				// �קK�A�׷j�M��
	
	// �j�Mhistogram max ���j
	max		=	0;
	for( i = 2; i < LWO_IMAGE_WIDTH - 2; i++ )		// �]���b��t���i��O���I
	{
		if( histogram_horizon[i] > max )
		{
			max		=	histogram_horizon[i];
			max_id	=	i;
		}
	}
	center_2					=	max_id;
	max_2						=	max;
	histogram_horizon[center_2]	=	-1;	// �קK�A�׷j�M��

	if( max_1 - max_2 <= 1 )		// ���p�r �Ҧp U
	{
		// max���ݭn�洫 �]���S���Φb�Ŷ��W.
		if( center_1 > center_2 )
		{
			tmp			=	center_1;
			center_1	=	center_2;
			center_2	=	tmp;
		}

		// �}�l�i��j��Y  �����V
		// ���䪺���Y
		for( i = left < 0 ? 0 : left; i < center_1; i++ )	// left�b��l�ƫ�N�|�ﱼ��
		{
			if( crop_data->histogram_horizon[i] == 0 )		// �]��histogram_horizon�Q��L �����έ쥻���ӷj�M
				left	=	i + 1;		// �`�N�o�� + 1 !!!!
		}
		// �k�䪺���Y
		for( i = right >= LWO_IMAGE_WIDTH ? LWO_IMAGE_WIDTH-1 : right; i > center_2; i-- )
		{
			if( crop_data->histogram_horizon[i] == 0 )		// �]��histogram_horizon�Q��L �����έ쥻���ӷj�M
				right	=	i - 1;
		}
	}

	else		// ��p�r �Ҧp A
	{
		// �}�l�i��j��Y  �����V
		// ���䪺���Y
		for( i = left; i < center_1; i++ )
		{
			if( crop_data->histogram_horizon[i] == 0 )		// �]��histogram_horizon�Q��L �����έ쥻���ӷj�M
				left	=	i + 1;
		}
		// �k�䪺���Y
		for( i = right; i > center_1; i-- )
		{
			if( crop_data->histogram_horizon[i] == 0 )		// �]��histogram_horizon�Q��L �����έ쥻���ӷj�M
				right	=	i - 1;
		}
	}

	// �g�^���
	crop_data->top		=	top;
	crop_data->left		=	left;
	crop_data->right	=	right;
	crop_data->bottom	=	bottom;

}


/******************************************************************************
	���~�ɤ@�h�� 
	(�վ���� �S�u���ɭ� �^�ǧP�_�O�_�� I )
*******************************************************************************/
short	LWO_add_box( short *_top, short *_bottom, short *_left, short *_right )
{
	short	top		=	*_top,
			bottom	=	*_bottom,
			left	=	*_left,
			right	=	*_right;

	short	is_I	=	-1;

	// �䧹�� �}�l������
	// �o��O���� ���ݭn�S�O�ʧ@
	if( top == 0 && bottom == LWO_IMAGE_HEIGHT - 1 && left == 0 && right == LWO_IMAGE_WIDTH - 1 )
	{
		is_I	=	0;	// ���O I
	}
	// �D���� �ݭn�h���@�� (�ʤֳ����i��ɭ�)
	else
	{
		// �h���@�骺�ʧ@
		top		-=	ADD_VALUE_SIZE;		// ���\ -1
		bottom	+=	ADD_VALUE_SIZE;		// ���\�W�L���

		if( right - left >= LWO_IS_I )
		{
			left	-=	ADD_VALUE_SIZE;
			right	+=	ADD_VALUE_SIZE;
			is_I	=	0;		// ���O I
		}
		else
		{
			is_I	=	1;		// �N��O I
		}
	}

	// �^�g���
	*_top		=	top;
	*_bottom	=	bottom;
	*_left		=	left;
	*_right		=	right;

	return	is_I;
}





/******************************************************************************
	�w�� I  crop�����D��  recrop�������I
*******************************************************************************/
void		LWO_I_denoise( CROP_DATA *crop_data )
{
	short	top		=	crop_data->top,
			bottom	=	crop_data->bottom,
			left	=	crop_data->left,
			right	=	crop_data->right;

	BYTE	*bin	=	crop_data->bin;
	BYTE	*img	=	crop_data->img;

	short	i,	j;

	for( i = 0; i < LWO_IMAGE_WIDTH; i++ )
		for( j = 0; j < LWO_IMAGE_HEIGHT; j++ )
		{
			if( i >= left && i <= right && j >= top && j <= bottom )
				continue;		// �L�ʧ@

			if( LWO_IMAGE_XY( bin, i, j ) == LWO_BIN_VALUE )
				LWO_IMAGE_XY( img, i, j )	=	get_bg_data();
		}
}







/******************************************************************************
	��Ϥ��h�� crop ���ʧ@
	�̭��|�]�t��Ӱʧ@
	1. boxing  �إX�ϧ�
	2. resize  �N�ϧνվ�� 22*32

	�H�U�Ѧ�crop_data�w�q

	img : ��l�v��
	bin : �G�ȤƼv��

	_top, _bottom, _left, _right  �ت��W�U���k�� 

	bg_value      : �H�K��@���S�����I�� ��/�C  �ΨӸɭ�
	bg_value_size : bg_value �� size

	�^�ǭȥΨӧP�_�O�_�� I

*******************************************************************************/
short	LWO_crop( CROP_DATA *crop_data, OCR_INIT_TYPE init_type )
{
	BYTE	buffer_clip_img[ (LWO_IMAGE_WIDTH + ADD_VALUE_SIZE*2) * (LWO_IMAGE_HEIGHT + ADD_VALUE_SIZE*2) ];		// �̤j�W�����W�L�o�ӽd�� �ҥHbuffer���o��j

	BYTE	*img		=	crop_data->img;
	BYTE	*bin		=	crop_data->bin;
	BYTE	*clip_img	=	&(buffer_clip_img[0]);		// �Ϋ��ШӾާ@
//	short	*bg_data	=	crop_data->bg_data;			// ���O�g�J�P�@�Ӱ϶� �ҥH���ݭn���s���V

	short	is_I	=	0;		// �o�Ӧr�O�_�� I
	short	i,	j;
	short	top		=	-1,
			bottom	=	-1,
			left	=	-1,
			right	=	-1;
	short	new_width,	new_height;			// �s���e ��

	double	wh_ratio;				// �u�Τ@���� �O���B�I��
#ifdef _OCR_USE_FLOAT_
	double	cx,		cy;
#else
	short	cx,		cy;				// ����2 �N�i�H���ƭp��F
#endif

	LWO_CROP_TYPE	crop_type	=	LWO_NO_TYPE;		// crop  error���~���B�z

	memset( clip_img, 0, sizeof(BYTE) * ((LWO_IMAGE_WIDTH + ADD_VALUE_SIZE*2) * (LWO_IMAGE_HEIGHT + ADD_VALUE_SIZE*2)) );

	// �j�M�X�@�Ӥj���� �|�s�P���T�@�_�ئ�
	LWO_crop_big_boxing( crop_data );

	// �N�ة����Y (�åB��X�@���S�����I�� ��/�C )
	LWO_crop_shrink_boxing( crop_data );

	// ���o�ت����
	top		=	crop_data->top;
	bottom	=	crop_data->bottom;
	left	=	crop_data->left;
	right	=	crop_data->right;


	// �վ���� ���~�ɤ@�h�� (�o��O�q�X��ɽd�� �S���u���ɭ�)
	is_I	=	LWO_add_box( &top, &bottom, &left, &right );

	// �N�ت���Ƽg�^�hcrop_data
	crop_data->top		=	top;
	crop_data->bottom	=	bottom;
	crop_data->left		=	left;
	crop_data->right	=	right;

	// ���s�]�w�e ��
	if( is_I == 0 )
	{
		new_width	=	right - left + 1;		// �`�N + 1
		new_height	=	bottom - top + 1;
	}
	else		//  I �� �e ���O�쥻���e �]���e�פW���Y��
	{
		new_width	=	LWO_IMAGE_WIDTH;		// �]�� I ������V�Y��
		new_height	=	bottom - top + 1;
	}


	/*
		crop ���` �������B�z
		w/h <= 0.4  �X�i �m�� �������B�z ����resize
		w/h >= 0.8  �j��Y  ���쥻�P�_ (�A���P�_�O�_�� I)
		���߰���    �j��Y  ���쥻�P�_ (�A���P�_�O�_�� I)
		�j��Y�٬O���`�� ���B�z
		���O I ���~�B�z
		�p�G U �����_�� �S�I��j���I �|�X���D
		(�ثe�j�Mhistogram�@���j�M�d��P�_)
		(histogram��n�b��t��pixel�� ���L)
	*/
	wh_ratio	=	1.0 * new_width / new_height;
#ifdef _OCR_USE_FLOAT_
	cx			=	0.5 * ( left + right );
	cy			=	0.5 * ( top + bottom );
#else
	cx			=	left + right;		// ����2 !!
	cy			=	top + bottom;
#endif

	crop_type	=	LWO_NO_TYPE;		// �@�}�l�O�w�]��

	if( is_I == 0 )   // �O I �����L
	{
		if( wh_ratio <= 0.4 && init_type == OCR_CNY_20 )		// �p�B�~�� �]���j�B�|�� 1 �y�����~
		{
			// �p�� histogram
			get_histogram( crop_data );

			// �X�i �m�� �������B�z ����resize
			LWO_wh_small( crop_data );

			// ���~�ɤ@��  (�S�ݬO�_�n�o�˰�)
			LWO_add_box( &crop_data->top, &crop_data->bottom, &crop_data->left, &crop_data->right );

			// ���s�g�J�W�U���k�����  (�]���|�g�bcrop_data�̭�)
			top			=	crop_data->top;
			bottom		=	crop_data->bottom;
			left		=	crop_data->left;
			right		=	crop_data->right;

			// ���s�]�w new_width����
			new_width	=	LWO_IMAGE_WIDTH;		// ������ I �@��
			new_height	=	bottom - top + 1;
			// ���p�O I �w�g�Q�� �ҥH����
			crop_type	=	LWO_WH_SMALL;
		}
		else if( wh_ratio >= 0.8 )
		{
			// �p�� histogram
			get_histogram( crop_data );

			// �j��Y  ���쥻�P�_ (�A���P�_�O�_�� I)
			LWO_force_pinch( crop_data );

			// ���~�X�i �åB�P�_�O�_�� I
			is_I	=	LWO_add_box( &crop_data->top, &crop_data->bottom, &crop_data->left, &crop_data->right );

			if( is_I == 1 )
				LWO_I_denoise( crop_data );		// �w�� I �h�� �������I

			// ���s�g�J�W�U���k�����
			top			=	crop_data->top;		// �]�����U���������� �ҥH�����b�o��N���g�J���.  (���ӥi�H��code��²��)
			bottom		=	crop_data->bottom;
			left		=	crop_data->left;
			right		=	crop_data->right;
			// ���s�]�w new_width����
			if( is_I == 0 )
			{
				new_width	=	right - left + 1;		// �`�N + 1
				new_height	=	bottom - top + 1;
			}
			else
			{		
				new_width	=	LWO_IMAGE_WIDTH;		// �]�� I ������V�Y��
				new_height	=	bottom - top + 1;
			}

			// �p�G�٬O�����D �N����resize�F 
			wh_ratio	=	1.0 * new_width / new_height;
#ifdef _OCR_USE_FLOAT_
			cx			=	0.5 * ( left + right );		// �ثe�L�P�_ (�Ҽ{�X�R�� �O�dcode �i�H�[mark)
			cy			=	0.5 * ( top + bottom );	
#else
			cx			=	left + right;
			cy			=	top + bottom;
#endif

			crop_type	=	LWO_WH_BIG;

			if( wh_ratio >= 0.8 && is_I == 0 )		// I�٬O�o���ʧ@  
			{
				crop_data->crop_type	=	crop_type;
				return	-1;
			}

		}
#ifdef _OCR_USE_FLOAT_
		else if( cx > 10.5 + LWO_CENTER_OUT || cx < 10.5 - LWO_CENTER_OUT || cy > 15.5 + LWO_CENTER_OUT || cy < 15.5 - LWO_CENTER_OUT )
#else
		else if( cx > 21 + LWO_CENTER_OUT || cx < 21 - LWO_CENTER_OUT || cy > 31 + LWO_CENTER_OUT || cy < 31 - LWO_CENTER_OUT )
#endif
		{
			// �p�� histogram
			get_histogram( crop_data );

			// �j��Y  ���쥻�P�_ (�A���P�_�O�_�� I)
			LWO_force_pinch( crop_data );

			// ���~�X�i �åB�P�_�O�_�� I
			is_I	=	LWO_add_box( &crop_data->top, &crop_data->bottom, &crop_data->left, &crop_data->right );

			if( is_I == 1 )
				LWO_I_denoise( crop_data );		// �w�� I �h�� �������I

			// ���s�g�J�W�U���k�����
			top			=	crop_data->top;
			bottom		=	crop_data->bottom;
			left		=	crop_data->left;
			right		=	crop_data->right;
			// ���s�]�w new_width����
			if( is_I == 0 )
			{
				new_width	=	right - left + 1;		// �`�N + 1
				new_height	=	bottom - top + 1;
			}
			else
			{		
				new_width	=	LWO_IMAGE_WIDTH;		// �]�� I ������V�Y��
				new_height	=	bottom - top + 1;
			}

			// �p�G�٬O�����D �N����resize�F 
			wh_ratio	=	1.0 * new_width / new_height;
			cx			=	0.5 * ( left + right );
			cy			=	0.5 * ( top + bottom );	

			crop_type	=	LWO_CENTER_ERROR;

			if( wh_ratio >= 0.8 && is_I == 0 )		// I�٬O�o���ʧ@
			{
				crop_data->crop_type	=	crop_type;
				return	-1;
			}
		}
		else
		{
			// ���`��
			crop_type	=	LWO_NO_TYPE;		// �S��recrop
		}
	}
	

	crop_data->crop_type	=	crop_type;		// �^�g���


	// �}�l�i��ŵ� �åB�����i��ɭ�
	if( crop_type == LWO_WH_SMALL )
	{
		LWO_place_center_for_wh_small( crop_data, clip_img );		// �L�p���ɭ� ���m�����B�z
	}
	else if( is_I == 0 )
	{
		for( i = 0; i < new_width; i++ )
			for( j = 0; j < new_height; j++ )
			{
				// �d�� ���ݭn�ɭ�
				if( left+i >= 0 && left+i < LWO_IMAGE_WIDTH && top+j >= 0 && top+j < LWO_IMAGE_HEIGHT )
					*( clip_img + j*new_width + i )		=	LWO_IMAGE_XY( img, left+i, top+j );

				else
					*( clip_img + j*new_width + i )		=	get_bg_data();
			}
	}
	else if( is_I == 1 )	// �]�� I �����h���m��
	{
		LWO_place_center_for_I( crop_data, clip_img );
	}
	else
	{
		// error...
	}

	//LWO_scaled_bilinear( img, clip_img, new_width, new_height, LWO_IMAGE_WIDTH, LWO_IMAGA_HEIGHT );		// ��Ϥ����Y��  bilinear����
	LWO_scaled_nearest( img, clip_img, new_width, new_height, LWO_IMAGE_WIDTH, LWO_IMAGE_HEIGHT );			// nearest����

	return	is_I;
}


/******************************************************************************
	�N�Ϥ��m���ŵ� (�D�n�O���F I )
*******************************************************************************/
void	LWO_place_center_for_wh_small( CROP_DATA *crop_data, BYTE *clip_img )
{
	BYTE	*img		=	crop_data->img;				// ��l�Ϥ����
//	short	*bg_data	=	crop_data->bg_data;			// �ɭȸ��

	short	top			=	crop_data->top;
	short	bottom		=	crop_data->bottom;
	short	left		=	crop_data->left;
	short	right		=	crop_data->right;

	short	new_width	=	LWO_IMAGE_WIDTH;			// I ���e����
	short	new_height	=	bottom - top + 1;

	short	center_x2	=	(left + right) / 2;			// ���X���ت�����  �n��o���I��� (center_x, center_y)
	short	center_y2	=	(top + bottom) / 2;

	short	center_x	=	(new_width-1) / 2;			// �ŵ���Ϥ�������
	short	center_y	=	(new_height-1) / 2;

	short	off_x		=	center_x2 - center_x;		// �첾���
	short	off_y		=	center_y2 - center_y;	

	short	i,	j;

	for( i = 0; i < new_width; i++ )
		for( j = 0; j < new_height; j++ )
		{
			if( i+off_x >= 0 && i+off_x < LWO_IMAGE_WIDTH && j+off_y >= 0 && j+off_y < LWO_IMAGE_HEIGHT )
				*( clip_img + j*new_width + i )	=	LWO_IMAGE_XY( img, i+off_x, j+off_y );
			else
				*( clip_img + j*new_width + i )	=	get_bg_data();
		}
}





/******************************************************************************
	�N�Ϥ��m���ŵ� (�D�n�O���F I )
*******************************************************************************/
void	LWO_place_center_for_I( CROP_DATA *crop_data, BYTE *clip_img )
{
	BYTE	*img		=	crop_data->img;				// ��l�Ϥ����
//	short	*bg_data	=	crop_data->bg_data;			// �ɭȸ��

	short	top			=	crop_data->top;
	short	bottom		=	crop_data->bottom;
	short	left		=	crop_data->left;
	short	right		=	crop_data->right;

	short	new_width	=	LWO_IMAGE_WIDTH;			// I ���e����
	short	new_height	=	bottom - top + 1;

	short	center_x2	=	(left + right) / 2;			// ���X���ت�����  �n��o���I��� (center_x, center_y)
	short	center_y2	=	(top + bottom) / 2;

	short	center_x	=	(new_width-1) / 2;			// �ŵ���Ϥ�������
	short	center_y	=	(new_height-1) / 2;

	short	off_x		=	center_x2 - center_x;		// �첾���
	short	off_y		=	center_y2 - center_y;	

	short	i,	j;

	for( i = 0; i < new_width; i++ )
		for( j = 0; j < new_height; j++ )
		{
			if( i+off_x >= 0 && i+off_x < LWO_IMAGE_WIDTH && j+off_y >= 0 && j+off_y < LWO_IMAGE_HEIGHT )
				*( clip_img + j*new_width + i )	=	LWO_IMAGE_XY( img, i+off_x, j+off_y );
			else
				*( clip_img + j*new_width + i )	=	get_bg_data();
		}
}




/******************************************************************************
	�N�Ϥ��� resize nearest����

	�e���� (w,h) -> (nw,nh)
	��l��� = clip_img  ��n����b img

	http://tech-algorithm.com/articles/nearest-neighbor-image-scaling/  �Ѧҳo�䪺�{���X

    int[] temp = new int[w2*h2] ;
    double x_ratio = w1/(double)w2 ;
    double y_ratio = h1/(double)h2 ;
    double px, py ; 
    for (int i=0;i<h2;i++) {
        for (int j=0;j<w2;j++) {
            px = Math.floor(j*x_ratio) ;
            py = Math.floor(i*y_ratio) ;
            temp[(i*w2)+j] = pixels[(int)((py*w1)+px)] ;
        }
    }
    return temp ;

*******************************************************************************/
void		LWO_scaled_nearest( BYTE *img, BYTE *clip_img, short w, short h, short nw, short nh )
{
	short	i,	j;
#ifdef _OCR_USE_FLOAT_
	double	x_ratio	=	(double)w / nw;
	double	y_ratio	=	(double)h / nh;
#endif
	short	px,	py;

    for( i = 0; i < nh; i++ )  
	{
        for( j = 0; j < nw; j++ )	// j = �e  i = ��  �򥭱`�ϥΤ��P
		{
#ifdef _OCR_USE_FLOAT_
            px	=	(int)( j*x_ratio );  
            py	=	(int)( i*y_ratio );  
#else
			px	=	j * w / nw;
			py	=	i * h / nh;
#endif

			*( img + i*nw + j )	=	*( clip_img + py*w + px );
        }
    }
}






