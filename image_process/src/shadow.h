/*--------------------------------------------------------------------------------------------------------------------------------------------------------
	shadow.h
	���h�t��k����
/*--------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef _SHADOW_H
#define _SHADOW_H

#include "share_shadow.h"
#include "andreamatrix.hpp"
#include "andreavector.hpp"
#include <cstring>


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    typedef   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef	andrea::matrix<double>			bMatrixDouble;
typedef andrea::vector<double>			bVectorDouble;
typedef	andrea::matrix<int>				bMatrixInt;
typedef andrea::vector<int>				bVectorInt;
typedef andrea::vector<unsigned char>	bVectorByte;
typedef andrea::matrix<unsigned char>	bMatrixByte;

typedef bool (*compareInt)(int, int );												// function
typedef bool (*compareByte)( unsigned char, unsigned char );						// function

// namespace emboss
namespace	emboss{



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    define & global value   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define		SHADOW_VALUE		1		// ���v��
#define		UN_SHADOW_VALUE		0		// �D���v��



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    enum   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// �W�ΥY
enum	BUMP
{
	CONCAVE		=	0,			// �W
	PROTRUDING,					// �Y
	BUMP_MAX
};

// channel type
enum	CHANNEL
{
	RED		=	0,		// ����
	GREEN,
	BLUE,
	GRAY,
	CHANNEL_MAX
};




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    struct   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// �W�Y�˴��M�Ϊ����
struct	EmbossSettings
{	
	// �]�i�H�Q�ε��y�Ъ��覡�ӵe���u  (�̷ӻݨD�Ӱ��վ�)
	int				up_line_1;			// �W�Y�˴� �}���v�X�{�϶� �W���u �_�I     (��ڤW���k�]�O�ΤW�U�Ӧs  �N���t�~���W�s�����k�F)
	int				up_line_2;			//                         �W���u ���I
	int				down_line_1;		//                         �U���u �_�I
	int				down_line_2;		//                         �U���u ���I

	bMatrixByte		allow_area;
	bMatrixByte		allow_area2;

	// �w�]�غc
	EmbossSettings()
	{
		up_line_1		=	0;
		up_line_2		=	0;
		down_line_1		=	0;
		down_line_2		=	0;

		allow_area. resize(0,0);
		allow_area2.resize(0,0);
	}
	// �ƻs�غc
	EmbossSettings( const EmbossSettings &em_setting )
	{
		*this	=	em_setting;
	}
	// operator = 
	void	operator = ( const EmbossSettings &em_setting )
	{
		up_line_1		=	em_setting.up_line_1;
		up_line_2		=	em_setting.up_line_2;
		down_line_1		=	em_setting.down_line_1;
		down_line_2		=	em_setting.down_line_2;
		allow_area		=	em_setting.allow_area;
		allow_area2		=	em_setting.allow_area2;
	}
	// �Ѻc
	~EmbossSettings()
	{}

};

// �����˴��M�Ϊ����
struct	ScreenSettings
{
	// �����˴���
	int				th1;				// ����϶������threshold (�ά۴�h��threshold���)
	int				th2;	
	bMatrixByte		area_map1;			// ������ �N�϶��������
	bMatrixByte		area_map2;			// �צV��

	ScreenSettings()
	{
		th1				=	0;			
		th2				=	0;	
		area_map1.resize(0,0);		
		area_map2.resize(0,0);			
	}
	ScreenSettings( const ScreenSettings& ss )
	{
		*this	=	ss;
	}
	void	operator = ( const ScreenSettings& ss )
	{
		th1				=	ss.th1;
		th2				=	ss.th2;
		area_map1		=	ss.area_map1;
		area_map2		=	ss.area_map2;
	}

	~ScreenSettings() {}
};


// ���v�]�w���
struct	ShadowSettings
{	
	// �@�q�]�w
	int					x,		y;			// ���W���y��
	int					img_width;			// �Ϥ��j�p �ΨӧP�_�O2k 4k 8k
	int					img_height;			
	int					size;				// ����Τj�p
	int					threshold;			// ���v�`�ת�thr
	bool				enhance;			// true: ��泱�v	
	EMBOSS_DIRECTION	direction;			// ��V
	EMBOSS_DARK_LIGHT	dark_light;			// �M�w�G�ηt�v	
	BUMP				bump;				// �W�ΥY
	CHANNEL				channel;			// �Ϥ��nŪ����channel  (���p�H��emboss�]�����|�W�[)

	// ���v�˴��]�w
	EmbossSettings	em_settings;

	// �����˴���
	ScreenSettings	sc_settings;

};


// ���v�^�Ǹ��
class	ShadowReturnData
{
public:
    bVectorByte		mono_sub_seq;			// ��դl�ǦC
	bVectorByte		shadow_seq;				// ���v��m
	bVectorByte		shadow_depth;			// �`�׸�T  (��l��� - ���v)	

	// �w�]�غc
	ShadowReturnData()
	{
		mono_sub_seq. resize(0);
		shadow_seq.   resize(0);
		shadow_depth. resize(0);
	}
	// ���w�j�p�غc
	ShadowReturnData( int n )
	{
		mono_sub_seq. resize(n);
		shadow_seq.   resize(n);
		shadow_depth. resize(n);
	}
	// �Ѻc
	~ShadowReturnData()
	{
	}
	// �ƻs�غc
	ShadowReturnData( const ShadowReturnData &srd )
	{
		*this	=	srd;
	}
	// �B�� =
	void	operator = ( const ShadowReturnData &srd )
	{
		//raw_data		=	srd.raw_data;
		mono_sub_seq	=	srd.mono_sub_seq;
		shadow_seq		=	srd.shadow_seq;
		shadow_depth	=	srd.shadow_depth;
	}


	void	init( int n )
	{
		mono_sub_seq. resize(n);
		shadow_seq.   resize(n);
		shadow_depth. resize(n);
	}
};








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Shadow  class   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class	Shadow
{
	// note: ���F��K�g��public
	public:
		int		*sub_inx;					// �쥻�O�Ȧs�ΰO���� ���F�קKmemory allocate �令member value
		int		*link_inx;

		int		size;						// �qsetting�Ӫ����
		int		threshold;					// ���v�`��
		bool	enhance;					// �M�w�n����I �٬O��q�����v
		BUMP	bump;						// �M�w���ˬO�W�ΥY

		EMBOSS_DARK_LIGHT	dark_light;		// �M�w�n��G�γ��v

		Shadow();		// �غc
		~Shadow();		// �Ѻc

		void			set( int _size, const  ShadowSettings &setting );						// �]�w

		// �M���դl�ƦC	�[�t��
		void			sub_mono_seq( bVectorByte &seq, bVectorByte &is_choose );				// �ΦbEmboss  (�٦���������V)

		void			sub_mono_seq_less( bVectorByte &seq, bVectorByte &is_choose );			// ��ڤW�O�p�󵥩� (���W)  �ΦbScreen
		void			sub_mono_seq_greater( bVectorByte &seq, bVectorByte &is_choose );		// ��ڤW�O�j�󵥩�	(����)	

		// ���v����
		void			shadow_detector( bVectorByte &seq, ShadowReturnData &return_data );

		// �����˴�
		void			screen_detector( bVectorByte &seq, ShadowReturnData &return_data, int th_id );

		// �D�X���Asub seq���϶�
		void			find_non_sub_seq_interval( int &i1, int &i2, bVectorByte &is_choose );


};




} // end namespace emboss


#endif