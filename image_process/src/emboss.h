#ifndef _EMBOSS_H
#define _EMBOSS_H
   
//#define EMBOSS_SN	"$Id: 7fc304dc580783af82bcd4281e67a8d22e921c92 $"
   
#ifdef _DEBUG
#pragma comment(lib,"embossd.lib")
#elif NDEBUG
#pragma comment(lib,"emboss.lib")
#else
#error no _DEBUG or NDBUG
#endif

//test    
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	�����m�q�D
	�B�z��m�������\��
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <cmath>
#include <cassert>

#include "shadow.h"
#include "andreaimage.h"

#include <vector>


namespace	emboss{


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// PIXEL����  �o��u��x, y�y��
struct	PIXEL
{
	int		x;
	int		y;

	PIXEL() : x(0), y(0) {}
	PIXEL( int _x, int _y ) : x(_x), y(_y) {}
};

// �t�d�B�z���G (�Hregion�����  �p���s��block)
struct	RREmboss
{
	int		size;			// �O���o��Region���X��block
	int		devia;			// ���v��߰��t
	int		shadow_number;	// ���v��m�����t�ƶq
	int		number;			// ��Ӱ϶��o�~�ϼƶq
	bool	center_out;		// �P�_�O�_�n�Ѧҽ�߰��t

	int		anchor_x;		// region�����W��
	int		anchor_y;
	int		region_w;		// region���e��
	int		region_h;
};

// �P�W ��������
struct	RRScreen
{
	int			size;
	EMBOSS_DIRECTION	direction;	// ��Ӱ϶����`���v��V
	int			ng_count;			// ��Ӱ϶��o�~�ϼƶq
	//int		shadow_count;		// �o�~�϶����v�ƶq

	int			anchor_x;			// region�����W��
	int			anchor_y;
	int			region_w;			// region���e��
	int			region_h;
};


// �W�Y�Ϊ��^�ǵ��G���
struct	EmbossData
{
	// �צV ���V �@�q���
	double				centroid_x;						// ���v����ߦ�m (�@����U�h��)
	double				centroid_y;
	double				centroid_x2;					// �צV�ϥ�
	double				centroid_y2;
	int					shadow_size;					// ���v�Ӽ�
	int					out_shadow;						// ���b���\�Ϫ����v�Ӽ�
	int					up_line_1,		up_line_2;		// �e�˴��ϥ� �W�����u �_�I���I   (���k�@�γo�Ӹ��)   �o��ϥΪ��Opixel�y�Ц�m
	int					down_line_1,	down_line_2;	// �e�˴��ϥ� �U�����u �_�I���I
	bool				is_center_out;					// ��߬O�_���b���\�Ϥ�

	bMatrixByte			allow_area;						// �e�\�� ( = 1  �~�i�H���\���v�X�{     = 0 �����\���v�X�{
	bMatrixByte			allow_area2;					// �e�\��2 ( = 1  �~�i�H���\���v�X�{     = 0 �����\���v�X�{   �צV�ϥ�

	// �B�� = 
	void	operator = ( const EmbossData &em )
	{
		centroid_x		=	em.centroid_x;	
		centroid_y		=	em.centroid_y;
		centroid_x2		=	em.centroid_x2;	
		centroid_y2		=	em.centroid_y2;
		shadow_size		=	em.shadow_size;
		out_shadow		=	em.out_shadow;
		up_line_1		=	em.up_line_1;
		up_line_2		=	em.up_line_2;
		down_line_1		=	em.down_line_1;
		down_line_2		=	em.down_line_2;
		allow_area		=	em.allow_area;
		allow_area2		=	em.allow_area2;
		is_center_out	=	em.is_center_out;
	}
	// �w�]�غc���
	EmbossData()
	{
		centroid_x		=	0;	
		centroid_y		=	0;
		centroid_x2		=	0;
		centroid_y2		=	0;
		shadow_size		=	0;
		out_shadow		=	0;
		up_line_1		=	0;
		up_line_2		=	0;
		down_line_1		=	1;
		down_line_2		=	1;
		is_center_out	=	false;

		allow_area. resize(0,0);
		allow_area2.resize(0,0);
	}
	// ��Jsetting���غc�l
	EmbossData( const EmbossSettings settings )
	{
		shadow_size		=	0;
		out_shadow		=	0;
		up_line_1		=	settings.up_line_1;
		up_line_2		=	settings.up_line_2;
		down_line_1		=	settings.down_line_1;
		down_line_2		=	settings.down_line_2;

		allow_area		=	settings.allow_area;
		allow_area2		=	settings.allow_area2;
	}
	// �ƻs�غc���
	EmbossData( const EmbossData &em )
	{
		*this	=	em;
	}
	// �Ѻc���
	~EmbossData(){}	
};	// end EmbossData


// �����˴��^�ǵ��G���
struct	ScreenData
{
	/*
		���u�I���_�I����I
		�o�䪺���k�O  �W�U �W �U  �������k��    (��V�@�P)
		              ���k �� �k  �����W�U��
					  �צV�٦b��Ҥ�
	*/
	int					area1,	area2;				// �N�˴��Ϥ������ ���O�p��U�۪����v��
	int					th1,	th2;				// �ΨӧP�_��V�� threshold
	EMBOSS_DIRECTION			direct;						// �˴����G (��V)
	int					value;						// �˴����G (��)
	bMatrixByte			area_map1;					// �Ф��u��  ��������϶�  ���� ��mask
	bMatrixByte			area_map2;					// �Ф��u��  ��������϶�  ���� ��mask  �צV�ϥ�

	// �B��l = 
	void 	operator = ( const ScreenData &sc )
	{
		area1			=	sc.area1;
		area2			=	sc.area2;
		th1				=	sc.th1;
		th2				=	sc.th2;
		direct			=	sc.direct;
		value			=	sc.value;
		area_map1		=	sc.area_map1;
		area_map2		=	sc.area_map2;
	}
	// ��setting�ӳ]�w���B��l
	ScreenData( const ScreenSettings settings )
	{
		th1				=	settings.th1;
		th2				=	settings.th2;
		direct			=	EMBOSS_DIRECTION_MAX;
		value			=	0;
		area_map1		=	settings.area_map1;
		area_map2		=	settings.area_map2;
	}
	// �ƻs�غc�l
	ScreenData( const ScreenData &sc )
	{
		*this	=	sc;
	}
	// �w�]�غc���
	ScreenData()
	{
		area1			=	0;
		area2			=	0;
		th1				=	0;
		th2				=	0;
		direct			=	EMBOSS_DIRECTION_MAX;
		value			=	0;
		area_map1.resize(0,0);
		area_map2.resize(0,0);
	}
	// �Ѻc���
	~ScreenData(){}
};	// end ScreenData




// square����   �D�n�Φb���������C�@�ӥ���Ϊ��˴���
struct	SquareData
{
	CHANNEL				channel;						// �M�w�nŪ����channel�����

	int					anchor_x,	anchor_y;			// �w��y�� (�̾ڤ�V�өw)
	int					anchor_x2,	anchor_y2;			// �צV�ݭn��ո��
	int					img_width,	img_height;			// �Ϥ��� �e  �ΨӧP�_�۾� 2k 4k 8k

	// �צV ���V �@�q���
	int					size;							// ������Ƥj�p (�����sizse)
	EMBOSS_DIRECTION	direction;						// ��V
	EMBOSS_DARK_LIGHT	dark_light;						// �G�v�γ��v
	BUMP				bump;							// �Ϯ׬O�W�ΥY
	bool				enhance;						// �泱�v�Φh���v
	int					x,	y;							// ���W�����y��
	int					threshold;						// ���v�`�ת�thr

	// �W�Y�˴������
	EmbossData			em_data;

	// �����˴��ϥΪ����
	ScreenData			sc_data;

	// operator = (�ƻs)
	void	operator = ( const SquareData &sd )
	{
		channel		=	sd.channel;
		size	=	sd.size;
		anchor_x	=	sd.anchor_x;
		anchor_y	=	sd.anchor_y;
		anchor_x2	=	sd.anchor_x2;
		anchor_y2	=	sd.anchor_y2;
		img_width	=	sd.img_width;
		img_height	=	sd.img_height;
		direction	=	sd.direction;
		dark_light	=	sd.dark_light;
		bump		=	sd.bump;
		enhance		=	sd.enhance;
		x			=	sd.x;
		y			=	sd.y;
		threshold	=	sd.threshold;
		em_data		=	sd.em_data;
		sc_data		=	sd.sc_data;

	}

	// �غc���
	SquareData()
	{
		channel		=	CHANNEL_MAX;

		anchor_x	=	0;
		anchor_y	=	0;
		anchor_x2	=	0;
		anchor_y2	=	0;
		img_width	=	0;
		img_height	=	0;

		size		=	0;
		direction	=	EMBOSS_DIRECTION_MAX;
		dark_light	=	EMBOSS_DARK_LIGHT_MAX;
		bump		=	BUMP_MAX;
		enhance		=	false;
		x			=	0;
		y			=	0;
		threshold	=	0;
		em_data		=	EmbossData();
		sc_data		=	ScreenData();
	}
	// �غc��� by settings  (�i�J�e�]�w)
	SquareData( const ShadowSettings &settings )
	{
		channel		=	settings.channel;
		img_width	=	settings.img_width;
		img_height	=	settings.img_height;

		x			=	settings.x;
		y			=	settings.y;
		size		=	settings.size;
		threshold	=	settings.threshold;
		enhance		=	settings.enhance;
		direction	=	settings.direction;
		dark_light	=	settings.dark_light;
		bump		=	settings.bump;

		em_data	=	EmbossData( settings.em_settings );
		sc_data	=	ScreenData( settings.sc_settings );

	}
	// �غc��� (�ƻs)
	SquareData( const SquareData &sd )
	{
		*this		=	sd;
	}
	// �Ѻc
	~SquareData()
	{
		size		=	0;
		x			=	0;
		y			=	0;
	}

};			// end   struct  SquareData






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef		std::vector<SquareData>		EmbossVector;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// KernelDensity ����  �o��ΨӹL�o���T
class	KernelDensity
{
	private:
		bMatrixDouble	kernel;														// smooth �� kernel

	public:
		KernelDensity();															// �غc���

		void			dosomething( bMatrixDouble &p_data, bMatrixByte &data );	// ����
		void			kernel_square( int n );										// ���Ͱ򥻪������ kernel
		int				element_count(bMatrixByte &data);							// �����Ӽƭp��

};





// Emboss����
class	Emboss	
{
	public:
		AndreaImage::AndreaChannel		channel_func;								// channel function,���VAndreaImage image���U��function.   ���VScreen�γo�Ө�Ū�����

		EMBOSS_GOOD_BAD					good_bad;									// �P�_��block�O���~�٬O�o�~
		Shadow							shadow_obj;									// ���v�˴�����

		EMBOSS_DIRECTION				direction;									// ���emboss������w��V
		int								size;										// ���w�j�p
		int								x_left,	x_right,							// �˴��Ϫ��w���I
										y_up,	y_down;		
		int								roi_x,	roi_y;								// �w���I
		int								anchor_x,									// ���I   �|�b set_square_bound �����]�w
										anchor_y,
										anchor_x2,
										anchor_y2;
		PIXEL							start,		end;							// �_���I�y�� (�ݩ�Ȧs�Ϊ�)

		AndreaImage						image;
		SquareData						roi;										// �˴��ϸ�� �w���n�n���
		KernelDensity					kd;											// �ΨӹL�o���T
		double							kd_th;										// kernel density��threshold.  �M�w��˪����p�n�L�o��.

		// ���U�쥻�Ofunction�����ϰ��ܼ� 20130304 �h��class member (���F�[�t)
		ShadowReturnData				*line_data;									// ���ө�bROI���U �@���@�������
		ShadowReturnData				*line_data2;
		bVectorByte						*seq_line;									// ���@��1D�����
		bVectorByte						*seq_line2;									// �צV�ϥ�
		bVectorByte						seq_extend;									// �|���ɭȪ��ʧ@
		const static int				extend_len	=	30;							// ���e����pixel�e��
		int								original_len;								// ��l�}�C����
		ShadowReturnData				shadow_data_tmp;							// ���v���  (�@�ӬO��l�� �@�ӬO�R��������Ʈ��Ӧ^�ǥΪ���)
		ShadowSettings					shadow_settings;							// ���v�]�w �ݩ�߿W���
		bMatrixDouble					bm_tmp;										// kernel density�ϥ�

		// ����V�ϥ�
		bVectorByte						*seq_line_up;								// ����V�ϥΪ�seq_line
		bVectorByte						*seq_line_down;
		bVectorByte						*seq_line_left;
		bVectorByte						*seq_line_right;
		ShadowReturnData				*line_data_up;								// ����V�ϥΪ�line_data
		ShadowReturnData				*line_data_down;
		ShadowReturnData				*line_data_left;
		ShadowReturnData				*line_data_right;
		bMatrixByte						up_map;										// �Ȧs �̫�Ψ�merge
		bMatrixByte						down_map;				
		bMatrixByte						left_map;
		bMatrixByte						right_map;

		// �̫ᵲ�G������ �쥻��b SquareData roi���U.
		bVectorByte						edge;										// �s��edge��m
		bVectorByte						edge2;										// �צV
		bMatrixByte						shadow;										// �̫ᳱ�v�����G ��byte�s
		bMatrixByte						shadow2;									// �צV
		bMatrixByte						deep;										// �`�� (�D��ߨϥ�)
		bMatrixByte						deep2;										// �צV
		bMatrixByte						allow_area;									// �e�\�ϸ�� (����V�S�o�F��)
		bMatrixByte						allow_area2;

	public:
		Emboss();																								// �غc
		virtual					~Emboss();																		// �Ѻc

		//~~~~~~~~~~~~~~~~~ �w���]�w ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		void					set( SquareData _roi );															// �]�w roi ��� (�w�����ͦn�O����Ŷ��קKre-allocate)
		void					generate_allow_area();															// ����mask���
		void					set_image( AndreaImage imagePass );												// �]�w�Ϥ�

		//~~~~~~~~~~~~~~~~~ �妸�ϥ� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~						
		void					batch_shadow_line( int th_id );													// �@���B�z�@���u
		void					batch_shadow_line_2( int th_id );												// �@���B�z�@���u �צV�ϥ�
		void					batch_shadow_ALLD( int th_id );													// �@���B�z�@���u ����V�ϥ�
		void					batch_loadimage();																// �妸����Ū��
		void					batch_linedata2matrix();														// �妸�ഫ���
		void					batch_kernel_density();															// �妸�o�����T
		void					batch_roi_centroid();															// �妸�D���
		void					batch_result_output_emboss_defect( AndreaImage img, const int value );			// �妸�аOdefect map

		//~~~~~~~~~~~~~~~~~ �~������ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~														
		EMBOSS_DIRECTION		get_direction();																// ���o��V
		void					exec();																			// �۷��쥻��execution  multi-thread�ɨϥ�

		//~~~~~~~~~~~~~~~~~ �����Ӹ` ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		void					loadimage( bVectorByte &seq );													// Ū���Ϥ����  (�[�t��)
		bool					is_vertical_horizontal( const EMBOSS_DIRECTION direction );						// �P�_�O�_������������V(�W�U���k) 
		bool					is_single_direction( const EMBOSS_DIRECTION direction );						// �P�_�O�_�� �� ��V
		bool					is_bin_direction( const EMBOSS_DIRECTION direction );							// �P�_�O�_�� �� ��V
		void					set_square_bound();																// �]�w�˴�����ɽd��  �w���I
		void					get_edge();																		// �p����

		//~~~~~~~~~~~~~~~~~ ���G��X ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		void					result_output_emboss           ( AndreaImage img );								// �W�Y���G��X  (��shadow���)
		void					result_output_emboss_allow_area( AndreaImage img );								// �e�X�e�\��
		void					result_output_shadow           ( AndreaImage img );								// �e�X���v
		void					result_output_emboss_centroid  ( AndreaImage img );								// �e�X���
		void					result_output_edge( AndreaImage img );											// �e�Xedge
		void					emboss_result( SquareData &square );											// �p�⳱�v�����G (���μo)
		int						emboss_result_out_area();														// �^�Ǹ��b���\�ϥ~�����v�Ӽ�
		bool					emboss_result_out_center();														// �P�_��߬O�_���b���\�ϥ~ 
		int						all_dir_result();																// ���v�e�\�T�� �P�_
		//~~~~~~~~~~~~~~~~~ ��L ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};


// Screen����  ���� ���V�M��.
class	Screen	
{
	public:
		AndreaImage::AndreaChannel		channel_func;					// channel function,���VAndreaImage image���U��function.   ���VScreen�γo�Ө�Ū�����

		EMBOSS_GOOD_BAD					good_bad;						// �P�_��block�O���~�٬O�o�~
		Shadow							shadow_obj;						// ���v�˴�����

		AndreaImage						image;
		SquareData						roi;							// �˴��ϸ�� 

		EMBOSS_DIRECTION				direction;						// ���emboss������w��V
		EMBOSS_DARK_LIGHT				dark_light;						// �G�t�v
		BUMP							bump;							// ���˥W�ΥY
		int								size;							// ���w�j�p
		int								x_left,	x_right,				// �˴��Ϫ�����I
										y_up,	y_down;		
		int								roi_x,	roi_y;					// �w���I
		int								anchor_x,						// ���I   �|�b set_square_bound �����]�w
										anchor_y,
										anchor_x2,
										anchor_y2;
		PIXEL							start,		end;				// �_���I�y�� (�ݩ�Ȧs�Ϊ�)


		// �쥻���ӬO�Ȧs�ܼ� ���F�[�t �令member value
		const static int				extend_len	=	30;				// ���e����pixel�e��
		int								original_len;					// ��l�}�C����
		ShadowSettings					shadow_settings;				// �����]�w���
		ShadowReturnData				*line_data;						// �X�᪺֫��� (�N�Ӧ^��V�X��)
		ShadowReturnData				*line_data2;					// �צV�ϥ�
		ShadowReturnData				line_front;						// �L�h��V 
		ShadowReturnData				line_back;						// �^�Ӥ�V
		bVectorByte						*seq_line_front;				// �@��1D�����  (�L�h)
		bVectorByte						*seq_line_front2;				// �צV
		bVectorByte						*seq_line_back;					// �^��
		bVectorByte						*seq_line_back2;				// �צV
		bVectorByte						seq_extend;						// �N��l��Ƹɭȫ��J�U�@�h
		ShadowReturnData				shadow_data_tmp;				// �]�t�ɭȸ�� �ѤU�@�h�^�Ǫ����.

		// ����V�ϥ�
		bVectorByte						*seq_line_up;
		bVectorByte						*seq_line_down;
		bVectorByte						*seq_line_left;
		bVectorByte						*seq_line_right;
		ShadowReturnData				*line_data_up;
		ShadowReturnData				*line_data_down;
		ShadowReturnData				*line_data_left;
		ShadowReturnData				*line_data_right;
		bMatrixByte						up_map;		
		bMatrixByte						down_map;
		bMatrixByte						left_map;
		bMatrixByte						right_map;

		// ���G�ϥ�  �쥻��b SquareData roi���U
		bMatrixByte						shadow;							// ���v���
		bMatrixByte						shadow2;						// �צV�ϥ�
		bMatrixByte						area_map1;						// �Ϥ�����Ӱ϶� 
		bMatrixByte						area_map2;		

	public:

		Screen();																								// �غc
		virtual					~Screen();																		// �Ѻc

		//~~~~~~~~~~~~~~~~~ �w���]�w ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		void					set( SquareData _roi );															// �]�w roi ��� (�w�����ͦn�O����Ŷ��קKre-allocate)
		void					generate_center_line_map();														// ���͹w�]��center map (�����ϥ�)
		bool					determine_bump( AndreaImage imagePass );										// �M�w�Ӱ϶������ˬO�W�٬O�Y Page2�|�����ϥ�
		int						determine_screen_value();														// �^�� �����˴� �� thr
		void					set_image( AndreaImage imagePass );												// �]�w�Ϥ�

		//~~~~~~~~~~~~~~~~~ �~������ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		void					batch_shadow_line( int th_id );													// �@���B�z�@���u
		void					batch_shadow_line_2( int th_id );												// �@���B�z�@���u �צV�ϥ�
		void					batch_loadimage();																// �妸����Ū��
		void					batch_linedata2matrix();														// �妸�ഫ���
		EMBOSS_DIRECTION		get_direction();																// ���o��V
		void					exec();																			// �۷��쥻��execution  multi-thread�ɨϥ�  �������V�ϥ�
		void					batch_shadow_ALLD( int th_id );							
		void					batch_result_output_screen_defect( AndreaImage img, const int value );			// �妸�edefect map

		//~~~~~~~~~~~~~~~~~ �����Ӹ` ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		int						all_dir_result();
		bVectorInt				loadimage( PIXEL s, PIXEL e, const EMBOSS_DIRECTION direction );											// Ū���Ϥ����
		void					loadimage( bVectorByte &seq_front, bVectorByte &seq_back );													// Ū���Ϥ���� (���V�[�t��)
		void					loadimage( bVectorByte &seq );
		ShadowReturnData		shadow_line( PIXEL start, PIXEL end, EMBOSS_DIRECTION direct, ShadowSettings shadow_data );					// �@���ˬd�@���u
		bool					is_vertical_horizontal( const EMBOSS_DIRECTION direction );													// �P�_�O�_������������V(�W�U���k)   �Ϊ̱צV(���W���U�k�W�k�U)
		bool					is_single_direction( const EMBOSS_DIRECTION direction );													// �P�_�O�_�� �� ��V
		bool					is_bin_direction( const EMBOSS_DIRECTION direction );														// �P�_�O�_�� �� ��V
		void					set_square_bound();																							// �]�w�˴�����ɽd��  �w���I

		//~~~~~~~~~~~~~~~~~ ���G��X ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		void					result_output_shadow            ( AndreaImage img );														// �e�X���v
		void					result_output_screen		    ( AndreaImage img );														// �����L�X���G
		void					result_output_screen_area		( AndreaImage img );														// �N��Ӱ϶��аO�b�Ϥ��W
		void					screen_result_v_d( SquareData &square );																	// �����˴� ���G  �P�_value & direction  �ثe�S������function
		void					screen_result( SquareData &square );																		// �����˴� ���G  

		//~~~~~~~~~~~~~~~~~ ��L ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		bMatrixByte				screen_center_line_map( PIXEL anchor, int size, EMBOSS_DIRECTION direct, PIXEL start, PIXEL end );			// ���� �N�y�� �ন map
		bMatrixByte				screen_center_line_map2( PIXEL anchor, int size, EMBOSS_DIRECTION direct, PIXEL start, PIXEL end );			// ���� �N�y�� �ন map �צV�ϥ�
};





// �t�d��openMP����multi-thread������
class	Emboss_MT
{
	// note: ���F��K  �N����private�F
	public:
		const static int	MAX_THR		=	7;					// �̤jthread�Ӽ�

		int			size;
		Emboss		*em;

		Emboss_MT();											// �غc
		~Emboss_MT();											// �Ѻc

		void	init( int _size );								// ��l��
		void	destroy();										// �����Ѻc �N��ƧR��.
		void	exec( AndreaImage image );						// ���� (�b�o��i��openMP)
		void	exec_page2( AndreaImage image );
		void	defect( AndreaImage img, int value );			// �аOdefect map, �ϥ�openMP
};



// �t�d��openMP����multi-thread������
class	Screen_MT
{
	// note: ���F��K  �N����private�F
	public:
		const static int	MAX_THR		=	7;					// �̤jthread�Ӽ�

		int			size;
		Screen		*sc;

		Screen_MT();											// �غc
		~Screen_MT();											// �Ѻc

		void	init( int _size );								// ��l��
		void	destroy();										// �����Ѻc �N��ƧR��
		void	exec( AndreaImage image );						// ���� (�b�o��i��openMP)
		void	exec_page2( AndreaImage image );				// page2 �ϥ� 
		void	defect( AndreaImage img, int value );			// �аOdefect map �ϥ�openMP
};






}		// end namespace emboss




#endif