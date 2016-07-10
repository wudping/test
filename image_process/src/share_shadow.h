#ifndef _SHARE_SHADOW_H
#define _SHARE_SHADOW_H


/*=========================================================================================================
	�NAOI�{���|�Ψ쪺enum, define��b�o��.
/*=========================================================================================================*/

#define		EMBOSS_CENTER_WEIGHT	20000			// ��߰��� AOI�e���W�|�X�{���v����  �w20000�O�]��window�̤j85*85 �Ҽ{�צV *2


// �䴩�B�I�ƪ�point
struct	FPOINT
{
	double	x;
	double	y;
};



// ��V
enum	EMBOSS_DIRECTION
{
	EMBOSS_UP				=	0,
	EMBOSS_DOWN				=	1,
	EMBOSS_LEFT				=	2,
	EMBOSS_RIGHT			=	3,
	EMBOSS_LEFT_UP			=	4,
	EMBOSS_LEFT_DOWN		=	5,
	EMBOSS_RIGHT_UP			=	6,
	EMBOSS_RIGHT_DOWN		=	7,
	EMBOSS_ALL_DIRECTION	=	8,			// ����V
	EMBOSS_UP_DOWN			=	9,			// �P�ɰ��W�U ���涰
	EMBOSS_LEFT_RIGHT		=	10,			// �P�ɰ����k ���涰
	EMBOSS_LU_RD			=	11,			// ���W �� �k�U
	EMBOSS_LD_RU			=	12,			// ���U �� �k�W
	EMBOSS_MIDDLE			=	13,			// ����
	EMBOSS_DIRECTION_MAX	=	14
};

// ���v�ΫG�v
enum	EMBOSS_DARK_LIGHT
{
	EMBOSS_DARK				=	0,
	EMBOSS_LIGHT			=	1,
	EMBOSS_DARK_LIGHT_MAX	=	2
};

// ��block�O���~�٬O�o�~
enum	EMBOSS_GOOD_BAD
{
	EMBOSS_BAD			=	0,		// �o�~
	EMBOSS_GOOD			=	1,		// ��block�O���~
};


#endif