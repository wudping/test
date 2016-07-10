#ifndef _OCR_SHARED_H
#define _OCR_SHARED_H



//���Τj�p
#define	Sub_width		22
#define Sub_height		32

//#define	_OCR_USE_FLOAT_					// �O�_�ϥ�float���w�q


typedef enum	OCR_INIT_TYPE_DEFINE
{
	OCR_CNY_20		=	0,				// �p�B
	OCR_CNY_100		=	1,			// �j�B

} OCR_INIT_TYPE ;


typedef		unsigned char	BYTE;


short	template_original[ 37 * Sub_width * Sub_height ];



short	nr_class;												// class �Ӽ�
short	nr_feature;												// feature �Ӽ�

#ifdef _OCR_USE_FLOAT_
float	*Omega;													// ��� �����binit���ɭԳQ��l��
#else
short	*Omega;
#endif

short	*O_label;												// ��Omega�@�ժ� �쥻���W label (�令global,�ҥH�W�r�]�ﱼ)

int		Scale_Table[10001];										// STORE (1/MAX-MIN) TABLE

#ifdef _OCR_USE_FLOAT_
float		Shift_Template_Value[ 37 * 4 * 12 * 16 * 16 ];
float	 	Shift_Template_ASST [ 37 * 4 * 12 * 2 ];
#else
int		Shift_Template_Value[ 37 * 4 * 12 * 16 * 16 ];
int	 	Shift_Template_ASST [ 37 * 4 * 12 * 2 ];
#endif

void	Pre_Load_Shift_Template(int _index, int _block, int _v_shift, int _h_shift);

#endif