#ifndef _OCR_RECOGNITION_H
#define _OCR_RECOGNITION_H

#include "./OCR_image_process.h"
#include "./OCR_read_data.h"
#include "./OCR_shared.h"
#include "./LWOCR.h"




//#define	_TEMPLATE_PATH	"D:\\TEST\\Template"				// �˪O���|
#define	_TEMPLATE_PATH_CNY_20	"..\\DATA\\CNY20_Template"		// �˪O���| �p�B
#define	_TEMPLATE_PATH_CNY_100	"..\\DATA\\CNY100_Template"		// �˪O���| �p�B 


#define V_Sample_Num	2 //�Y����㰣 �|���䨤�����򥢪����D
#define H_Sample_Num	2


#define BZ_V_Sample_Num 5 //�Y����㰣 �|���䨤�����򥢪����D(2,3,5,9,15)
#define BZ_H_Sample_Num 4 //(2,3,4,7)

#define OCR_2_30		1073741823		// 2^30 - 1    �ΨӳB�z overflow ���D







void	OCR_init( OCR_INIT_TYPE init_type );							// OCR ��l��
void	OCR_init_template(  );					// OCR ��l�Ƽ˪O


int		OCR_exec( BYTE *img, BYTE *bin );				// �D�{�Ƕi�J�I


int		OCR_Recognize( BYTE *Segmental_image_Binary, BYTE *Segmental_image );		// ��r���� ���鳡��



void	OCR_bin_feature( BYTE *Segmental_image_Binary, short *Sample, int *BZ_Feature );		// �p�� binary�Ϫ�feature

void	OCR_feature( BYTE *Segmental_image, short *Sample, short *source_matching, int *Feature, int *BZ_Feature );	// �@�몺�S�x

void	OCR_act_out( int *Feature, int *reorder_label, short *reorder_output );	// �B�zact_out  (�ݭn�B�zoverflow ��j�p)

void	OCR_template( int *reorder_label, short *reorder_output , short *source_matching );	// �˪O��� 



void	OCR_free();									// ����O����


#endif