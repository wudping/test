#include "emboss.h"


namespace emboss{



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Screen  class  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/***************************************************************************************
	�غc
/****************************************************************************************/
Screen::Screen()
{
	channel_func	=	NULL;
	good_bad		=	EMBOSS_GOOD;
	// shadow_obj
	// image
	// roi
	direction		=	EMBOSS_DIRECTION_MAX;
	dark_light		=	EMBOSS_DARK_LIGHT_MAX;
	bump			=	BUMP_MAX;
	size			=	-1;

	x_left			=	-1;
	x_right			=	-1;
	y_up			=	-1;
	y_down			=	-1;
	roi_x			=	-1;
	roi_y			=	-1;
	anchor_x		=	-1;
	anchor_y		=	-1;
	anchor_x2		=	-1;
	anchor_y2		=	-1;

	start			=	PIXEL(-1,-1);
	end				=	PIXEL(-1,-1);

	original_len	=	-1;
	// shadow_settings
	line_data		=	NULL;
	line_data2		=	NULL;
	//line_fron;
	//line_back;
	seq_line_front	=	NULL;
	seq_line_front2	=	NULL;
	seq_line_back	=	NULL;
	seq_line_back2	=	NULL;

	seq_extend.resize(0);
	//shadow_data_tmp

	seq_line_up		=	NULL;
	seq_line_down	=	NULL;
	seq_line_left	=	NULL;
	seq_line_right	=	NULL;

	line_data_up	=	NULL;
	line_data_down	=	NULL;
	line_data_left	=	NULL;
	line_data_right	=	NULL;

	//up_map.resize(0,0);
	//down_map.resize(0,0);
	//left_map.resize(0,0);
	//right_map.resize(0,0);

	//shadow;	
	//shadow2;

}


/***************************************************************************************
	�Ѻc
/****************************************************************************************/
Screen::~Screen()
{
	channel_func	=	NULL;
	good_bad		=	EMBOSS_GOOD;

	delete	[] line_data;				line_data		=	NULL;
	delete	[] line_data2;				line_data2		=	NULL;
	delete	[] seq_line_front;			seq_line_front	=	NULL;
	delete	[] seq_line_front2;			seq_line_front2	=	NULL;
	delete	[] seq_line_back;			seq_line_back	=	NULL;
	delete	[] seq_line_back2;			seq_line_back2	=	NULL;

	delete	[] seq_line_up;				seq_line_up		=	NULL;
	delete	[] seq_line_down;			seq_line_down	=	NULL;
	delete	[] seq_line_left;			seq_line_left	=	NULL;
	delete	[] seq_line_right;			seq_line_right	=	NULL;

	delete	[] line_data_up;			line_data_up	=	NULL;
	delete	[] line_data_down;			line_data_down	=	NULL;
	delete	[] line_data_left;			line_data_left	=	NULL;
	delete	[] line_data_right;			line_data_right	=	NULL;
}





/***************************************************************************************
	���v�e�\�T�� 
	�P�_���X�ӳ��v 
	�W�L�ƶq�N�s���o�~ (�Y�檺�� �X�{�@�ӴN�O�o�~)
/****************************************************************************************/
int		Screen::all_dir_result()
{
	int		i,	j;
	int		count;

	assert( direction == EMBOSS_ALL_DIRECTION );
	assert( shadow.size1() == size && shadow.size2() == size );

	count	=	0;
	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
		{
			if( shadow( i, j ) == SHADOW_VALUE )
				count++;
		}
	
	return	count;
}


/***************************************************************************************
	���v�˴��Ϊ�
	Ū�����ɸ��
	�[�t�� (�קK�O���魫�ƫŧi)
	�������] seq ���e�N�w�g����resize�ʧ@
/****************************************************************************************/
void	Screen::loadimage( bVectorByte &seq )
{
	assert( seq.size() == size );

	int			i;

	// �ˬd�I�O�_���T  �ˬd�۹��m
	switch(direction)
	{
		case EMBOSS_UP:
			assert(start.x == end.x && start.y < end.y);
			break;
		case EMBOSS_DOWN:
			assert(start.x == end.x && start.y > end.y);
			break;
		case EMBOSS_LEFT:
			assert(start.x < end.x && start.y == end.y);
			break;
		case EMBOSS_RIGHT:
			assert(start.x > end.x && start.y == end.y);
			break;
		case EMBOSS_LEFT_UP:
			assert(end.x-start.x == end.y-start.y);
			break;
		case EMBOSS_LEFT_DOWN:
			assert(end.x-start.x == start.y-end.y);
			break;
		case EMBOSS_RIGHT_UP:
			assert(start.x-end.x == end.y-start.y);
			break;
		case EMBOSS_RIGHT_DOWN:
			assert(start.x-end.x == start.y-end.y);
			break;
		default:
			assert(false);
	}

#ifdef _DEBUG
	// ���M�wsize  (�]���Ҽ{����� �o��|���Ҥ��P �w���Ҽ{�i�h) 
	// �ˬdsize  debug�Ҧ��U�~�ϥ�
	int		tmp_size;
	switch(direction)
	{
		case EMBOSS_UP :
			tmp_size	=	end.y - start.y + 1;
			break;
		case EMBOSS_DOWN :
			tmp_size	=	start.y - end.y + 1;
			break;
		case EMBOSS_LEFT :
			tmp_size	=	end.x - start.x + 1;
			break;
		case EMBOSS_RIGHT :
			tmp_size	=	start.x - end.x + 1;
			break;
		case EMBOSS_LEFT_UP :
			tmp_size	=	end.x - start.x + 1;
			break;
		case EMBOSS_LEFT_DOWN :
			tmp_size	=	end.x - start.x + 1;
			break;
		case EMBOSS_RIGHT_UP :
			tmp_size	=	start.x - end.x + 1;
			break;
		case EMBOSS_RIGHT_DOWN :
			tmp_size	=	start.x - end.x + 1;
			break;
		case EMBOSS_ALL_DIRECTION:
			tmp_size	=	size;	// �Ȯɥ��o��
			break;
		default:
			assert(false);
	}
	assert( seq.size() == tmp_size );		// �ˬdseq��size
	assert( size == tmp_size );				// �ˬd��setting��size
#endif

	assert( channel_func != NULL );

	// �}�lŪ���Ϥ����
	switch( direction )
	{
		case EMBOSS_UP :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( start.x, start.y + i );
			break;

		case EMBOSS_DOWN :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( start.x, start.y - i );
			break;

		case EMBOSS_LEFT:
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( start.x + i, start.y );
			break;

		case EMBOSS_RIGHT :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( start.x - i, start.y );
			break;

		case EMBOSS_LEFT_UP :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( start.x + i, start.y + i );
			break;

		case EMBOSS_LEFT_DOWN :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( start.x + i, start.y - i );
			break;

		case EMBOSS_RIGHT_UP :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( start.x - i, start.y + i );
			break;

		case EMBOSS_RIGHT_DOWN :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( start.x - i, start.y - i );
			break;

		default :
			assert(false);
	}
}


/***************************************************************************************
	���v�˴��Ϊ�
	�@���ˬd�@���u
	����V�ϥ�
/****************************************************************************************/
void	Screen::batch_shadow_ALLD( int th_id )
{
	// EMBOSS_UP~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ShadowReturnData	&shadow_data	=	line_data_up[th_id];

	seq_extend.set_value( 0, extend_len, seq_line_up[th_id](0) );		// ���ͩ��e�������
	seq_extend.add_vector( extend_len, seq_line_up[th_id] );			// ���^�쥻�����

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();

	// EMBOSS_DOWN~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ShadowReturnData	&shadow_data2	=	line_data_down[th_id];

	seq_extend.set_value( 0, extend_len, seq_line_down[th_id](0) );		// ���ͩ��e�������
	seq_extend.add_vector( extend_len, seq_line_down[th_id] );			// ���^�쥻�����

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data2.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data2.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();

	// EMBOSS_LEFT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ShadowReturnData	&shadow_data3	=	line_data_left[th_id];

	// ���ͩ��e�������
	seq_extend.set_value( 0, extend_len, seq_line_left[th_id](0) );		// ���e�ɭ�
	seq_extend.add_vector( extend_len, seq_line_left[th_id] );			// ���^�쥻�����

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data3.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data3.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();

	// EMBOSS_RIGHT~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ShadowReturnData	&shadow_data4	=	line_data_right[th_id];

	// ���ͩ��e�������
	seq_extend.set_value( 0, extend_len, seq_line_right[th_id](0) );		// ���e�ɭ�
	seq_extend.add_vector( extend_len, seq_line_right[th_id] );				// ���^�쥻�����

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data4.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data4.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();
}







/***************************************************************************************
	�妸����Ū��
/****************************************************************************************/
void	Screen::batch_loadimage()
{
	int					i;
	int					x,		y,		x2,		y2;

	assert( direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT || direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU || direction == EMBOSS_ALL_DIRECTION );
#ifdef _DEBUG
	for( i = 0; i < size; i++ )
		assert( seq_line_front[i].size() == seq_line_back[i].size() && seq_line_front[i].size() == size );
#endif


	// �}�l���˴��Ϩ���ư��P�_
	switch( direction )
	{
		case EMBOSS_UP_DOWN:
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x_left + i;
				end.x	=	x_left + i;
				start.y	=	y_up;
				end.y	=	y_down;
				loadimage( seq_line_front[i], seq_line_back[i] );
			}
			break;

		case EMBOSS_LEFT_RIGHT :
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x_left;
				end.x	=	x_right;
				start.y	=	y_up + i;
				end.y	=	y_up + i;
				loadimage( seq_line_front[i], seq_line_back[i] );
			}
			break;

	
		case EMBOSS_LU_RD:
			x	=	x_left;
			y	=	(y_up + y_down)/2;
			x2	=	x;
			y2	=	y + 1;
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x + i;							
				end.x	=	x + i + size - 1;				
				start.y	=	y - i;							
				end.y	=	y - i + size - 1;				
				loadimage( seq_line_front[i], seq_line_back[i] );

				start.x	=	x2 + i;
				end.x	=	x2 + i + size - 1;
				start.y	=	y2 - i;
				end.y	=	y2 - i + size - 1;
				loadimage( seq_line_front2[i], seq_line_back2[i] );
			}
			break;

		case EMBOSS_LD_RU:
			x	=	x_left;
			y	=	(y_up + y_down)/2;
			x2	=	x;
			y2	=	y + 1;
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x + i;
				end.x	=	x + i + size - 1;
				start.y	=	y + i ;
				end.y	=	y + i - size + 1;
				loadimage( seq_line_front[i], seq_line_back[i] );

				start.x	=	x2 + i;
				end.x	=	x2 + i + size - 1;
				start.y	=	y2 + i ;
				end.y	=	y2 + i - size + 1;
				loadimage( seq_line_front2[i], seq_line_back2[i] );
			}
			break;

		case EMBOSS_ALL_DIRECTION:
			for(  i = 0; i < size; i++ )
			{
				direction	=	EMBOSS_UP_DOWN;
				start.x		=	x_left + i;
				end.x		=	x_left + i;
				start.y		=	y_up;
				end.y		=	y_down;
				loadimage( seq_line_up[i], seq_line_down[i] );

				direction	=	EMBOSS_LEFT_RIGHT;
				start.x		=	x_left;
				end.x		=	x_right;
				start.y		=	y_up + i;
				end.y		=	y_up + i;
				loadimage( seq_line_left[i], seq_line_right[i] );

				direction	=	EMBOSS_ALL_DIRECTION;	// �]���i�H�bloadimage���ˬd �ҥH�C�����n��direction���]�@��
			}
			assert( direction == EMBOSS_ALL_DIRECTION );
			break;

		default:
			assert(false);
	}
}





/***************************************************************************************
	�^�� �����˴� �� thr
	�]�w�˱i���ɭԨϥ�
	�����bexcution��~��I�s��function
	���M�|�S�����v���
/****************************************************************************************/
int		Screen::determine_screen_value()
{
	assert( direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT || direction == EMBOSS_LU_RD || 
		    direction == EMBOSS_LD_RU   || direction == EMBOSS_LU_RD      || direction == EMBOSS_LD_RU );

	assert( shadow.size1() == shadow.size2() && area_map1.size1() == area_map1.size2() && shadow.size1() == area_map1.size1() );
	assert( shadow2.size1() == shadow2.size2() && area_map2.size1() == area_map2.size2() && shadow2.size1() == area_map2.size1() );
	assert( shadow.size1() == shadow2.size1() && area_map1.size1() == area_map2.size1() );

	int		i,	j;
	int		area1;		
	int		area2;
	
	/* 
		���u�I���_�I����I
		�o�䪺���k�O  �W�U �W �U  �������k��    (��V�@�P)   area1 = �W  area2 = �U
		              ���k �� �k  �����W�U��				 area1 = ��  area2 = �U
					  �צV�٦b��Ҥ�
	*/
	area1	=	0;		
	area2	=	0;

	// �}�l�p�⳱�v����
	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
		{
			if( shadow( i, j ) == SHADOW_VALUE )
			{
				if( direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT || direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU )
				{
					if( area_map1(i,j) == 1 )	area1++;
					else						area2++;		// �]���S�����߰� �ҥH�D�@�ΤG
				}
				else
					assert(false);
			}
			if( direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU )
			{
				if( shadow2( i, j ) == SHADOW_VALUE )
				{
					if( area_map2(i,j) == 1 )	area1++;
					else						area2++;		// �]���S�����߰� �ҥH�D�@�ΤG
				}
			}
		}

	return	abs( area1 - area2 );		// �^�ǭȪ������Ӱ϶����Ȭ۴�
}




/***************************************************************************************
	�����˴� ���G

	�Ъ`�N ���v�e�\�T�� �o��ݭn�h�[�P�_
	(�]������P�_���o�覡���P �u�n�o�˼g)
/****************************************************************************************/
void	Screen::screen_result( SquareData &square )
{
	// ����V
	if( is_bin_direction(direction) )
		screen_result_v_d( square );

	else if( direction == EMBOSS_ALL_DIRECTION )
	{	
		square.sc_data.value	=	all_dir_result();
		square.sc_data.direct	=	EMBOSS_DIRECTION_MAX;		// �令�έȨӧP�_ ��V�û����@�ӷ|������ (�f�tAOI�{���g�k)
	}
	else
		assert(false);
}



/***************************************************************************************
	�����˴� ���G
	�p��value & direction 
	(�ثe�S������function)
/****************************************************************************************/
void	Screen::screen_result_v_d( SquareData &square )
{
	const int		THR1			=	roi.sc_data.th1;
	const int		THR2			=	roi.sc_data.th2;

	assert( shadow.size1() == shadow.size2() && shadow2.size1() == shadow2.size2() && shadow.size1() == shadow2.size1() );
	assert( area_map1.size1() == area_map1.size2() && area_map2.size1() == area_map2.size2() && area_map1.size1() == area_map2.size1() );
	assert( shadow.size1() == size );

	int			i,	j;
	
	/* 
		���u�I���_�I����I
		�o�䪺���k�O  �W�U �W �U  �������k��    (��V�@�P)   area1 = �W  area2 = �U
		              ���k �� �k  �����W�U��				 area1 = ��  area2 = �U
					  �צV�٦b��Ҥ�
	*/
	int		area1	=	0;		
	int		area2	=	0;

	// �N���v���G�L�X

	// �}�l�p�⳱�v����
	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
		{
			if( shadow( i, j ) == SHADOW_VALUE )
			{
				if( direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT || direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU )
				{
					if( area_map1(i,j) == 1 )			area1++;
					else if( area_map1(i,j) == 2 )		area2++;		// �]���S�����߰� �ҥH�D�@�ΤG
					else								assert(false);
				}
				else
					assert(false);
			}
			if( direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU )
			{
				if( shadow2( i, j ) == SHADOW_VALUE )
				{
					if( area_map2(i,j) == 1 )			area1++;
					else if( area_map2(i,j) == 2 )		area2++;		// �]���S�����߰� �ҥH�D�@�ΤG
					else								assert(false);
				}
			}
		}

	// �^�g���
	square.sc_data.area1	=	area1;
	square.sc_data.area2	=	area2;

	// ���͵��G ��
	if( area1 - area2 >= THR1 )
		square.sc_data.value	=	area1;

	else if( area2 - area1 >= THR2 )
		square.sc_data.value	=	area2;

	else
		square.sc_data.value	=	(area1 + area2)/2;


	// ���͵��G  ��V 
	switch(direction)
	{
		case EMBOSS_UP:
		case EMBOSS_DOWN:
		case EMBOSS_UP_DOWN:
			if( area1 - area2 > THR1 )
				square.sc_data.direct	=	EMBOSS_UP;
			else if( area2 - area1 > THR2 )
				square.sc_data.direct	=	EMBOSS_DOWN;
			else
				square.sc_data.direct	=	EMBOSS_MIDDLE;
			break;
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
		case EMBOSS_LEFT_RIGHT:
			if( area1 - area2 > THR1 )
				square.sc_data.direct	=	EMBOSS_LEFT;
			else if( area2 - area1 > THR2 )
				square.sc_data.direct	=	EMBOSS_RIGHT;
			else
				square.sc_data.direct	=	EMBOSS_MIDDLE;
			break;
		case EMBOSS_LU_RD:
			if( area1 - area2 > THR1 )
				square.sc_data.direct	=	EMBOSS_LEFT_UP;
			else if( area2 - area1 > THR2 )
				square.sc_data.direct	=	EMBOSS_RIGHT_DOWN;
			else
				square.sc_data.direct	=	EMBOSS_MIDDLE;
			break;
		case EMBOSS_LD_RU:
			if( area1 - area2 > THR1 )
				square.sc_data.direct	=	EMBOSS_LEFT_DOWN;
			else if( area2 - area1 > THR2 )
				square.sc_data.direct	=	EMBOSS_RIGHT_UP;
			else
				square.sc_data.direct	=	EMBOSS_MIDDLE;
			break;
		default:
			assert(false);
	}

}





/***************************************************************************************
	���͹w�]������map
/****************************************************************************************/
void	Screen::generate_center_line_map()
{
	area_map1.resize( size, size );
	area_map2.resize( size, size );

	int		i,	j;

	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
		{
			if( i < size/2 )
			{
				area_map1( i, j )	=	1;
				area_map2( i, j )	=	1;
			}
			else
			{
				area_map1( i, j )	=	2;
				area_map2( i, j )	=	2;
			}
		}
}


/***************************************************************************************
	���� 
	���� �N�y�� �ন map
	���w���Ȭ�����n�e�W�h���C��.
/****************************************************************************************/
bMatrixByte		Screen::screen_center_line_map( PIXEL anchor, int size, EMBOSS_DIRECTION direct, PIXEL start, PIXEL end )
{
	int				x1,			y1;					// �s��_�I�� �� �@���I
	int				x2,			y2;					// �_�I�䪺�t�@���I
	int				i,			j;
	int				start_x,	start_y;
	int				x,			y;
	double			value;
	bMatrixByte		r_data( size, size );			// �^�ǭ�

	if( direct == EMBOSS_LU_RD || direct == EMBOSS_LD_RU )
	{
		anchor.x	-=	size/2;
		anchor.y	+=	size/2;		// �]���w���I�O���W�䤤�I �ҥH�n����
	}

	/*
		���w�I (x1,y1)   (x2,y2)
		�o�� line     (x-x1)(y-y2) - (x-x2)(y-y1) = 0
	*/

	/*
		�o�䵹2  ��1 ����90��
		�O���F�f�tAOI�{��
		�ק諸�ܭn�p��bug
	*/

	switch(direct)
	{
		case EMBOSS_UP_DOWN:
			x1			=	anchor.x;	
			y1			=	anchor.y;
			x2			=	anchor.x;
			y2			=	anchor.y + size - 1;	// �Ҽ{�Y�����D
			assert( start.x == x1 && start.x == x2 && start.y >= y1 && start.y <= y2 );		// �ˬd�I�O�_���b��ؤW

			x1			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y1			=	anchor.y;
			x2			=	anchor.x + size - 1;
			y2			=	anchor.y + size - 1;
			assert( end.x == x1 && end.x == x2 && end.y >= y1 && end.y <= y2 );

			for( i = 0; i < size; i++ )
			{
				for( j = 0; j < size; j++ )
				{
					x		=	anchor.x + i;
					y		=	anchor.y + j;
					value	=	(x-start.x)*(y-end.y) - (x-end.x)*(y-start.y) ;
					if( value >= 0 )	r_data(i,j)		=	2;
					else				r_data(i,j)		=	1;		// �S�����B�z��
				}
			}
			break;

		case EMBOSS_LEFT_RIGHT:
			x1			=	anchor.x;	
			y1			=	anchor.y;
			x2			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y2			=	anchor.y;
			assert( start.x >= x1 && start.x <= x2 && start.y == y1 && start.y == y2 );

			x1			=	anchor.x;	
			y1			=	anchor.y + size - 1;	// �Ҽ{�Y�����D
			x2			=	anchor.x + size - 1;
			y2			=	anchor.y + size - 1;

			for( i = 0; i < size; i++ )
			{
				for( j = 0; j < size; j++ )
				{
					x		=	anchor.x + i;
					y		=	anchor.y + j;
					value	=	(x-start.x)*(y-end.y) - (x-end.x)*(y-start.y) ;
					if( value >= 0 )	r_data(i,j)		=	1;
					else				r_data(i,j)		=	2;		// �S�����B�z��
				}
			}
			break;

		case EMBOSS_LU_RD:
			x1			=	anchor.x;	
			y1			=	anchor.y;
			x2			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y2			=	anchor.y + size - 1;
			assert( start.x - x1 == start.y - y1 && x2 - start.x == y2 - start.y );

			x1			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y1			=	anchor.y - size + 1;	// ��ڤW�O + (size-1)    - (size-1)
			x2			=	x1 + size - 1;
			y2			=	y1 + size - 1;
			assert( end.x - x1 == end.y - y1 && x2 - end.x == y2 - end.y );

			for( i = 0; i < size; i++ )
			{
				start_x		=	anchor.x + i;
				start_y		=	anchor.y - i;

				for( j = 0; j < size; j++ )
				{
					x		=	start_x + j;
					y		=	start_y + j;	// x,y �O ������map���y��
					value	=	(x-start.x)*(y-end.y) - (x-end.x)*(y-start.y) ;

					if( value >= 0 )	r_data(j,i)		=	2;
					else				r_data(j,i)		=	1;		// �S�����B�z��
									       /* �G�N����90��*/
				}
			}
			break;

		case EMBOSS_LD_RU:
			x1			=	anchor.x;	
			y1			=	anchor.y;
			x2			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y2			=	anchor.y - size + 1;
			assert( start.x - x1 == y1 - start.y && x2 - start.x == start.y - y2 );

			x1			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y1			=	anchor.y + size - 1;
			x2			=	x1 + size - 1;
			y2			=	y1 - size + 1;
			assert( end.x - x1 == y1 - end.y && x2 - end.x == end.y - y2 );

			for( i = 0; i < size; i++ )
			{
				start_x		=	anchor.x + i;
				start_y		=	anchor.y + i;

				for( j = 0; j < size; j++ )
				{
					x		=	start_x + j;
					y		=	start_y - j;	// x,y �O ������map���y��
					value	=	(x-start.x)*(y-end.y) - (x-end.x)*(y-start.y) ;

					if( value >= 0 )	r_data(i,j)		=	1;
					else				r_data(i,j)		=	2;		// �S�����B�z��
				}
			}
			break;

		case EMBOSS_ALL_DIRECTION:
			r_data.clear();
			break;
		default:
			assert(false);
	}

	return	r_data;
}






/***************************************************************************************
	���� 
	���� �N�y�� �ন map
	���w���Ȭ�����n�e�W�h���C��.
	2�����צV�ϥ�
/****************************************************************************************/
bMatrixByte		Screen::screen_center_line_map2( PIXEL anchor, int size, EMBOSS_DIRECTION direct, PIXEL start, PIXEL end )
{
	int				x1,			y1;					// �s��_�I�� �� �@���I
	int				x2,			y2;					// �_�I�䪺�t�@���I
	int				i,			j;
	int				start_x,	start_y;
	int				x,			y;
	double			value;
	bMatrixByte		r_data( size, size );			// �^�ǭ�

	/*
		�o�䵹2  ��1 ����90��
		�O���F�f�tAOI�{��
		�ק諸�ܭn�p��bug
	*/


	if( direct == EMBOSS_LU_RD || direct == EMBOSS_LD_RU )
	{
		anchor.x	-=	size/2;
		anchor.y	+=	size/2;		// �]���w���I�O���W�䤤�I �ҥH�n����
	}

	switch(direct)
	{
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
			r_data.clear();
			break;

		case EMBOSS_LU_RD:
			x1			=	anchor.x;	
			y1			=	anchor.y;
			x2			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y2			=	anchor.y + size - 1;
			assert( start.x - x1 == start.y - y1 && x2 - start.x == y2 - start.y );

			x1			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y1			=	anchor.y - size + 1;	// ���O + (size-1) �� - (size-1)
			x2			=	x1 + size - 1;
			y2			=	y1 + size - 1;
			assert( end.x - x1 == end.y - y1 && x2 - end.x == y2 - end.y );

			for( i = 0; i < size; i++ )
			{
				start_x		=	anchor.x + i;
				start_y		=	anchor.y - i + 1;
									// �d�N�o��offset
				for( j = 0; j < size; j++ )
				{
					x		=	start_x + j;
					y		=	start_y + j;	// x,y �O ������map���y��
					value	=	(x-start.x)*(y-end.y) - (x-end.x)*(y-start.y) ;

					if( value >= 0 )	r_data(j,i)		=	2;
					else				r_data(j,i)		=	1;		// �S�����B�z��
					                     /*�G�N����90��*/
				}
			}
			break;

		case EMBOSS_LD_RU:
			x1			=	anchor.x;	
			y1			=	anchor.y;
			x2			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y2			=	anchor.y - size + 1;
			assert( start.x - x1 == y1 - start.y && x2 - start.x == start.y - y2 );

			x1			=	anchor.x + size - 1;	// �Ҽ{�Y�����D
			y1			=	anchor.y + size - 1;
			x2			=	x1 + size - 1;
			y2			=	y1 - size + 1;
			assert( end.x - x1 == y1 - end.y && x2 - end.x == end.y - y2 );

			for( i = 0; i < size; i++ )
			{
				start_x		=	anchor.x + i;
				start_y		=	anchor.y + i + 1;
									// �d�N�o��offset
				for( j = 0; j < size; j++ )
				{
					x		=	start_x + j;
					y		=	start_y - j;	// x,y �O ������map���y��
					value	=	(x-start.x)*(y-end.y) - (x-end.x)*(y-start.y) ;

					if( value >= 0 )	r_data(i,j)		=	1;
					else				r_data(i,j)		=	2;		// �S�����B�z��
				}
			}
			break;

		case EMBOSS_ALL_DIRECTION:
			r_data.clear();
			break;
		default:
			assert(false);
	}

	return	r_data;
}



/***************************************************************************************
	�]�w�Ϥ�
/****************************************************************************************/
void	Screen::set_image( AndreaImage imagePass )
{
	image	=	imagePass;
}







/***************************************************************************************
	�妸�ഫ���
/****************************************************************************************/
void	Screen::batch_linedata2matrix()
{
	assert( shadow.size1() == shadow.size2() && shadow.size1() == size );
	assert( shadow2.size1() == shadow2.size2() && shadow2.size1() == size );

	int		i,	j;

	switch(direction)
	{
		case EMBOSS_UP_DOWN:
			/*
				��ƱƦC        �ন 
				1   6           1  2  3  4  5
				2   7           6  7  8  9 10    
				3   8
				4   9
				5  10
				( i, j ) �۵�  �]����Ʀs��|�b����90��
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
					shadow( i, j )	=	line_data[i].shadow_seq(j);
			break;

		case EMBOSS_LEFT_RIGHT:
			/*
				��ƱƦC           �ন 
				1  2  3  4  5      1  2  3  4  5
				6  7  8  9 10      6  7  8  9 10          (�n�`�N���� 90 ��)
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
					shadow( i, j )	=	line_data[j].shadow_seq(i);
			break;


		case EMBOSS_LU_RD:			// left up <-> right down �Ȯɸ� EMBOSS_LEFT_UP �ۦP
			/*
				��ƱƦC           �ন 
				  6                1  2  3  4  5
				1   7              6  7  8  9 10         �o��Ʊ��ন�򥪤U�@�� (��K�L�X)
				  2  8
					3  9
					  4  10
						5
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[j].shadow_seq(i);
					// �ĤG����ƭn��
					shadow2( i, j )	=	line_data2[j].shadow_seq(i);
				}
			break;


		case EMBOSS_LD_RU:			// left down <-> right up �Ȯɸ�left down�ۦP
			/*
				��ƱƦC           �ন 
						5		   1  2  3  4  5            (�צV���Ʊ�H�o�Ӭ���)
					  4  10        6  7  8  9 10
					3   9
				  2   8  
				1   7    
				  6  8
				   11
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[i].shadow_seq(j);
					// �ĤG�����
					shadow2( i, j )	=	line_data2[i].shadow_seq(j);
				}
			break;
		case EMBOSS_ALL_DIRECTION:

			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					up_map( i, j )		=	line_data_up[i].shadow_seq(j);	
					down_map( i, j )	=	line_data_down[i].shadow_seq(size-j-1);
					left_map( i, j )	=	line_data_left[j].shadow_seq(i);
					right_map( i, j )	=	line_data_right[j].shadow_seq(size-i-1);

					// �ഫ�� �}�l�p�⵲�G (�֦��@�Ӱj�鬰�F�[�t)
					if( up_map(i,j) == 1 || down_map(i,j) == 1 || left_map(i,j) == 1 || right_map(i,j) == 1 )
						shadow( i, j )	=	1;
					else
						shadow( i, j )	=	0;

				}
			break;

		default:
			assert(false);

	}
}





/***************************************************************************************
	�@���ˬd�@���u
	�������V��
	�妸��
/****************************************************************************************/
void	Screen::batch_shadow_line( int th_id )
{
	int		i;

	// front ��V
	
	// ���ͩ��e�������
	seq_extend.set_value( 0, extend_len, seq_line_front[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line_front[th_id] );

	// ���v��� ��J�U�@�h��function���B��
	shadow_obj.screen_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp, th_id );

	// �R���ɭȪ�����
	line_front.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	line_front.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();


	// �����@�Ӥ�V�F....

	// back ��V

	// ���e�ɭ�
	seq_extend.set_value( 0, extend_len, seq_line_back[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line_back[th_id] );

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.screen_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp, th_id );

	// �R���ɭȪ�����
	line_back.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	line_back.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();


	// �i��merge �X�֨�Ӹ��  front back
	for( i = 0; i < original_len; i++ )
	{
		// �涰
		if( (bump == CONCAVE && dark_light == EMBOSS_LIGHT) || (bump == PROTRUDING && dark_light == EMBOSS_DARK) )
		{
			                                                                         /* ����  */
			if( line_front.shadow_seq(i) == SHADOW_VALUE && line_back.shadow_seq( original_len-i-1) == SHADOW_VALUE )
				line_data[th_id].shadow_seq(i)		=	SHADOW_VALUE;
			else
				line_data[th_id].shadow_seq(i)		=	UN_SHADOW_VALUE;
		}
		// �p��
		else 
		{
			                                                                        /* ���� */
			if( line_front.shadow_seq(i) == SHADOW_VALUE || line_back.shadow_seq(original_len-i-1) == SHADOW_VALUE )
				line_data[th_id].shadow_seq(i)		=	SHADOW_VALUE;
			else
				line_data[th_id].shadow_seq(i)		=	UN_SHADOW_VALUE;
		}
	}
}


/***************************************************************************************
	�@���ˬd�@���u
	�������V��
	�妸��
	2�� �ϥΦb�צV
/****************************************************************************************/
void	Screen::batch_shadow_line_2( int th_id )
{
	int		i;

	// front ��V
	
	// ���ͩ��e�������
	seq_extend.set_value( 0, extend_len, seq_line_front2[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line_front2[th_id] );

	// ���v��� ��J�U�@�h��function���B��
	shadow_obj.screen_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp, th_id );

	// �R���ɭȪ�����
	line_front.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	line_front.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();

	// �����@�Ӥ�V�F....

	// back ��V

	// ���e�ɭ�
	seq_extend.set_value( 0, extend_len, seq_line_back2[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line_back2[th_id] );

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.screen_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp, th_id );

	// �R���ɭȪ�����
	line_back.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	line_back.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();


	// �i��merge �X�֨�Ӹ��  front back
	for( i = 0; i < original_len; i++ )
	{
		// �涰
		if( (bump == CONCAVE && dark_light == EMBOSS_LIGHT) || (bump == PROTRUDING && dark_light == EMBOSS_DARK) )
		{
			if( line_front.shadow_seq(i) == SHADOW_VALUE && line_back.shadow_seq( original_len-i-1 ) == SHADOW_VALUE )
				line_data2[th_id].shadow_seq(i)		=	SHADOW_VALUE;
			else
				line_data2[th_id].shadow_seq(i)		=	UN_SHADOW_VALUE;
		}
		// �p��
		else 
		{
			if( line_front.shadow_seq(i) == SHADOW_VALUE || line_back.shadow_seq( original_len-i-1 ) == SHADOW_VALUE )
				line_data2[th_id].shadow_seq(i)		=	SHADOW_VALUE;
			else
				line_data2[th_id].shadow_seq(i)		=	UN_SHADOW_VALUE;
		}
	}


}



/***************************************************************************************
	�P�_�O�_������������V(�W�U���k)   
/****************************************************************************************/
bool	Screen::is_vertical_horizontal( const EMBOSS_DIRECTION direction )
{
	if( direction == EMBOSS_UP || direction == EMBOSS_DOWN ||
		direction == EMBOSS_LEFT || direction == EMBOSS_RIGHT ||
		direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT )
			return	true;
	else
			return	false;
}






/***************************************************************************************
	�]�w�˴�����ɽd��  �w���I
/****************************************************************************************/
void	Screen::set_square_bound()
{
	assert( roi_x >= 0 && roi_y >= 0 );
	assert( size > 0 );
	assert( direction < EMBOSS_DIRECTION_MAX );

	// �W�U���k�|��V
	if( is_vertical_horizontal(direction) || direction == EMBOSS_ALL_DIRECTION )
	{
		x_left	=	roi_x;						// x������_�l�I
		x_right	=	roi_x + size - 1;			// x���k����I
		y_up	=	roi_y;						// y���W��_�l�I
		y_down	=	roi_y + size - 1;   		// y���U����I
	}	
	// ���W���U�k�W�k�U�|�Ӥ�V  �o��|�y�L���I�n �Ȯɥ����B�z
	else
	{
		x_left	=	roi_x - size/2;				// x������_�l�I
		x_right	=	roi_x + size/2 + size;  	// x���k����I
		y_up	=	roi_y - size/2;         	// y���W��_�l�I
		y_down	=	roi_y + size/2 + size;  	// y���U����I
	}

	switch( direction )
	{
		case EMBOSS_UP :
			// �]�w�w���I (�̾ڤ�V)
			anchor_x	=	x_left;
			anchor_y	=	y_up;
			break;

		case EMBOSS_DOWN :
			anchor_x	=	x_left;
			anchor_y	=	y_down;
			break;

		case EMBOSS_LEFT :
			anchor_x	=	x_left;
			anchor_y	=	y_up;
			break;

		case EMBOSS_RIGHT :
			anchor_x	=	x_right;
			anchor_y	=	y_up;
			break;

		case EMBOSS_LEFT_UP :
			// �o��|�W�L���X�˴���  �n�p�� 
			// �P�_�覡�|��W�U���k���������Ǥ��P
			anchor_x	=	x_left;								// �צV����ո��
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;							// ��ƤG (���W�ΤU�����@��)
			anchor_y2	=	anchor_y + 1;
			break;
			
		case EMBOSS_LEFT_DOWN :
			anchor_x	=	x_left;
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;
			anchor_y2	=	anchor_y + 1;
			break;

		case EMBOSS_RIGHT_UP :
			anchor_x	=	x_right;
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;
			anchor_y2	=	anchor_y + 1;
			break;

		case EMBOSS_RIGHT_DOWN :
			anchor_x	=	x_right;
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;
			anchor_y2	=	anchor_y + 1; 
			break;

		case EMBOSS_LU_RD :			// ���W �� �k�U
			anchor_x	=	x_left;							// �צV����ո��
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;					// ��ƤG (���W�ΤU�����@��)
			anchor_y2	=	anchor_y + 1;
			break;

		case EMBOSS_LD_RU :			// ���U �� �k�W
			anchor_x	=	x_left;
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;
			anchor_y2	=	anchor_y + 1;
			break;

		case EMBOSS_ALL_DIRECTION:
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
			// ���ݭn�]�w���  �]���|��}�����P��V�Ӱ�
			break;

		default:
			assert(false);
	}

}




/***************************************************************************************
	�����˴��Ϊ�
	Ū�����ɸ��
	���V�[�t�� (�קK�O���魫�ƫŧi)
	�������] seq ���e�N�w�g����resize�ʧ@
/****************************************************************************************/
void	Screen::loadimage( bVectorByte &seq_front, bVectorByte &seq_back )
{
	assert( seq_front.size() == seq_back.size() && seq_front.size() == size );

	int			i;

	// �ˬd�I�O�_���T  �ˬd�۹��m
	switch(direction)
	{
		case EMBOSS_UP_DOWN:
			assert(start.x == end.x && start.y < end.y);
			break;
		case EMBOSS_LEFT_RIGHT:
			assert(start.x < end.x && start.y == end.y);
			break;
		case EMBOSS_LU_RD:
			assert(end.x-start.x == end.y-start.y);
			break;
		case EMBOSS_LD_RU:
			assert(end.x-start.x == start.y-end.y);
			break;
		default:
			assert(false);
	}

#ifdef _DEBUG  
	// �ˬdsize  debug�Ҧ��U�~�ϥ�
	int		tmp_size;
	switch(direction)
	{
		case EMBOSS_UP_DOWN:
			tmp_size	=	end.y - start.y + 1;
			break;
		case EMBOSS_LEFT_RIGHT:
			tmp_size	=	end.x - start.x + 1;
			break;
		case EMBOSS_LU_RD:
			tmp_size	=	end.x - start.x + 1;
			break;
		case EMBOSS_LD_RU:
			tmp_size	=	end.x - start.x + 1;
			break;
		default:
			assert(false);
	}
	assert( seq_front.size() == tmp_size && seq_back.size() == tmp_size && size == tmp_size );		// �ˬdseq��size
#endif

	assert( channel_func != NULL );

	// �}�lŪ���Ϥ����
	switch( direction )
	{
		case EMBOSS_UP_DOWN:
			for( i = 0; i < size; i++ )
			{
				seq_front(i)		=	(image.* channel_func)( start.x, start.y + i );
				// �����Nindex����
				seq_back(size-i-1)	=	(image.* channel_func)( start.x, start.y + i );
			}
			break;

		case EMBOSS_LEFT_RIGHT:
			for( i = 0; i < size; i++ )
			{
				seq_front(i)		=	(image.* channel_func)( start.x + i, start.y );
				seq_back(size-i-1)	=	(image.* channel_func)( start.x + i, start.y );
			}
			break;

		case EMBOSS_LU_RD:
			for( i = 0; i < size; i++ )
			{
				seq_front(i)		=	(image.* channel_func)( start.x + i, start.y + i );
				seq_back(size-i-1)	=	(image.* channel_func)( start.x + i, start.y + i );
			}
			break;

		case EMBOSS_LD_RU:
			for( i = 0; i < size; i++ )
			{
				seq_front(i)		=	(image.* channel_func)( start.x + i, start.y - i );
				seq_back(size-i-1)	=	(image.* channel_func)( start.x + i, start.y - i );
			}
			break;

		default :
			assert(false);
	}
}


/***************************************************************************************
	���v�˴��Ϊ�
	Ū�����ɸ��
	�ثe�|�Φb�M�wbump�o�ؤ��ݭn�Ĳv���a��.
/****************************************************************************************/
bVectorInt		Screen::loadimage( PIXEL s, PIXEL e, const EMBOSS_DIRECTION direction )
{
	bVectorInt		seq;
	int				i;

	// �ˬd�I�O�_���T  �ˬd�۹��m
	switch(direction)
	{
		case EMBOSS_UP:
			assert( s.x == e.x && s.y < e.y);
			break;
		case EMBOSS_DOWN:
			assert( s.x == e.x && s.y > e.y );
			break;
		case EMBOSS_LEFT:
			assert( s.x < e.x && s.y == e.y );
			break;
		case EMBOSS_RIGHT:
			assert( s.x > e.x && s.y == e.y );
			break;
		case EMBOSS_LEFT_UP:
			assert( e.x - s.x == e.y - s.y );
			break;
		case EMBOSS_LEFT_DOWN:
			assert( e.x - s.x == s.y - e.y );
			break;
		case EMBOSS_RIGHT_UP:
			assert( s.x - e.x == e.y - s.y );
			break;
		case EMBOSS_RIGHT_DOWN:
			assert( s.x - e.x == s.y - e.y );
			break;
		default:
			assert(false);
	}

#ifdef _DEBUG
	int		_size	=	0;

	// ���M�wsize  (�]���Ҽ{����� �o��|���Ҥ��P �w���Ҽ{�i�h)
	switch(direction)
	{
		case EMBOSS_UP :
			_size	=	e.y - s.y + 1;
			break;
		case EMBOSS_DOWN :
			_size	=	s.y - e.y + 1;
			break;
		case EMBOSS_LEFT :
			_size	=	e.x - s.x + 1;
			break;
		case EMBOSS_RIGHT :
			_size	=	s.x - e.x + 1;
			break;
		case EMBOSS_LEFT_UP :
			_size	=	e.x - s.x + 1;
			break;
		case EMBOSS_LEFT_DOWN :
			_size	=	e.x - s.x + 1;
			break;
		case EMBOSS_RIGHT_UP :
			_size	=	s.x - e.x + 1;
			break;
		case EMBOSS_RIGHT_DOWN :
			_size	=	s.x - e.x + 1;
			break;
		default:
			assert(false);
	}
	assert( _size == size );
#endif

	// ��l��seq
	seq.resize(size);

	assert( channel_func != NULL );

	// �}�lŪ���Ϥ����
	switch( direction )
	{
		case EMBOSS_UP :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( s.x, s.y + i );
			break;

		case EMBOSS_DOWN :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( s.x, s.y - i );
			break;

		case EMBOSS_LEFT:
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( s.x + i, s.y );
			break;

		case EMBOSS_RIGHT :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( s.x - i, s.y );
			break;

		case EMBOSS_LEFT_UP :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( s.x + i, s.y + i );
			break;

		case EMBOSS_LEFT_DOWN :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( s.x + i, s.y - i );
			break;

		case EMBOSS_RIGHT_UP :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( s.x - i, s.y + i );
			break;

		case EMBOSS_RIGHT_DOWN :
			for( i = 0; i < size; i++ )
				seq(i)	=	(image.* channel_func)( s.x - i, s.y - i );
			break;

		default :
			assert(false);
	}

	return	seq;
}


/***************************************************************************************
	�妸�edefect map
/****************************************************************************************/
void	Screen::batch_result_output_screen_defect( AndreaImage img, const int value )
{
	assert( shadow.size1() == shadow.size2()  );
	assert( shadow2.size1() == shadow2.size2()  );
	assert( shadow.size1() == shadow2.size1() && shadow.size1() == size );

	const int	x	=	roi_x;
	const int	y	=	roi_y;

	int		i,	j;
	int		mask		=	0x80;	// �γ̰��� = 1�ӷ�mask  �Ϥ���Ӱ϶�
	int		tmp_value	=	0;		

	switch(direction)
	{
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					tmp_value	=	0;
					if( area_map1(i,j) == 1 )
						tmp_value	|=	mask;						// ��϶� �@�Ӱ϶��[�Wmask �̰���bit
					if( shadow(i,j) == SHADOW_VALUE )
						tmp_value	|=	value;						// �g�J�쥻�����

					img.gray_channel( i + x, j + y, tmp_value );	// �g�J���
				}
			break;

		case EMBOSS_LU_RD:
		case EMBOSS_LD_RU:
		// �o��ӳ��Υ��U����  �Ӱ�����
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �Ĥ@��V
					tmp_value	=	0;
					if( area_map1(i,j) == 1 )
						tmp_value	|=	mask;			// ��϶� �@�Ӱ϶��[�Wmask �̰���bit
					if( shadow(i,j) == SHADOW_VALUE )
						tmp_value	|=	value;			// �g�J�쥻�����

					img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j, tmp_value );

					// �ĤG��V
					tmp_value	=	0;
					if( area_map2(i,j) == 1 )
						tmp_value	|=	mask;			// ��϶� �@�Ӱ϶��[�Wmask �̰���bit
					if( shadow2(i,j) == SHADOW_VALUE )
						tmp_value	|=	value;			// �g�J�쥻�����

					img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, tmp_value );		// (�T�w���� y+1 �� offset)*/
				}
			break;
		
		case EMBOSS_ALL_DIRECTION:
			// �u�n�X�{���v���� �S��mask���[��
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					if( shadow( i, j ) == SHADOW_VALUE )
						img.gray_channel( x + i, y + j , value );
				}

			break;


		default:
			assert(false);
	}
}




/***************************************************************************************
	�e�X��Ӱ϶�
/****************************************************************************************/
void	Screen::result_output_screen_area( AndreaImage img )
{
	int					x			=	roi_x;
	int					y			=	roi_y;
	int					i,			j;
	int					start_x,	start_y;

	assert( area_map1.size1() == size && area_map1.size2() == size );
	assert( area_map2.size1() == size && area_map2.size2() == size );

	if( direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU )
	{
		x	-=	size/2;
		y	+=	size/2;		// �w���I��^�����I
	}

	switch(direction)
	{
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					if( area_map1(i,j) == 1 )			img.pixel_channel( x+i, y+j, 123, 255, 0 );
					else if ( area_map1(i,j) == 2 )		img.pixel_channel( x+i, y+j, 255, 0, 123 );
					else								img.pixel_channel( x+i, y+j, 0, 123, 255 );
				}
			break;
		case EMBOSS_LU_RD:
			for( i = 0; i < size; i++ )
			{
				start_x	=	x + i;
				start_y	=	y - i;
				for( j = 0; j < size; j++ )
				{
					      /*�G�N������*/
					if( area_map1(j,i) == 1 )			img.pixel_channel( start_x+j, start_y+j, 123, 255, 0 );
					else if ( area_map1(j,i) == 2 )		img.pixel_channel( start_x+j, start_y+j, 255, 0, 123 );
					else								img.pixel_channel( start_x+j, start_y+j, 0, 123, 255 );

					if( area_map2(j,i) == 1 )			img.pixel_channel( start_x+j, start_y+j +1, 123, 255, 0 );
					else if ( area_map2(j,i) == 2 )		img.pixel_channel( start_x+j, start_y+j +1, 255, 0, 123 );
					else								img.pixel_channel( start_x+j, start_y+j +1, 0, 123, 255 );
				}
			}
			break;
		case EMBOSS_LD_RU:
			for( i = 0; i < size; i++ )
			{
				start_x	=	x + i;		
				start_y	=	y + i;
				for( j = 0; j < size; j++ )
				{
					if( area_map1(i,j) == 1 )			img.pixel_channel( start_x+j, start_y-j, 123, 255, 0 );
					else if ( area_map1(i,j) == 2 )		img.pixel_channel( start_x+j, start_y-j, 255, 0, 123 );
					else								img.pixel_channel( start_x+j, start_y-j, 0, 123, 255 );

					if( area_map2(i,j) == 1 )			img.pixel_channel( start_x+j, start_y-j +1, 123, 255, 0 );
					else if ( area_map2(i,j) == 2 )		img.pixel_channel( start_x+j, start_y-j +1, 255, 0, 123 );
					else								img.pixel_channel( start_x+j, start_y-j +1, 0, 123, 255 );
				}
			}
			break;
		case EMBOSS_ALL_DIRECTION:
			break;
		default:
			assert(false);
	}
}




/***************************************************************************************
	���o��V
/****************************************************************************************/
EMBOSS_DIRECTION	Screen::get_direction()
{
	//return	roi.direction;		// �n���S��k�ˬd�O�_����i���@~@  ���ӧ�Ϋ���(?)
	return	direction;
}




/***************************************************************************************
	�e�X���v
/****************************************************************************************/
void	Screen::result_output_shadow( AndreaImage img )
{
	assert( shadow.size1() == shadow.size2() && shadow2.size1() == shadow2.size2() && shadow.size1() == shadow2.size1() && shadow.size1() == size );

	const int			x			=	roi_x;
	const int			y			=	roi_y;

	int		i,	j;

	switch(direction)
	{
		case EMBOSS_UP :
		case EMBOSS_DOWN:
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X���v
					if( shadow(i,j) == SHADOW_VALUE )
						img.gray_channel( i + x, j + y, 227 );
						//img.pixel_channel( i + x, j + y, 0, 255, 255 );
				}
			break;

		case EMBOSS_LEFT_UP:
		case EMBOSS_LEFT_DOWN:
		case EMBOSS_LU_RD:
		case EMBOSS_LD_RU:
		// �o��ӳ��Υ��U����  �Ӱ�����
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X���v
					if( shadow(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j, 227 );
						//img.pixel_channel( x_left + i + j, (y_up+y_down)/2 + i - j, 0, 255, 255 );

					if( shadow2(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, 227 );
						//img.pixel_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, 0, 255, 255 );		// (�T�w���� y+1 �� offset)

				}
			break;

		case EMBOSS_RIGHT_UP:
		case EMBOSS_RIGHT_DOWN:
		// �o��ӭn�h�@��offset
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// ���X���v
					if( shadow(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j, 227 );
						//img.pixel_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j, 0, 255, 255 );
					                                           
					if( shadow2(i,j) == SHADOW_VALUE )
											           /*�d�N�o�䪺�p��*/
						img.gray_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j + 1, 227 );	
						//img.pixel_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j + 1, 0, 255, 255 );		// (�T�w���� y+1 �� offset)

				}
			break;
		
		case EMBOSS_ALL_DIRECTION:
			// �u�n�X�{���v���� �S��mask���[��
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					if( shadow( i, j ) == SHADOW_VALUE )
						img.gray_channel( x + i, y + j , 227 );
						//img.pixel_channel( x + i, y + j , 0, 255, 255 );
				}

			break;


		default:
			assert(false);
	}
}







/***************************************************************************************
	�]�w roi ��� 
	(�w�����ͦn�O����Ŷ��קKre-allocate)
/****************************************************************************************/
void	Screen::set( SquareData _roi )
{
	int		i;

	roi				=	_roi;
	size			=	roi.size;
	direction		=	roi.direction;
	dark_light		=	roi.dark_light;
	bump			=	roi.bump;

	// �]�w�nŪ����channel  �S�]�w�|�]�X���~
	switch( roi.channel )
	{
		case RED:
			channel_func	=	&AndreaImage::red_channel;
			break;

		case GREEN:
			channel_func	=	&AndreaImage::green_channel;
			break;

		case BLUE:
			channel_func	=	&AndreaImage::blue_channel;
			break;

		case GRAY:
			channel_func	=	&AndreaImage::gray_channel;
			break;

		default:
			assert(false);
	}

	original_len	=	size;

	roi_x			=	roi.x;
	roi_y			=	roi.y;

	good_bad		=	EMBOSS_GOOD;

	// �}�l�]�w�ܼ�
	shadow_settings.dark_light	=	roi.dark_light;
	shadow_settings.threshold	=	roi.threshold;
	shadow_settings.enhance		=	false;

	shadow_obj.set( original_len + extend_len, shadow_settings );	// �`�N �o�䪺size���O�쥻��size �O�[�W�ɭȪ�size!!


	line_data	=	new	ShadowReturnData[size];
	for( i = 0; i < size; i++ )
		line_data[i]	=	ShadowReturnData(size);
	line_data2	=	new ShadowReturnData[size];
	for( i = 0; i < size; i++ )
		line_data2[i]	=	ShadowReturnData(size);

	line_front	=	ShadowReturnData(size);
	line_back	=	ShadowReturnData(size);

	seq_line_front	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line_front[i].resize(size);

	seq_line_front2	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line_front2[i].resize(size);

	seq_line_back	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line_back[i].resize(size);

	seq_line_back2	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line_back2[i].resize(size);

	area_map1	=	roi.sc_data.area_map1;
	area_map2	=	roi.sc_data.area_map2;


	seq_extend.resize( original_len + extend_len );
	
	//shadow_data_tmp		=	ShadowReturnData( original_len + extend_len );
	shadow_data_tmp.init( original_len + extend_len );

	// ����V
	seq_line_up		=	new bVectorByte[size];
	seq_line_down	=	new bVectorByte[size];
	seq_line_left	=	new bVectorByte[size];
	seq_line_right	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
	{
		seq_line_up[i].resize(size);
		seq_line_down[i].resize(size);
		seq_line_left[i].resize(size);
		seq_line_right[i].resize(size);
	}

	line_data_up		=	new ShadowReturnData[size];
	line_data_down		=	new ShadowReturnData[size];
	line_data_left		=	new ShadowReturnData[size];
	line_data_right		=	new ShadowReturnData[size];
	for( i = 0; i < size; i++ )
	{
		line_data_up[i]		=	ShadowReturnData(size);
		line_data_down[i]	=	ShadowReturnData(size);
		line_data_left[i]	=	ShadowReturnData(size);
		line_data_right[i]	=	ShadowReturnData(size);
		
	}

	up_map.resize(size,size);
	down_map.resize(size,size);
	left_map.resize(size,size);
	right_map.resize(size,size);

	shadow.resize(size,size);
	shadow2.resize(size,size);


	// �o�ӳ]�w�����b�]�w�nroi����.
	set_square_bound();

}



/***************************************************************************************
	�M�w�Ӱ϶������ˬO�W�٬O�Y 
	Page2�|�����ϥ�
/****************************************************************************************/
bool	Screen::determine_bump( AndreaImage imagePass )
{
	image		=	imagePass;		// �]�w�Ϥ�

	assert( direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT || direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU || direction == EMBOSS_ALL_DIRECTION );
	
	double				value1,	value2;
	int					sum,	count;
	int					*mask;
	int					i,		j;
	int					x,		y;

	PIXEL				p1,		p2;
	bMatrixByte			map( size, size );
	bVectorInt			line( size );

	if( direction == EMBOSS_ALL_DIRECTION )
	{
		roi.bump	=	PROTRUDING;		// �w�]��
		return	true;
	}

	// Ū�����
	switch(direction)
	{
		case EMBOSS_UP_DOWN:
			for( i = 0; i < size; i++ )
			{
				p1.x	=	x_left + i;
				p2.x	=	x_left + i;
				p1.y	=	y_up;
				p2.y	=	y_down;
				line	=	loadimage( p1, p2, EMBOSS_UP );

				for( j = 0; j < size; j++ )
					map( i, j )		=	line(j);
			}
			break;

		case EMBOSS_LEFT_RIGHT:
			for( i = 0; i < size; i++ )
			{
				p1.x	=	x_left;
				p2.x	=	x_right;
				p1.y	=	y_up + i;
				p2.y	=	y_up + i;
				line	=	loadimage( p1, p2, EMBOSS_LEFT );

				for( j = 0; j < size; j++ )
					map( i, j )		=	line(j);
			}
			break;

		case EMBOSS_LU_RD:
			x	=	x_left;
			y	=	(y_up + y_down)/2;
			for( i = 0; i < size; i++ )
			{
				p1.x	=	x + i;							
				p2.x	=	x + i + size - 1;				
				p1.y	=	y - i;							
				p2.y	=	y - i + size - 1;				
				line	=	loadimage( p1, p2, EMBOSS_LEFT_UP );							// �Ĥ@����� �o�����ӥu�ݭn�@����ƴN���F

				for( j = 0; j < size; j++ )
					map( i, j )		=	line(j);
			}
			break;

		case EMBOSS_LD_RU:
			x	=	x_left;
			y	=	(y_up + y_down)/2;
			for( i = 0; i < size; i++ )
			{
				p1.x	=	x + i;
				p2.x	=	x + i + size - 1;
				p1.y	=	y + i ;
				p2.y	=	y + i - size + 1;
				line	=	loadimage( p1, p2, EMBOSS_LEFT_DOWN );

				for( j = 0; j < size; j++ )
					map( i, j )		=	line(j);
			}
			break;

		default:
			assert(false);
	}

	// �[�`
	for( i = 0; i < size; i++ )
	{
		sum		=	0;
		for( j = 0; j < size; j++ )
			sum		+=	map( j, i );
		line(i)		=	sum;
	}

	// ����mask
	mask	=	new int[size];

	for( i = 0; i < (size+1)/2; i++ )
		mask[i]	=	i+1;
	for( i = (size+1)/2; i < size; i++ )
		mask[i]	=	size-i;
	
	// �p���
	count	=	0;
	for( i = 0; i < size; i++ )
		count	+=	mask[i];

	// ��mask�[�`
	sum		=	0;
	for( i = 0; i < size; i++ )
		sum		+=	mask[i] * line(i);
	value1	=	1.0*sum / count;

	// �⥭��
	sum		=	0;
	for( i = 0; i < size; i++ )
		sum		+=	line(i);
	value2	=	1.0*sum / size;

	delete [] mask;		// �O�o�R�����
	mask	=	0;

	if( value1 > value2 )		// �Y
	{
		shadow_obj.bump			=	PROTRUDING;		// �����ק�F  ���F��K
		roi.bump				=	PROTRUDING;		// �o�䩳�U����R�� �|��bug
		bump					=	PROTRUDING;
		shadow_settings.bump	=	PROTRUDING;
	}
	else if( value1 < value2 )	// �W
	{
		shadow_obj.bump			=	CONCAVE;		// �����ק�F  ���F��K
		roi.bump				=	CONCAVE;
		bump					=	CONCAVE;
		shadow_settings.bump	=	CONCAVE;
	}
	else
	{
		shadow_obj.bump			=	BUMP_MAX;		// �����ק�F  ���F��K
		roi.bump				=	BUMP_MAX;		// default setting
		bump					=	BUMP_MAX;
		shadow_settings.bump	=	BUMP_MAX;
		return	false;
	}

	return	true;
}









/***************************************************************************************
	�P�_�O�_�� �� ��V
/****************************************************************************************/
bool	Screen::is_bin_direction( const EMBOSS_DIRECTION direction )
{
	if( direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT ||
		direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU )
			return	true;
	else
		return	false;
}




/***************************************************************************************
	�O�_�����V
/****************************************************************************************/
bool	Screen::is_single_direction( const EMBOSS_DIRECTION direction )
{
	if( direction == EMBOSS_UP || direction == EMBOSS_DOWN ||
		direction == EMBOSS_LEFT || direction == EMBOSS_RIGHT ||
		direction == EMBOSS_LEFT_UP || direction == EMBOSS_LEFT_DOWN ||
		direction == EMBOSS_RIGHT_UP || direction == EMBOSS_RIGHT_DOWN )
			return	true;
	else
		return	false;
}






/***************************************************************************************
	�����L�X���G
/****************************************************************************************/
void	Screen::result_output_screen( AndreaImage img )
{
	// �Ȯ�����
	//if( direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT ||
	//	direction == EMBOSS_LU_RD   || direction == EMBOSS_LD_RU )
	//		result_output_screen_area( img );				// �аO��Ӱ϶�

	result_output_shadow( img );					// �аO���v
}







/***************************************************************************************
	�۷��쥻��execution  
	multi-thread�ɨϥ�
	�������V�ϥ�
/****************************************************************************************/
void	Screen::exec()
{
	int		i;

	if( direction == EMBOSS_ALL_DIRECTION )
	{
		for( i = 0; i < size; i++ )
		{
			batch_shadow_ALLD(i);
		}
		return;
	}

	for( i = 0; i < size; i++ )
		batch_shadow_line(i);

	if( direction >= EMBOSS_LU_RD && direction <= EMBOSS_LD_RU )		// ���F�[�t �p��enum value
	{
		for( i = 0; i < size; i++ )
			batch_shadow_line_2(i);
	}
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Emboss  class  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/***************************************************************************************
	�غc
/****************************************************************************************/
Emboss::Emboss()
{
	channel_func	=	NULL;
	good_bad		=	EMBOSS_GOOD;
	//Shadow					shadow_obj;											// ���v�˴�����
	direction		=	EMBOSS_DIRECTION_MAX;					
	size			=	-1;											
	x_left			=	-1;
	x_right			=	-1;
	y_up			=	-1;
	y_down			=	-1;		
	roi_x			=	-1;
	roi_y			=	-1;
	anchor_x		=	-1;
	anchor_y		=	-1;
	anchor_x2		=	-1;
	anchor_y2		=	-1;
	start			=	PIXEL(-1,-1);
	end				=	PIXEL(-1,-1);;
	//AndreaImage				image;
	//SquareData				roi;												// �˴��ϸ�� �w���n�n���
	//KernelDensity			kd;													// �ΨӹL�o���T
	line_data		=	NULL;											
	line_data2		=	NULL;
	seq_line		=	NULL;	
	seq_line2		=	NULL;
	//bVectorByte				seq_extend;											// �|���ɭȪ��ʧ@
	original_len	=	-1;
	//ShadowReturnData		shadow_data_tmp;									// ���v���  (�@�ӬO��l�� �@�ӬO�R��������Ʈ��Ӧ^�ǥΪ���)
	//ShadowSettings			shadow_settings;									// ���v�]�w �ݩ�߿W���
	//bMatrixDouble			bm_tmp;												// kernel density�ϥ�
	seq_line_up		=	NULL;
	seq_line_down	=	NULL;
	seq_line_left	=	NULL;
	seq_line_right	=	NULL;
	line_data_up	=	NULL;
	line_data_down	=	NULL;
	line_data_left	=	NULL;
	line_data_right	=	NULL;
	//bMatrixByte				up_map;												// �Ȧs �̫�Ψ�merge
	//bMatrixByte				down_map;
	//bMatrixByte				left_map;
	//bMatrixByte				right_map;

	//shadow.resize(0,0);	
	//shadow2.resize(0,0);
	//deep;
	//deep2;


}


/***************************************************************************************
	�Ѻc
/****************************************************************************************/
Emboss::~Emboss()
{
	good_bad		=	EMBOSS_GOOD;
	//Shadow					shadow_obj;											// ���v�˴�����
	direction		=	EMBOSS_DIRECTION_MAX;					
	size			=	-1;											
	x_left			=	-1;
	x_right			=	-1;
	y_up			=	-1;
	y_down			=	-1;		
	roi_x			=	-1;
	roi_y			=	-1;
	anchor_x		=	-1;
	anchor_y		=	-1;
	anchor_x2		=	-1;
	anchor_y2		=	-1;
	start			=	PIXEL(-1,-1);
	end				=	PIXEL(-1,-1);;
	//AndreaImage				image;
	//SquareData				roi;												// �˴��ϸ�� �w���n�n���
	//KernelDensity			kd;													// �ΨӹL�o���T
	delete [] line_data;	line_data		=	NULL;											
	delete [] line_data2;	line_data2		=	NULL;
	delete [] seq_line;		seq_line		=	NULL;	
	delete [] seq_line2;	seq_line2		=	NULL;
	//bVectorByte				seq_extend;											// �|���ɭȪ��ʧ@
	original_len	=	-1;
	//ShadowReturnData		shadow_data_tmp;									// ���v���  (�@�ӬO��l�� �@�ӬO�R��������Ʈ��Ӧ^�ǥΪ���)
	//ShadowSettings			shadow_settings;									// ���v�]�w �ݩ�߿W���
	//bMatrixDouble			bm_tmp;												// kernel density�ϥ�
	delete [] seq_line_up;		seq_line_up		=	NULL;
	delete [] seq_line_down;	seq_line_down	=	NULL;
	delete [] seq_line_left;	seq_line_left	=	NULL;
	delete [] seq_line_right;	seq_line_right	=	NULL;
	delete [] line_data_up;		line_data_up	=	NULL;
	delete [] line_data_down;	line_data_down	=	NULL;
	delete [] line_data_left;	line_data_left	=	NULL;
	delete [] line_data_right;	line_data_right	=	NULL;
	//bMatrixByte				up_map;												// �Ȧs �̫�Ψ�merge
	//bMatrixByte				down_map;
	//bMatrixByte				left_map;
	//bMatrixByte				right_map;


}


/***************************************************************************************
	�妸����Ū��
/****************************************************************************************/
void	Emboss::batch_loadimage()
{
	int					i;
	int					x,		y,		x2,		y2;

	// �}�l���˴��Ϩ���ư��P�_
	switch( direction )
	{
		case EMBOSS_UP :
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x_left + i;
				end.x	=	x_left + i;
				start.y	=	y_up;
				end.y	=	y_down;
				loadimage( seq_line[i] );
			}
			break;

		case EMBOSS_DOWN :
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x_left + i;
				end.x	=	x_left + i;
				start.y	=	y_down;
				end.y	=	y_up;
				loadimage( seq_line[i] );
			}
			break;

		case EMBOSS_LEFT :
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x_left;
				end.x	=	x_right;
				start.y	=	y_up + i;
				end.y	=	y_up + i;
				loadimage( seq_line[i] );
			}
			break;

		case EMBOSS_RIGHT :
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x_right;
				end.x	=	x_left;
				start.y	=	y_up + i;
				end.y	=	y_up + i;
				loadimage( seq_line[i] );
			}
			break;

		case EMBOSS_LEFT_UP :
			x	=	x_left;
			y	=	(y_up + y_down)/2;
			x2	=	x;
			y2	=	y + 1;
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x + i;							
				end.x	=	x + i + size - 1;				
				start.y	=	y - i;							
				end.y	=	y - i + size - 1;				
				loadimage( seq_line[i] );

				start.x	=	x2 + i;
				end.x	=	x2 + i + size - 1;
				start.y	=	y2 - i;
				end.y	=	y2 - i + size - 1;
				loadimage( seq_line2[i] );
			}
			break;
			
		case EMBOSS_LEFT_DOWN :
			x	=	x_left;
			y	=	(y_up + y_down)/2;
			x2	=	x;
			y2	=	y + 1;
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x + i;
				end.x	=	x + i + size - 1;
				start.y	=	y + i ;
				end.y	=	y + i - size + 1;
				loadimage( seq_line[i] );

				start.x	=	x2 + i;
				end.x	=	x2 + i + size - 1;
				start.y	=	y2 + i ;
				end.y	=	y2 + i - size + 1;
				loadimage( seq_line2[i] );
			}
			break;

		case EMBOSS_RIGHT_UP :
			x	=	x_right;
			y	=	(y_up + y_down)/2;
			x2	=	x;
			y2	=	y + 1;
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x - i;
				end.x	=	x - i - size + 1;
				start.y	=	y - i;
				end.y	=	y - i + size - 1;
				loadimage( seq_line[i] );

				start.x	=	x2 - i;
				end.x	=	x2 - i - size + 1;
				start.y	=	y2 - i;
				end.y	=	y2 - i + size - 1;
				loadimage( seq_line2[i] );
			}
			break;

		case EMBOSS_RIGHT_DOWN :
			x	=	x_right;
			y	=	(y_up + y_down)/2;
			x2	=	x;
			y2	=	y + 1;
			for(  i = 0; i < size; i++ )
			{
				start.x	=	x - i;
				end.x	=	x - i - size + 1;
				start.y	=	y + i ;
				end.y	=	y + i - size + 1;
				loadimage( seq_line[i] );

				start.x	=	x2 - i;
				end.x	=	x2 - i - size + 1;
				start.y	=	y2 + i ;
				end.y	=	y2 + i - size + 1;
				loadimage( seq_line2[i] );
			}
			break;

		case EMBOSS_ALL_DIRECTION:
			for(  i = 0; i < size; i++ )
			{
				// EMBOSS_UP
				direction	=	EMBOSS_UP;

				start.x	=	x_left + i;
				end.x	=	x_left + i;
				start.y	=	y_up;
				end.y	=	y_down;
				loadimage( seq_line_up[i] );

				// EMBOSS_DOWN
				direction	=	EMBOSS_DOWN;

				start.x	=	x_left + i;
				end.x	=	x_left + i;
				start.y	=	y_down;
				end.y	=	y_up;
				loadimage( seq_line_down[i] );

				// EMBOSS_LEFT
				direction	=	EMBOSS_LEFT;

				start.x	=	x_left;
				end.x	=	x_right;
				start.y	=	y_up + i;
				end.y	=	y_up + i;
				loadimage( seq_line_left[i] );

				// EMBOSS_RIGHT
				direction	=	EMBOSS_RIGHT;

				start.x	=	x_right;
				end.x	=	x_left;
				start.y	=	y_up + i;
				end.y	=	y_up + i;
				loadimage( seq_line_right[i] );

				// �N��V��^��
				direction	=	EMBOSS_ALL_DIRECTION;		//��b�o��O���F�i�H�����@�ǿ��~
			}
			assert( direction == EMBOSS_ALL_DIRECTION );
			break;

		default:
			assert(false);
	}
}





/***************************************************************************************
	�]�w�Ϥ�
/****************************************************************************************/
void	Emboss::set_image( AndreaImage imagePass )
{
	image	=	imagePass;
}




/***************************************************************************************
	�妸�o�����T
/****************************************************************************************/
void	Emboss::batch_kernel_density()
{
	assert( kd_th >= 0 && kd_th <= 1 );

	if( direction == EMBOSS_ALL_DIRECTION )
		return;

	int					i,	j;			

	kd.dosomething( bm_tmp, shadow );

	// �N�o�����v����ưf�V�]�w�^�h line data
	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
		{
			// �o��ĥΨ��涰���@�k (���I�ܤ� �o��noise�y����)
			if( bm_tmp( i, j ) < kd_th )					// �u���@���I���L�o��
				shadow( i, j )	=	UN_SHADOW_VALUE;
		}

	// �צV�n���ĤG��
	if( !is_vertical_horizontal(roi.direction) )
	{
		kd.dosomething( bm_tmp, shadow2 );

		for( i = 0; i < size; i++ )
			for( j = 0; j < size; j++ )
			{
				// �o��ĥΨ��涰���@�k (���I�ܤ� �o��noise�y����)
				if( bm_tmp( i, j ) < kd_th )					// �u���@���I���L�o��
					shadow2( i, j )		=	UN_SHADOW_VALUE;
			}
	}
}



/***************************************************************************************
	�妸�ഫ���
/****************************************************************************************/
void	Emboss::batch_linedata2matrix()
{
	assert( shadow.size1() == shadow.size2() && shadow.size1() == deep.size1() && deep.size1() == deep.size2() );
	assert( shadow2.size1() == shadow2.size2() && shadow2.size1() == deep2.size1() && deep2.size1() == deep2.size2() );
	assert( shadow.size1() == shadow2.size1() && deep.size1() == deep2.size1() );

	int					i,	j;

	switch(direction)
	{
		case EMBOSS_UP :
			/*
				��ƱƦC        �ন 
				1   6           1  2  3  4  5
				2   7           6  7  8  9 10    
				3   8
				4   9
				5  10
				( i, j ) �۵�  �]����Ʀs��|�b����90��
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[i].shadow_seq(j);
					deep( i, j )	=	line_data[i].shadow_depth(j);
				}
			break;

		case EMBOSS_DOWN :
			/*
				��ƱƦC        �ন 
				5  10           1  2  3  4  5
				4   9           6  7  8  9 10    
				3   8
				2   7
				1   6
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[i].shadow_seq(size-j-1);
					deep( i, j )	=	line_data[i].shadow_depth(size-j-1);
				}
			break;

		case EMBOSS_LEFT :
			/*
				��ƱƦC           �ন 
				1  2  3  4  5      1  2  3  4  5
				6  7  8  9 10      6  7  8  9 10          (�n�`�N���� 90 ��)
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[j].shadow_seq(i);
					deep( i, j )	=	line_data[j].shadow_depth(i);
				}
			break;

		case EMBOSS_RIGHT :
			/*
				��ƱƦC           �ন 
				5  4  3  2  1      1  2  3  4  5
			   10  9  8  7  6      6  7  8  9 10          (�n�`�N���� 90 ��)
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[j].shadow_seq(size-i-1);
					deep( i, j )	=	line_data[j].shadow_depth(size-i-1);
				}
			break;

		case EMBOSS_LEFT_UP:
			/*
				��ƱƦC           �ন 
				  6                1  2  3  4  5
				1   7              6  7  8  9 10         �o��Ʊ��ন�򥪤U�@�� (��K�L�X)
				  2  8
					3  9
					  4  10
						5
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[j].shadow_seq(i);
					deep( i, j )	=	line_data[j].shadow_depth(i);
					// �ĤG����ƭn��
					shadow2( i, j )	=	line_data2[j].shadow_seq(i);
					deep2( i, j )	=	line_data2[j].shadow_depth(i);
				}
			break;

		case EMBOSS_LEFT_DOWN:
			/*
				��ƱƦC           �ন 
						5		   1  2  3  4  5            (�צV���Ʊ�H�o�Ӭ���)
					  4  10        6  7  8  9 10
					3   9
				  2   8  
				1   7    
				  6  8
				   11
			*/
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[i].shadow_seq(j);
					deep( i, j )	=	line_data[i].shadow_depth(j);
					// �ĤG�����
					shadow2( i, j )	=	line_data2[i].shadow_seq(j);
					deep2( i, j )	=	line_data2[i].shadow_depth(j);
				}
			break;

		case EMBOSS_RIGHT_UP:
			/*
				��ƱƦC           �ন 
				   11              1  2  3  4  5
					  6            6  7  8  9 10
					7   1    
				  8   2
				9   3
			 10   4
				5
			*/			
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[size-i-1].shadow_seq(size-j-1);
					deep( i, j )	=	line_data[size-i-1].shadow_depth(size-j-1);
					// �ĤG�����
					shadow2( i, j )	=	line_data2[size-i-1].shadow_seq(size-j-1);
					deep2( i, j )	=	line_data2[size-i-1].shadow_depth(size-j-1);
				}
			break;


		case EMBOSS_RIGHT_DOWN:
			/*
				��ƱƦC           �ন 
					 5	           1  2  3  4  5
				  10  4            6  7  8  9 10
					9  3  
					 8  2
					  7  1
					   6
			*/	
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					shadow( i, j )	=	line_data[size-j-1].shadow_seq(size-i-1);
					deep( i, j )	=	line_data[size-j-1].shadow_depth(size-i-1);
					// �ĤG�����
					shadow2( i, j )	=	line_data2[size-j-1].shadow_seq(size-i-1);
					deep2( i, j )	=	line_data2[size-j-1].shadow_depth(size-i-1);
				}
			break;

		case EMBOSS_ALL_DIRECTION:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					up_map( i, j )		=	line_data_up[i].shadow_seq(j);
					down_map( i, j )	=	line_data_down[i].shadow_seq(size-j-1);
					left_map( i, j )	=	line_data_left[j].shadow_seq(i);
					right_map( i, j )	=	line_data_right[j].shadow_seq(size-i-1);

					// �֦��@�Ӱj��  ���F�[�t 
					if( up_map(i,j) == SHADOW_VALUE || down_map(i,j) == SHADOW_VALUE || left_map(i,j) == SHADOW_VALUE || right_map(i,j) == SHADOW_VALUE )
						shadow( i, j )	=	1;
					else
						shadow( i, j )	=	0;
				}
			break;

		default:
			assert(false);

	}
}





/***************************************************************************************
	���v�˴��Ϊ�
	�@���ˬd�@���u
/****************************************************************************************/
void	Emboss::batch_shadow_line_2( int th_id )
{
	ShadowReturnData	&shadow_data	=	line_data2[th_id];

	// ���ͩ��e�������
	seq_extend.set_value( 0, extend_len, seq_line2[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line2[th_id] );

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );
	shadow_data.shadow_depth.add_vector( shadow_data_tmp.shadow_depth,    extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();
}






/***************************************************************************************
	���v�˴��Ϊ�
	�@���ˬd�@���u
	����V�ϥ�
/****************************************************************************************/
void	Emboss::batch_shadow_ALLD( int th_id )
{
	// EMBOSS_UP
	ShadowReturnData	&shadow_data	=	line_data_up[th_id];

	// ���ͩ��e�������
	// ���e�ɭ�
	seq_extend.set_value( 0, extend_len, seq_line_up[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line_up[th_id] );

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );
	shadow_data.shadow_depth.add_vector( shadow_data_tmp.shadow_depth,    extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();

	// EMBOSS_DOWN
	ShadowReturnData	&shadow_data2	=	line_data_down[th_id];

	// ���ͩ��e�������
	// ���e�ɭ�
	seq_extend.set_value( 0, extend_len, seq_line_down[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line_down[th_id] );

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data2.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data2.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );
	shadow_data2.shadow_depth.add_vector( shadow_data_tmp.shadow_depth,    extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();

	// EMBOSS_LEFT
	ShadowReturnData	&shadow_data3	=	line_data_left[th_id];

	// ���ͩ��e�������
	// ���e�ɭ�
	seq_extend.set_value( 0, extend_len, seq_line_left[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line_left[th_id] );

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data3.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data3.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );
	shadow_data3.shadow_depth.add_vector( shadow_data_tmp.shadow_depth,    extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();

	// EMBOSS_RIGHT
	ShadowReturnData	&shadow_data4	=	line_data_right[th_id];

	// ���ͩ��e�������
	// ���e�ɭ�
	seq_extend.set_value( 0, extend_len, seq_line_right[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line_right[th_id] );

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data4.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data4.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );
	shadow_data4.shadow_depth.add_vector( shadow_data_tmp.shadow_depth,    extend_len, extend_len+original_len );

	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();
}



/***************************************************************************************
	���v�˴��Ϊ�
	�@���ˬd�@���u
/****************************************************************************************/
void	Emboss::batch_shadow_line( int th_id )
{
	ShadowReturnData	&shadow_data	=	line_data[th_id];

	// ���ͩ��e�������
	seq_extend.set_value( 0, extend_len, seq_line[th_id](0) );
	// ���^�쥻�����
	seq_extend.add_vector( extend_len, seq_line[th_id] );

	// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����
	shadow_obj.shadow_detector( seq_extend, /*shadow_settings,*/ shadow_data_tmp );

	// �R���ɭȪ�����
	shadow_data.mono_sub_seq.add_vector( shadow_data_tmp.mono_sub_seq,    extend_len, extend_len+original_len );
	shadow_data.shadow_seq.add_vector  ( shadow_data_tmp.shadow_seq,      extend_len, extend_len+original_len );
	shadow_data.shadow_depth.add_vector( shadow_data_tmp.shadow_depth,    extend_len, extend_len+original_len );


	shadow_data_tmp.mono_sub_seq.clear_add_value();		// �M���ɭ�
	seq_extend.clear_add_value();
}





/***************************************************************************************
	�P�_�O�_������������V(�W�U���k)   
/****************************************************************************************/
bool	Emboss::is_vertical_horizontal( const EMBOSS_DIRECTION direction )
{
	if( direction == EMBOSS_UP || direction == EMBOSS_DOWN ||
		direction == EMBOSS_LEFT || direction == EMBOSS_RIGHT ||
		direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT )
			return	true;
	else
			return	false;
}







/***************************************************************************************
	�]�w�˴�����ɽd��  �w���I
/****************************************************************************************/
void	Emboss::set_square_bound()
{
	assert( roi_x >= 0 && roi_y >= 0 );
	assert( size > 0 );
	assert( direction < EMBOSS_DIRECTION_MAX );

	// �W�U���k�|��V
	if( is_vertical_horizontal(direction) || direction == EMBOSS_ALL_DIRECTION )
	{
		x_left	=	roi_x;					// x������_�l�I
		x_right	=	roi_x + size - 1;		// x���k����I
		y_up	=	roi_y;					// y���W��_�l�I
		y_down	=	roi_y + size - 1;       // y���U����I
	}	
	// ���W���U�k�W�k�U�|�Ӥ�V  �o��|�y�L���I�n �Ȯɥ����B�z
	else
	{
		x_left	=	roi_x - size/2;			// x������_�l�I
		x_right	=	roi_x + size/2 + size;  // x���k����I
		y_up	=	roi_y - size/2;         // y���W��_�l�I
		y_down	=	roi_y + size/2 + size;  // y���U����I
	}

	switch( direction )
	{
		case EMBOSS_UP :
			// �]�w�w���I (�̾ڤ�V)
			anchor_x	=	x_left;
			anchor_y	=	y_up;
			break;

		case EMBOSS_DOWN :
			anchor_x	=	x_left;
			anchor_y	=	y_down;
			break;

		case EMBOSS_LEFT :
			anchor_x	=	x_left;
			anchor_y	=	y_up;
			break;

		case EMBOSS_RIGHT :
			anchor_x	=	x_right;
			anchor_y	=	y_up;
			break;

		case EMBOSS_LEFT_UP :
			// �o��|�W�L���X�˴���  �n�p�� 
			// �P�_�覡�|��W�U���k���������Ǥ��P
			anchor_x	=	x_left;								// �צV����ո��
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;							// ��ƤG (���W�ΤU�����@��)
			anchor_y2	=	anchor_y + 1;
			break;
			
		case EMBOSS_LEFT_DOWN :
			anchor_x	=	x_left;
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;
			anchor_y2	=	anchor_y + 1;
			break;

		case EMBOSS_RIGHT_UP :
			anchor_x	=	x_right;
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;
			anchor_y2	=	anchor_y + 1;
			break;

		case EMBOSS_RIGHT_DOWN :
			anchor_x	=	x_right;
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;
			anchor_y2	=	anchor_y + 1; 
			break;

		case EMBOSS_LU_RD :			// ���W �� �k�U
			anchor_x	=	x_left;							// �צV����ո��
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;					// ��ƤG (���W�ΤU�����@��)
			anchor_y2	=	anchor_y + 1;
			break;

		case EMBOSS_LD_RU :			// ���U �� �k�W
			anchor_x	=	x_left;
			anchor_y	=	(y_up + y_down)/2;
			anchor_x2	=	anchor_x;
			anchor_y2	=	anchor_y + 1;
			break;

		case EMBOSS_ALL_DIRECTION:
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
			// ���ݭn�]�w���  �]���|��}�����P��V�Ӱ�
			break;

		default:
			assert(false);
	}

}





/***************************************************************************************
	���v�˴��Ϊ�
	Ū�����ɸ��
	�[�t�� (�קK�O���魫�ƫŧi)
	�������] seq ���e�N�w�g����resize�ʧ@
/****************************************************************************************/
void	Emboss::loadimage( bVectorByte &seq )
{
	int			i;

	// �ˬd�I�O�_���T  �ˬd�۹��m
	switch(direction)
	{
		case EMBOSS_UP:
			assert(start.x == end.x && start.y < end.y);
			break;
		case EMBOSS_DOWN:
			assert(start.x == end.x && start.y > end.y);
			break;
		case EMBOSS_LEFT:
			assert(start.x < end.x && start.y == end.y);
			break;
		case EMBOSS_RIGHT:
			assert(start.x > end.x && start.y == end.y);
			break;
		case EMBOSS_LEFT_UP:
			assert(end.x-start.x == end.y-start.y);
			break;
		case EMBOSS_LEFT_DOWN:
			assert(end.x-start.x == start.y-end.y);
			break;
		case EMBOSS_RIGHT_UP:
			assert(start.x-end.x == end.y-start.y);
			break;
		case EMBOSS_RIGHT_DOWN:
			assert(start.x-end.x == start.y-end.y);
			break;
		default:
			assert(false);
	}

#ifdef _DEBUG
	// ���M�wsize  (�]���Ҽ{����� �o��|���Ҥ��P �w���Ҽ{�i�h) 
	// �ˬdsize  debug�Ҧ��U�~�ϥ�
	int		tmp_size;
	switch(direction)
	{
		case EMBOSS_UP :
			tmp_size	=	end.y - start.y + 1;
			break;
		case EMBOSS_DOWN :
			tmp_size	=	start.y - end.y + 1;
			break;
		case EMBOSS_LEFT :
			tmp_size	=	end.x - start.x + 1;
			break;
		case EMBOSS_RIGHT :
			tmp_size	=	start.x - end.x + 1;
			break;
		case EMBOSS_LEFT_UP :
			tmp_size	=	end.x - start.x + 1;
			break;
		case EMBOSS_LEFT_DOWN :
			tmp_size	=	end.x - start.x + 1;
			break;
		case EMBOSS_RIGHT_UP :
			tmp_size	=	start.x - end.x + 1;
			break;
		case EMBOSS_RIGHT_DOWN :
			tmp_size	=	start.x - end.x + 1;
			break;
		default:
			assert(false);
	}
	assert( seq.size() == tmp_size );		// �ˬdseq��size
	assert( size == tmp_size );				// �ˬd��setting��size
#endif

	// �}�lŪ���Ϥ����
	switch( direction )
	{
		case EMBOSS_UP :
			for( i = 0; i < size; i++ )
				//seq( i )	=	image.gray_channel( start.x, start.y + i );
				seq(i)		=	(image.* channel_func)( start.x, start.y + i );
			break;

		case EMBOSS_DOWN :
			for( i = 0; i < size; i++ )
				//seq( i )	=	image.gray_channel( start.x, start.y - i );
				seq( i )	=	(image.* channel_func)( start.x, start.y - i );
			break;

		case EMBOSS_LEFT:
			for( i = 0; i < size; i++ )
				//seq( i )	=	image.gray_channel( start.x + i, start.y );
				seq( i )	=	(image.* channel_func)( start.x + i, start.y );
			break;

		case EMBOSS_RIGHT :
			for( i = 0; i < size; i++ )
				//seq( i )	=	image.gray_channel( start.x - i, start.y );
				seq( i )	=	(image.* channel_func)( start.x - i, start.y );
				break;

		case EMBOSS_LEFT_UP :
			for( i = 0; i < size; i++ )
				//seq( i )	=	image.gray_channel( start.x + i, start.y + i );
				seq( i )	=	(image.* channel_func)( start.x + i, start.y + i );
			break;

		case EMBOSS_LEFT_DOWN :
			for( i = 0; i < size; i++ )
				//seq( i )	=	image.gray_channel( start.x + i, start.y - i );
				seq( i )	=	(image.* channel_func)( start.x + i, start.y - i );
			break;

		case EMBOSS_RIGHT_UP :
			for( i = 0; i < size; i++ )
				//seq(i)	=	image.gray_channel( start.x - i, start.y + i );
				seq( i )	=	(image.* channel_func)( start.x - i , start.y + i );
			break;

		case EMBOSS_RIGHT_DOWN :
			for( i = 0; i < size; i++ )
				//seq(i)	=	image.gray_channel( start.x - i, start.y - i );
				seq( i )	=	(image.* channel_func)( start.x - i, start.y - i );
			break;

		default :
			assert(false);
	}
}





/***************************************************************************************
	����mask���
	(�w�] �ĤG���ϥ� �קK�S���)
/****************************************************************************************/
void	Emboss::generate_allow_area()
{
	const int			up_1		=	roi.em_data.up_line_1;
	const int			up_2		=	roi.em_data.up_line_2;
	const int			down_1		=	roi.em_data.down_line_1;
	const int			down_2		=	roi.em_data.down_line_2;

	int			i,		j;

	// ��l�Ƹ��
	allow_area.resize( size, size );
	allow_area.clear();
	allow_area2.resize( size, size );
	allow_area.clear();

	switch(direction)
	{
		case EMBOSS_UP :
		case EMBOSS_DOWN :
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
					allow_area( i, j )	=	0;
			break;
		case EMBOSS_LEFT :
		case EMBOSS_RIGHT :
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
					allow_area( i, j )	=	0;
			break;
		case EMBOSS_LEFT_UP :
		case EMBOSS_RIGHT_DOWN :
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					allow_area( i, j )	=	0;
					allow_area2( i, j )	=	0;
				}
			break;
		case EMBOSS_LEFT_DOWN :
		case EMBOSS_RIGHT_UP :
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					allow_area( i, j )	=	0;
					allow_area2( i, j )	=	0;
				}
			break;

		case EMBOSS_ALL_DIRECTION:
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
		case EMBOSS_LU_RD:
		case EMBOSS_LD_RU:
			// ���ݭn�����\��
			break;

		default:
			assert(false);
	}
}



/***************************************************************************************
	�妸�аOdefect map
	�e�\�Ϸ|�[�W�Ĥ@�Ӧ줸�@��mask
/****************************************************************************************/
void	Emboss::batch_result_output_emboss_defect( AndreaImage img, const int value )
{
	assert( shadow.size1() == shadow.size2() );
	assert( shadow2.size1() == shadow2.size2() );

	const int	x	=	roi_x;
	const int	y	=	roi_y;

	int		i,	j;
	int		tmp_value	=	0;			// �Ȧs�� ���F�[�Wmask
	int		mask		=	0x80;		// 1byte�̰��쬰1

	switch(direction)
	{
		case EMBOSS_UP :
		case EMBOSS_DOWN:
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					tmp_value	=	0;	
					if( allow_area(i,j) == 1 )
						tmp_value	|=	mask;					// �[�W�̰���bit��mask  AOI�{���|�ݨ�z����
					if( shadow(i,j) == SHADOW_VALUE )
						tmp_value	|=	value;					// �O���v�N�[�W���v��

					img.gray_channel( i+x, j+y, tmp_value );	// �g�J��T �ק令���ޫ�˳��|�g�J (�S��ƴN�O0)
				}
			break;

		case EMBOSS_LEFT_UP:
		case EMBOSS_LEFT_DOWN:
		//case EMBOSS_LU_RD:
		//case EMBOSS_LD_RU:
		// �o��ӳ��Υ��U����  �Ӱ�����
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �Ĥ@��V
					tmp_value	=	0;	
					if( allow_area(i,j) == 1 )
						tmp_value	|=	mask;				// �[�W�̰���bit��mask  AOI�{���|�ݨ�z����
					if( shadow(i,j) == SHADOW_VALUE )
						tmp_value	|=	value;				// �O���v�N�[�W���v��

					img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j, tmp_value );		
					
					// �ĤG��V
					tmp_value	=	0;	
					if( allow_area2(i,j) == 1 )
						tmp_value	|=	mask;				// �[�W�̰���bit��mask  AOI�{���|�ݨ�z����
					if( shadow2(i,j) == SHADOW_VALUE )
						tmp_value	|=	value;				// �O���v�N�[�W���v��
					
					img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, tmp_value );		// (�T�w���� y+1 �� offset)
				}
			break;

		case EMBOSS_RIGHT_UP:
		case EMBOSS_RIGHT_DOWN:
		// �o��ӭn�h�@��offset
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �Ĥ@��V
					tmp_value	=	0;	
					if( allow_area(i,j) == 1 )
						tmp_value	|=	mask;				// �[�W�̰���bit��mask  AOI�{���|�ݨ�z����
					if( shadow(i,j) == SHADOW_VALUE )
						tmp_value	|=	value;				// �O���v�N�[�W���v��	

					img.gray_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j, tmp_value );

					// �ĤG��V
					tmp_value	=	0;	
					if( allow_area2(i,j) == 1 )
						tmp_value	|=	mask;				// �[�W�̰���bit��mask  AOI�{���|�ݨ�z����
					if( shadow2(i,j) == SHADOW_VALUE )
						tmp_value	|=	value;				// �O���v�N�[�W���v��

					img.gray_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j + 1, tmp_value );		// (�T�w���� y+1 �� offset)
											           /*�d�N�o�䪺�p��*/
				}
			break;
		
		case EMBOSS_ALL_DIRECTION:
			// �u�n�X�{���v���� �S��mask���[��
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
					if( shadow( i, j ) == SHADOW_VALUE )
						img.gray_channel( x + i, y + j , value );
			break;


		default:
			assert(false);
	}
}






/***************************************************************************************
	���o��V
/****************************************************************************************/
EMBOSS_DIRECTION	Emboss::get_direction()
{
	return	roi.direction;		// �n���S��k�ˬd�O�_����i���@~@  ���ӧ�Ϋ���(?)
}



/***************************************************************************************
	�e�X���v
/****************************************************************************************/
void	Emboss::result_output_shadow( AndreaImage img )
{
	assert( shadow.size1() == shadow.size2()  );
	assert( shadow2.size1() == shadow2.size2()  );

	const int	x	=	roi_x;
	const int	y	=	roi_y;

	int		i,	j;

	switch(direction)
	{
		case EMBOSS_UP :
		case EMBOSS_DOWN:
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X���v
					if( shadow(i,j) == SHADOW_VALUE )
						//img.gray_channel( i + x, j + y, 31 );
						img.pixel_channel( i + x, j + y, 0, 255, 255 );
					//else
					//	img.gray_channel( i+x, j+y, 0 );
				}
			break;

		case EMBOSS_LEFT_UP:
		case EMBOSS_LEFT_DOWN:
		case EMBOSS_LU_RD:
		case EMBOSS_LD_RU:
		// �o��ӳ��Υ��U����  �Ӱ�����
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X���v
					if( shadow(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j, 31 );
						//img.pixel_channel( x_left + i + j, (y_up+y_down)/2 + i - j, 0, 255, 255 );

					if( shadow2(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, 31 );
						//img.pixel_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, 0, 255, 255 );		// (�T�w���� y+1 �� offset)

				}
			break;

		case EMBOSS_RIGHT_UP:
		case EMBOSS_RIGHT_DOWN:
		// �o��ӭn�h�@��offset
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// ���X���v
					if( shadow(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j, 31 );
						//img.pixel_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j, 0, 255, 255 );
					                                           
					if( shadow2(i,j) == SHADOW_VALUE )
											           /*�d�N�o�䪺�p��*/
						img.gray_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j + 1, 31 );
						//img.pixel_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j + 1, 0, 255, 255 );		// (�T�w���� y+1 �� offset)

				}
			break;
		
		case EMBOSS_ALL_DIRECTION:
			// �u�n�X�{���v���� �S��mask���[��
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					if( shadow( i, j ) == SHADOW_VALUE )
						img.gray_channel( x + i, y + j , 31 );
						//img.pixel_channel( x + i, y + j , 0, 255, 255 );
				}

			break;


		default:
			assert(false);
	}
}






/***************************************************************************************
	�e�X�e�\��
/****************************************************************************************/
void	Emboss::result_output_emboss_allow_area( AndreaImage img )
{
	const int			x				=	roi_x;
	const int			y				=	roi_y;

	int		i,	j;
	int		count	=	0;

	switch(direction)
	{
		case EMBOSS_UP :
		case EMBOSS_DOWN:
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X�e�\��
					if( allow_area(i,j) == SHADOW_VALUE )
						img.pixel_channel( i + x, j + y, 255, 255, 0 );
				}
			break;

		case EMBOSS_LEFT_UP:
		case EMBOSS_LEFT_DOWN:
		// �o��ӳ��Υ��U����  �Ӱ�����
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X�e�\��
					if( allow_area(i,j) == SHADOW_VALUE )
						img.pixel_channel( x_left + i + j, (y_up+y_down)/2 + i - j, 255, 255, 0 );
					if( allow_area2(i,j) == SHADOW_VALUE )
						img.pixel_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, 255, 255, 0 );		// (�T�w���� y+1 �� offset)

				}
			break;

		case EMBOSS_RIGHT_UP:
		case EMBOSS_RIGHT_DOWN:
		// �o��ӭn�h�@��offset
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X�e�\��
					if( allow_area(i,j) == SHADOW_VALUE )
						img.pixel_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j, 255, 255, 0 );
					if( allow_area2(i,j) == SHADOW_VALUE )
						img.pixel_channel(x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j + 1, 255, 255, 0 );		// (�T�w���� y+1 �� offset)


				}
			break;

		case EMBOSS_ALL_DIRECTION:			// ����쪺�S���e�\��
			break;
	
		default:
			assert(false);
	}

}





/***************************************************************************************
	�e�X���
/****************************************************************************************/
void	Emboss::result_output_emboss_centroid( AndreaImage img )
{
	const int			cx			=	(int)roi.em_data.centroid_x;
	const int			cy			=	(int)roi.em_data.centroid_y;
	const int			cx2			=	(int)roi.em_data.centroid_x2;
	const int			cy2			=	(int)roi.em_data.centroid_y2;
	const int			x			=	roi_x;
	const int			y			=	roi_y;

	int					x1,	x2,	y1,	y2;

	assert( cx < size && cy < size && cx2 < size && cy2 < size );

	switch(direction)
	{
		case EMBOSS_UP:
		case EMBOSS_DOWN:
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
			if( cx >= 0 && cy >= 0 )
			{
				x1	=	x + cx;
				y1	=	y + cy;
				img.gray_channel( x1, y1, 224 );
				//img.pixel_channel( x1, y1, 255, 0, 0 );
			}
			break;

		case EMBOSS_LEFT_UP:
		case EMBOSS_LEFT_DOWN:
			if( cx >= 0 && cy >= 0 && cx2 >= 0 && cy2 >= 0 )
			{
				x1	=	x_left + cx + cy;
				y1	=	(y_up+y_down)/2 + cx - cy;
				x2	=	x_left + cx2 + cy2;
				y2	=	(y_up+y_down)/2 + cx2 - cy2 + 1;
				img.gray_channel( (x1+x2)/2, (y1+y2)/2, 224 );
				//img.pixel_channel( (x1+x2)/2, (y1+y2)/2, 255, 0, 0 );
			}
			break;

		case EMBOSS_RIGHT_UP:
		case EMBOSS_RIGHT_DOWN:
			if( cx >= 0 && cy >= 0 && cx2 >= 0 && cy2 >= 0 )
			{
				x1	=	x_left + cx + cy + 1 + (1+size)%2;
				y1	=	(y_up+y_down)/2 + cx - cy;
				x2	=	x_left + cx2 + cy2 + 1 + (1+size)%2;
				y2	=	(y_up+y_down)/2 + cx2 - cy2 + 1;
				img.gray_channel( (x1+x2)/2, (y1+y2)/2, 224 );
				//img.pixel_channel( (x1+x2)/2, (y1+y2)/2, 255, 0, 0 );
			}
			break;

		case EMBOSS_ALL_DIRECTION:	// �L���
			break;

		default:
			assert(false);
	}
}




/***************************************************************************************
	�W�Y���G��X  (��shadow���)
	��� ��X
/****************************************************************************************/
void	Emboss::result_output_emboss( AndreaImage img )
{
	//result_output_emboss_allow_area(  img );			// �e�X�e�\��
	result_output_shadow(  img );						// �e�X���v
	result_output_emboss_centroid(  img );				// �e�X���
}




/***************************************************************************************
	�]�w roi ��� 
	(�w�����ͦn�O����Ŷ��קKre-allocate)
/****************************************************************************************/
void	Emboss::set( SquareData _roi )
{
	int		i;

	roi				=	_roi;
	size			=	roi.size;
	direction		=	roi.direction;

	original_len	=	size;

	roi_x			=	roi.x;
	roi_y			=	roi.y;

	good_bad		=	EMBOSS_GOOD;

	// �]�w�nŪ����channel  �S�]�w�|�]�X���~
	switch( roi.channel )
	{
		case RED:
			channel_func	=	&AndreaImage::red_channel;
			break;

		case GREEN:
			channel_func	=	&AndreaImage::green_channel;
			break;

		case BLUE:
			channel_func	=	&AndreaImage::blue_channel;
			break;

		case GRAY:
			channel_func	=	&AndreaImage::gray_channel;
			break;

		default:
			assert(false);
	}

	
	// �Ȯɥ��ιϤ��j�p�ӨM�w kernel density (���p�t�׼v�T�L�j �~�令�w���]�w���覡,�H��n�q�����Ӱ��ק�)
	if( roi.img_width >= 8000 )
		kd_th	=	1.5 / 9.0 ;
	else if( roi.img_width >= 4000 )
		kd_th	=	1.5 / 9.0 ;
	else if( roi.img_width < 3000 )
		kd_th	=	1.5 / 9.0 ;
	else
		assert(false);

	// �P�ɪ�l�Ƹ�� �n�O����

	// ��V����
	seq_line	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line[i].resize(size);
	seq_line2	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line2[i].resize(size);

	seq_extend.resize( original_len + extend_len );
	shadow_data_tmp	=	ShadowReturnData( original_len + extend_len );

	
	// �s��edge���
	edge.resize(size);
	edge2.resize(size);


	line_data	=	new ShadowReturnData[size];
	for( i = 0; i < size; i++ )
		line_data[i]	=	ShadowReturnData( size );
	line_data2		=	new ShadowReturnData[size];
	for( i = 0; i < size; i++ )
		line_data2[i]	=	ShadowReturnData( size );

	bm_tmp.resize( size, size );

	allow_area		=	roi.em_data.allow_area;		// �e�\�ϸ��
	allow_area2		=	roi.em_data.allow_area2;

	// ���v�]�w
	shadow_settings.enhance		=	roi.enhance;
	shadow_settings.dark_light	=	roi.dark_light;
	shadow_settings.threshold	=	roi.threshold;
	shadow_settings.enhance		=	false;

	shadow_obj.set( original_len + extend_len, shadow_settings );		// �`�N�o�䪺�]�w�O�[�W�������� ���M�|����

	// ����V
	seq_line_up		=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line_up[i].resize(size);
	seq_line_down	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line_down[i].resize(size);
	seq_line_left	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line_left[i].resize(size);
	seq_line_right	=	new bVectorByte[size];
	for( i = 0; i < size; i++ )
		seq_line_right[i].resize(size);

	line_data_up		=	new ShadowReturnData[size];
	for( i = 0; i < size; i++ )
		line_data_up[i]		=	ShadowReturnData( size );
	line_data_down		=	new ShadowReturnData[size];
	for( i = 0; i < size; i++ )
		line_data_down[i]	=	ShadowReturnData( size );
	line_data_left		=	new ShadowReturnData[size];
	for( i = 0; i < size; i++ )
		line_data_left[i]	=	ShadowReturnData( size );
	line_data_right		=	new ShadowReturnData[size];
	for( i = 0; i < size; i++ )
		line_data_right[i]	=	ShadowReturnData( size );

	up_map.resize(size,size);																															
	down_map.resize(size,size);
	left_map.resize(size,size);
	right_map.resize(size,size);

	shadow.resize(size,size);
	shadow2.resize(size,size);
	deep.resize(size,size);
	deep2.resize(size,size);

	// �o�ӳ]�w�����b�]�w�nroi����.
	set_square_bound();

}





/***************************************************************************************
	�P�_�O�_�� �� ��V
/****************************************************************************************/
bool	Emboss::is_bin_direction( const EMBOSS_DIRECTION direction )
{
	if( direction == EMBOSS_UP_DOWN || direction == EMBOSS_LEFT_RIGHT ||
		direction == EMBOSS_LU_RD || direction == EMBOSS_LD_RU )
			return	true;
	else
		return	false;
}




/***************************************************************************************
	�O�_�����V
/****************************************************************************************/
bool	Emboss::is_single_direction( const EMBOSS_DIRECTION direction )
{
	if( direction == EMBOSS_UP || direction == EMBOSS_DOWN ||
		direction == EMBOSS_LEFT || direction == EMBOSS_RIGHT ||
		direction == EMBOSS_LEFT_UP || direction == EMBOSS_LEFT_DOWN ||
		direction == EMBOSS_RIGHT_UP || direction == EMBOSS_RIGHT_DOWN )
			return	true;
	else
		return	false;
}






/***************************************************************************************
	�D������v�����
/****************************************************************************************/
void	Emboss::batch_roi_centroid()
{
	assert( shadow.size1() == shadow.size2() && shadow2.size1() == shadow2.size2() && shadow.size1() == shadow2.size1() && shadow.size1() == size );

	int			i,		j;
	double		sum,	msum;
	double		x,		y;
	double		x2,		y2;

	// �D���
	// x ��V
	sum		=	0;
	msum	=	0;
	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
		{
			if( shadow(i,j) == 1 )
			{
				sum		+=	deep(i,j);
				msum	+=	(/*0.5 +*/ i) * deep(i,j);
			}
		}
	if( sum == 0 )			x	=	-1;						// -1 ��ܨS���
	else					x	=	msum / sum;	

	// y ��V
	sum		=	0;
	msum	=	0;
	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
		{
			if( shadow(i,j) == 1 )
			{
				sum		+=	deep(i,j);
				msum	+=	(/*0.5 +*/ j) * deep(i,j);
			}
		}
	if( sum == 0 )			y	=	-1;						// -1 ��ܨS���
	else					y	=	msum / sum;	

	roi.em_data.centroid_x	=	x;							// �N��Ƽg�Jroi
	roi.em_data.centroid_y	=	y;
	roi.em_data.centroid_x2	=	-1;
	roi.em_data.centroid_y2 =	-1;

	// �p��зǮt
	//roi_centroid_deviation( roi );

	// �צV�ݭn�A���@��
	if( !is_vertical_horizontal( roi.direction) ) 
	{
		// �D���
		// x ��V
		sum		=	0;
		msum	=	0;
		for( i = 0; i < size; i++ )
			for( j = 0; j < size; j++ )
			{
				if( shadow2(i,j) == 1 )
				{
					sum		+=	deep2(i,j);
					msum	+=	(/*0.5 +*/ i) * deep2(i,j);
				}
			}
		if( sum == 0 )			x2	=	-1;						// -1 ��ܨS���
		else					x2	=	msum / sum;	

		// y ��V
		sum		=	0;
		msum	=	0;
		for( i = 0; i < size; i++ )
			for( j = 0; j < size; j++ )
			{
				if( shadow2(i,j) == 1 )
				{
					sum		+=	deep2(i,j);
					msum	+=	(/*0.5 +*/ j) * deep2(i,j);
				}
			}
		if( sum == 0 )			y2	=	-1;						// -1 ��ܨS���
		else					y2	=	msum / sum;	

		roi.em_data.centroid_x2	=	x2;							// �γo�˪��@�k�ӳB�z�צV
		roi.em_data.centroid_y2 =	y2;
	}

}





/***************************************************************************************
	�^�Ǹ��b���\�ϥ~�����v�Ӽ�
/****************************************************************************************/
int		Emboss::emboss_result_out_area()
{	
	const int			x				=	roi_x;
	const int			y				=	roi_y;

	int			i,	j;
	int			count;

	assert( shadow.size1() == shadow.size2() && shadow.size1() == allow_area.size1() && allow_area.size1() == allow_area.size2() );
	assert( shadow2.size1() == shadow2.size2() && shadow2.size1() == allow_area2.size1() && allow_area2.size1() == allow_area2.size2() );
	assert( shadow.size1() == shadow2.size1() );

	// �p�ƾ��k�s
	count	=	0;

	// �}�l�p�⸨�X���\�Ϫ����v�Ӽ�
	switch(direction)
	{
		case EMBOSS_UP :
		case EMBOSS_DOWN:
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					if( shadow(i,j) == SHADOW_VALUE && allow_area(i,j) == 0 )
						count ++;
				}
			break;

		case EMBOSS_LEFT_UP:
		case EMBOSS_LEFT_DOWN:
		// �o��ӳ��Υ��U����  �Ӱ�����
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					if( shadow(i,j) == SHADOW_VALUE && allow_area(i,j) == 0 )
						count ++;

					if( shadow2(i,j) == SHADOW_VALUE && allow_area2(i,j) == 0 )
						count ++;
				}
			break;

		case EMBOSS_RIGHT_UP:
		case EMBOSS_RIGHT_DOWN:
		// �o��ӭn�h�@��offset
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					if( shadow(i,j) == SHADOW_VALUE && allow_area(i,j) == 0 )
						count ++;
					if( shadow2(i,j) == SHADOW_VALUE && allow_area2(i,j) == 0 )
						count ++;
				}
			break;
	
		default:
			assert(false);
	}
	
	return	count;
}




/***************************************************************************************
	�P�_��߬O�_���b���\�ϥ~
/****************************************************************************************/
bool	Emboss::emboss_result_out_center()
{
	const double		cx		=	roi.em_data.centroid_x;
	const double		cy		=	roi.em_data.centroid_y;
	const double		cx2		=	roi.em_data.centroid_x2;
	const double		cy2		=	roi.em_data.centroid_y2;

	int		x,	y;

	assert( shadow.size1() == shadow.size2() && shadow.size1() == allow_area.size1() && allow_area.size1() == allow_area.size2() );
	assert( shadow2.size1() == shadow2.size2() && shadow2.size1() == allow_area2.size1() && allow_area2.size1() == allow_area2.size2() );
	assert( shadow.size1() == shadow2.size1() );

	// �P�_���
	switch(direction)
	{
		case EMBOSS_UP:
		case EMBOSS_DOWN:
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
			if( cx >= 0 && cy >= 0 )
			{
				x		=	(int) cx;
				y		=	(int) cy;

				if( allow_area( x, y ) == 1 )
					return	true;
				else
					return	false;
			}
			else	// -1, -1�o�˪��P�_�N��S��쳱�v �û����Otrue
				return	true;
			break;

		case EMBOSS_LEFT_UP:
		case EMBOSS_LEFT_DOWN:
			if( cx >= 0 && cy >= 0 && cx2 >= 0 && cy2 >= 0 )
			{
				x		=	(int) cx;
				y		=	(int) cy;

				if( allow_area( x, y ) == 1 )
					return	true;
				else
					return	false;

				x		=	(int) cx2;
				y		=	(int) cy2;

				if( allow_area2( x, y ) == 1 )
					return	true;
				else
					return	false;

			}
			else
				return	true;
			break;

		case EMBOSS_RIGHT_UP:
		case EMBOSS_RIGHT_DOWN:
			if( cx >= 0 && cy >= 0 && cx2 >= 0 && cy2 >= 0 )
			{
				x		=	(int) cx;
				y		=	(int) cy;

				if( allow_area( x, y ) == 1 )
					return	true;
				else
					return	false;

				x		=	(int) cx2;
				y		=	(int) cy2;

				if( allow_area2( x, y ) == 1 )
					return	true;
				else
					return	false;
			}
			return	true;
			break;

		default:
			assert(false);
	}

	return	false;
}


/***************************************************************************************
	���v�e�\�T�� 
	�P�_���X�ӳ��v (���@�ӴN�s���o�~)
/****************************************************************************************/
int		Emboss::all_dir_result()
{
	assert( shadow.size1() == size && shadow.size2() == size );

	int		i,	j;
	int		count;

	assert( direction == EMBOSS_ALL_DIRECTION );

	count	=	0;
	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
		{
			if( shadow( i, j ) == SHADOW_VALUE )
				count++;
		}
	
	return	count;
}


/***************************************************************************************
	�p�⳱�v�����G (���μo)
/****************************************************************************************/
void	Emboss::emboss_result( SquareData &square )
{
	// ��V
	if( is_single_direction(direction) )
	{
		square.em_data.is_center_out	=	emboss_result_out_center();		// ��߬O�_���b���\�Ϥ�   true ��� ���~  false ��ܼo�~
		square.em_data.out_shadow		=	emboss_result_out_area();		// �p�⸨�b���\�ϥ~�����v�Ӽ�
	}
	// ���v�e�\�T�� �o�䪺�@�k�O���L�����v�X�{�N�|�P�_���o�~
	// �|���N�e�\�T�Ϫ��P�_�W�ߥX�� !!
	else if( direction == EMBOSS_ALL_DIRECTION )
	{
		square.em_data.out_shadow		=	all_dir_result();
		square.em_data.is_center_out	=	true;		// �קﱼ�o�� �令���I�ƧP�_. �o�ӥû����Otrue
	}
	else
		assert(false);
}




/***************************************************************************************
	�p��edge
/****************************************************************************************/
void	Emboss::get_edge()
{
	int		i;

	for( i = 0; i < size; i++ )
	{
		edge(i)		=	1;
		edge2(i)	=	1;
	}
}



/***************************************************************************************
	�e�Xedge
/****************************************************************************************/
void	Emboss::result_output_edge( AndreaImage img )
{
	assert( shadow.size1() == shadow.size2()  );
	assert( shadow2.size1() == shadow2.size2()  );

	const int	x	=	roi_x;
	const int	y	=	roi_y;

	int		i,	j;

	switch(direction)
	{
		case EMBOSS_UP :
		case EMBOSS_DOWN:
		case EMBOSS_LEFT:
		case EMBOSS_RIGHT:
		case EMBOSS_UP_DOWN:
		case EMBOSS_LEFT_RIGHT:
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X���v
					if( shadow(i,j) == SHADOW_VALUE )
						img.gray_channel( i + x, j + y, 31 );
						//img.pixel_channel( i + x, j + y, 0, 255, 255 );
				}
			break;

		case EMBOSS_LEFT_UP:
		case EMBOSS_LEFT_DOWN:
		case EMBOSS_LU_RD:
		case EMBOSS_LD_RU:
		// �o��ӳ��Υ��U����  �Ӱ�����
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// �e�X���v
					if( shadow(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j, 31 );
						//img.pixel_channel( x_left + i + j, (y_up+y_down)/2 + i - j, 0, 255, 255 );

					if( shadow2(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, 31 );
						//img.pixel_channel( x_left + i + j, (y_up+y_down)/2 + i - j + 1, 0, 255, 255 );		// (�T�w���� y+1 �� offset)

				}
			break;

		case EMBOSS_RIGHT_UP:
		case EMBOSS_RIGHT_DOWN:
		// �o��ӭn�h�@��offset
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					// ���X���v
					if( shadow(i,j) == SHADOW_VALUE )
						img.gray_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j, 31 );
						//img.pixel_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j, 0, 255, 255 );
					                                           
					if( shadow2(i,j) == SHADOW_VALUE )
											           /*�d�N�o�䪺�p��*/
						img.gray_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j + 1, 31 );
						//img.pixel_channel( x_left + i + j + 1 + (1+size)%2, (y_up+y_down)/2 + i - j + 1, 0, 255, 255 );		// (�T�w���� y+1 �� offset)

				}
			break;
		
		case EMBOSS_ALL_DIRECTION:
			// �u�n�X�{���v���� �S��mask���[��
			for( i = 0; i < size; i++ )
				for( j = 0; j < size; j++ )
				{
					if( shadow( i, j ) == SHADOW_VALUE )
						img.gray_channel( x + i, y + j , 31 );
						//img.pixel_channel( x + i, y + j , 0, 255, 255 );
				}

			break;


		default:
			assert(false);
	}
}



/***************************************************************************************
	�۷��쥻��execution  
	multi-thread�ɨϥ�
/****************************************************************************************/
void	Emboss::exec()
{
	int		i;

	if( direction == EMBOSS_ALL_DIRECTION )
	{
		for( i = 0; i < size; i++ )
		{
			batch_shadow_ALLD(i);
		}
		return;
	}

	// ��V
	for( i = 0; i < size; i++ )
		batch_shadow_line(i);

	// �צV �h�@���Ŷ�
	if( direction >= EMBOSS_LEFT_UP && direction <= EMBOSS_RIGHT_DOWN )	// ���F�[�t �p��enum value
	{
		for( i = 0; i < size; i++ )
			batch_shadow_line_2(i);
	}

	// �p��edge
	get_edge();

}













//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ KernelDensity ���� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/***************************************************************************************
	�غc���
/****************************************************************************************/
KernelDensity::KernelDensity()
{
	kernel_square(1);
}




/***************************************************************************************
	���Ͱ򥻪������ kernel
/****************************************************************************************/
void	KernelDensity::kernel_square( int n )
{
	int		m	=	2*n + 1;

	kernel.resize( m, m );

	int		i,	j;

	for( i = 0; i < m; i++ )
		for( j = 0; j < m; j++ )
			kernel( i, j )	=	1.0;
}



/***************************************************************************************
	�����Ӽƭp��
/****************************************************************************************/
int		KernelDensity::element_count( bMatrixByte &data )
{
	assert( data.size1() == data.size2() && data.size1() > 0 );

	int		i,	j;
	int		N	=	0;

	for( i = 0; i < data.size1(); i++ )
		for( j = 0; j < data.size2(); j++ )
		{
			if( data( i, j ) == 1 )
				N++;
		}

	return	N;
}



/***************************************************************************************
	�����ƭp�⪺�a��
/****************************************************************************************/
void	KernelDensity::dosomething( bMatrixDouble &p_data,  bMatrixByte &data )
{
	assert( data.size1() == data.size2() && data.size1() == p_data.size1() && p_data.size1() == p_data.size2() && data.size1() > 0 );
	assert( data.size1() >= 3 );

	int		i,	j,	x,	y;
	double	dcount;
		
	p_data.clear();		// �@�w�n�M�����.

	// �}�l���y����
	for( i = 0; i < data.size1(); i++ )
		for( j = 0; j < data.size2(); j++ )
		{
			dcount	=	0;
			for( x = -1; x <= 1; x++ )
				for( y = -1; y <= 1; y++ )
				{
					if( i+x >= 0 && j+y >= 0 && i+x < data.size1() && j+y < data.size2() )
						dcount	+=	data( i+x, j+y ) * kernel( x+1, y+1 );
				}
			dcount	/=	(9.0);						// 9.0 = h^D
			p_data( i, j )	=	dcount ;			// �קKwindows�j�p�z�Z���G
		}
}






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Emboss Multi-thread ���� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/***************************************************************************************
	�غc
/****************************************************************************************/
Emboss_MT::Emboss_MT()
{
	em		=	NULL;
	size	=	0;
}


/***************************************************************************************
	�Ѻc
/****************************************************************************************/
Emboss_MT::~Emboss_MT()
{
	delete	[] em;
	size	=	0;
}


/***************************************************************************************
	��l��
/****************************************************************************************/
void	Emboss_MT::init( int _size )
{
	assert( _size >= 0 );

	delete	[] em;	// ���N�¸�ƧR��

	size	=	_size;
	em		=	new Emboss[size];
}

/***************************************************************************************
	�����Ѻc
	�N��ƧR��
/****************************************************************************************/
void	Emboss_MT::destroy()
{
	size	=	0;
	delete	[] em;
	em	=	NULL;
}



/***************************************************************************************
	�аOdefect map, 
	�ϥ�openMP
/****************************************************************************************/
void	Emboss_MT::defect( AndreaImage img, int value )
{

#pragma omp parallel for
	for( int i = 0; i < size; i++ )
	{
		if( em[i].good_bad == EMBOSS_BAD )
			em[i].batch_result_output_emboss_defect( img, value );
	}

}



/***************************************************************************************
	���� 
	(�b�o��i��openMP)
/****************************************************************************************/
void	Emboss_MT::exec( AndreaImage image )
{
	//omp_set_num_threads(MAX_THR);
#pragma omp parallel for
	for( int i = 0; i < size; i++ )
	{
		em[i].set_image( image );			// �]�w�Ϥ�
		em[i].batch_loadimage();			// Ū���Ϥ�
		// �W�����  72 * 84*84  �ݭn�ɶ� = 0.63ms  

		em[i].exec();						// ����
		// �o��O�Y�̦h�ɶ�������

		em[i].batch_linedata2matrix();		// �ഫ�y��
		em[i].batch_kernel_density();		// �L�o���T
		// �o�䪺�ɶ����� (3ms)

		em[i].batch_roi_centroid();			// �p����
	}
}


/***************************************************************************************
	���� 
	(�b�o��i��openMP)
/****************************************************************************************/
void	Emboss_MT::exec_page2( AndreaImage image )
{
	// �]�w�Ϥ�
	for( int i = 0; i < size; i++ )
	{
		em[i].set_image( image );
		em[i].batch_loadimage();		// Ū���Ϥ�
		// �W�����  72 * 84*84  �ݭn�ɶ� = 0.63ms  

		em[i].exec();
		// �o��O�Y�̦h�ɶ�������

		em[i].batch_linedata2matrix();
		em[i].batch_kernel_density();

		em[i].batch_roi_centroid();			// �p����
	}
	// �o�䪺�ɶ����� (3ms)
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Screen Multi-thread ���� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/***************************************************************************************
	�غc
/****************************************************************************************/
Screen_MT::Screen_MT()
{
	sc		=	NULL;
	size	=	0;
}


/***************************************************************************************
	�Ѻc
/****************************************************************************************/
Screen_MT::~Screen_MT()
{
	delete	[] sc;
	size	=	0;
}


/***************************************************************************************
	��l��
/****************************************************************************************/
void	Screen_MT::init( int _size )
{
	assert( _size >= 0 );

	delete	[] sc;		// ���N�ª���ƲM��

	size	=	_size;
	sc		=	new Screen[size];
}


/***************************************************************************************
	�����Ѻc
	�N��ƧR��
/****************************************************************************************/
void	Screen_MT::destroy()
{
	size	=	0;
	delete	[] sc;
	sc		=	NULL;
}


/***************************************************************************************
	���� 
	(�b�o��i��openMP)
/****************************************************************************************/
void	Screen_MT::exec( AndreaImage image )
{
//	omp_set_num_threads(MAX_THR);


#pragma omp parallel for
	for( int i = 0; i < size; i++ )
	{
		sc[i].set_image( image );			// �]�w�Ϥ�
		sc[i].batch_loadimage();			// Ū���Ϥ�

		sc[i].exec();						// ����
		// �o��O�Y�̦h�ɶ�������

		sc[i].batch_linedata2matrix();		// �y���ഫ
	}
}


/***************************************************************************************
	�аOdefect map, 
	�ϥ�openMP
/****************************************************************************************/
void	Screen_MT::defect( AndreaImage img, int value )
{

#pragma omp parallel for
	for( int i = 0; i < size; i++ )
	{
		if( sc[i].good_bad == EMBOSS_BAD )
			sc[i].batch_result_output_screen_defect( img, value );
	}

}


/***************************************************************************************
	Page2�ϥΪ�����
/****************************************************************************************/
void	Screen_MT::exec_page2( AndreaImage image )
{
	for( int i = 0; i < size; i++ )
	{
		// �]�w�Ϥ�
		sc[i].set_image( image );

		// Ū���Ϥ�
		sc[i].batch_loadimage();

		sc[i].exec();
		// �o��O�Y�̦h�ɶ�������

		sc[i].batch_linedata2matrix();
	}

}






} // end namespace emboss