#ifndef _LWOCR_H
#define _LWOCR_H

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "OCR_shared.h"


/************************************************************************************************************************************

	note: �Ϥ��� array �Ӧs �o�䰲�]�O�Ƕ��� ���e�ᰪ���s��覡
		  _OCR_USE_FLOAT_ �w�q�b OCR_shared.h  �ΨӨM�w�O�_�ϥ�float

************************************************************************************************************************************/


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  define  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#define		LWO_BIN_VALUE		1															// �G�Ȥƫ� �D�s����  (���Fshow�Ϥ�K)
#define		LWO_IMAGE_WIDTH		22															// �Ϥ��e
#define		LWO_IMAGE_HEIGHT	32															// �Ϥ���
#define		LWO_IMAGE_SIZE		( LWO_IMAGE_WIDTH * LWO_IMAGE_HEIGHT )						// pixel �Ӽ� 704 = 22 * 32

#define		LWO_IMAGE_PIXEL( PT, INDEX )	(*( (PT) + (INDEX) ))							// Ū��pixel��m
#define		LWO_IMAGE_XY( PT, X, Y )		(*( (PT) + ((Y)*(LWO_IMAGE_WIDTH)) + (X) ))		// Ū�� (X,Y) ��m   �A���O���F�קK�X��  (���p�� X = left+i�������i��)

#define		LWO_IS_WORD			2															// �j��o�� THR �N�{���O���r
#define		ADD_VALUE_SIZE		1															// ���~�����o�ӶZ��  (�ʤ֪������ɭ�)
#define		LWO_IS_I			5															// �O�_�� I �� thr

#ifdef _OCR_USE_FLOAT_
	#define		LWO_CENTER_OUT		3														// �������������\�d��
#else
	#define		LWO_CENTER_OUT		6														// �]���S����2 �ҥH�ܦ� 6
#endif





/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  enum  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef enum	LWO_CROP_TYPE_DEFINE
{
	LWO_WH_SMALL		=	0,		// w/h <= 0.4
	LWO_WH_BIG			=	1,		// w/h >= 0.8
	LWO_CENTER_ERROR	=	2,		// ���߰���
	LWO_NO_TYPE			=	3,		// �L�w�q

} LWO_CROP_TYPE ;



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  struct  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// crop �Ϥ��һݪ��Ѽ�
typedef struct	CROP_DATA_DEFINE
{
	BYTE				*img;									// ��l�Ϥ�
	BYTE				*bin;									// �G�ȤƹϤ�
	short				top;									// �ت��W�U���k
	short				bottom;
	short				left;
	short				right;
	//short				*bg_data;								// �ɭȸ��
	short				id;
	short				histogram_horizon[LWO_IMAGE_WIDTH];		// ������ histogram
	short				histogram_vertical[LWO_IMAGE_HEIGHT];	// ������ histotram
	LWO_CROP_TYPE		crop_type;								// ����crop�o�ͪ����D
} CROP_DATA ;



/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  function  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
short		LWO_max( BYTE *img );																		// �^�ǹϤ����̤j��
short		LWO_min( BYTE *img );																		// �^�ǹϤ������p��

void		LWO_binary( BYTE *img, BYTE *bin );															// ��Ϥ���binarized

short		LWO_crop( CROP_DATA *crop_data, OCR_INIT_TYPE init_type );									// ��Ϥ���crop �ѼƲӸ`�� struct �w�q   �^�ǭȥi�H�P�_�O�_�� I
void		LWO_crop_big_boxing( CROP_DATA *crop_data );												// �j�M�X�@�Ӥj���� �N�r�ئ� (�snoise�@�_�ئ�)
void		LWO_crop_shrink_boxing( CROP_DATA *crop_data );												// �N�ة����Y (�I��Y�@�泣�S���I �N�����Y)

void		LWO_scaled_bilinear( BYTE *img, BYTE *clip_img, short w, short h, short nw, short nh );		// �N�Ϥ��� resize �ʧ@ �e�� �� (w,h) -> (nw, nh)  �o��Obilinear������
void		LWO_scaled_nearest( BYTE *img, BYTE *clip_img, short w, short h, short nw, short nh );		// �N�Ϥ��� resize nearest����
void		LWO_place_center_for_I( CROP_DATA *crop_data, BYTE *clip_img );								// �N�Ϥ��m���ŵ� �� I �Ϊ���
void		LWO_place_center_for_wh_small( CROP_DATA *crop_data, BYTE *clip_img );						// �N�Ϥ��m���ŵ� �� wh <= 0.4 ����
void		LWO_force_pinch( CROP_DATA *crop_data );													// �j��Y

short		LWO_add_box( short *_top, short *_bottom, short *_left, short *_right );					// ���~�ɤ@�h�� (�վ���� �S�u���ɭ� �^�ǧP�_�O�_�� I )
void		LWO_I_denoise( CROP_DATA *crop_data );														// �w�� I  crop�����D��  recrop�������I

#ifdef _OCR_USE_FLOAT_
static void		set_bg_data( double value );															// �]�w�ɭȸ��
static double	get_bg_data();																			// ���o�ɭȸ��
#else
static void		set_bg_data( short value );																// �]�w�ɭȸ��
static short	get_bg_data();																			// ���o�ɭȸ��
#endif

static void		get_histogram( CROP_DATA *crop_data );													// �p�� histogram (��binary data)


void		LWO_bin_rev( BYTE *bin );																	// �N��Ƥ���
void		LWO_wh_small( CROP_DATA *crop_data );														// �B�z wh <= 0.4



#endif