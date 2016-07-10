
#include "channel.h"

using namespace std;

#if 0


/***************************************************************************************
	�غc
/****************************************************************************************/
#ifdef _OPENPIV_
Channel::Channel( Settings *settingsPass, DataContainer *filedataPass )
	:	AndreaEngine()
{
	progress		=	settingsPass->getProgress();	
	settings		=	settingsPass;
	filedata		=	filedataPass;

	// ���v�˴�
	square_data		=	NULL;
	square_data2	=	NULL;

    mutex.lock();
    mutex.unlock();
}
#else
Channel::Channel()
{
	// ���v�˴�
	square_data		=	NULL;
	square_data2	=	NULL;
}
#endif


/***************************************************************************************
	�Ѻc
/****************************************************************************************/
Channel::~Channel()
{
	delete	[]	square_data;
	delete	[]	square_data2;
}








/***************************************************************************************
	���v�˴��Ϊ�
	�@���ˬd�@���u
/****************************************************************************************/
ShadowReturnData	Channel::shadow_line( PIXEL start, PIXEL end )
{
	int		i;
	int		len	=	30;		// ���e����pixel�e��

	bVectorInt			seq_line	=	loadimage( start, end );		// ��l���ɸ�� (line)

	int		ct	=	seq_line.size();

	bVectorInt			seq(ct + len);									// ��ڤW��i�h����� �|���ɭȪ��ʧ@
	ShadowReturnData	shadow_data_tmp,	shadow_data;				// ���v���
	ShadowSettings		shadow_settings;								// ���v�]�w��

	// ��l��vector���
	shadow_data				=	ShadowReturnData( ct );

	// �N��l��ƶ�i�h
	shadow_data.raw_data	=	seq_line;
	
	// ���e�ɭ�
	for( i = 0; i < len; i++ )
		seq(i)	=	seq_line(0);             //(int)(seqttt(0)+0.5);   

	// ���^�쥻�����
	for( i = len; i < len+ct; i++ )
		seq(i)	=	seq_line(i-len);		//(int)(seqttt(i-len)+0.5);		// �|�ˤ��J

	shadow_settings.inhence_mode	=	settings->inhence_mode;			// �]�w���v���
	shadow_settings.dark_light		=	settings->dark_light;			// �]�w��G�v�γ��v
	shadow_data_tmp					=	shadow_detector( seq, threshold, seq, shadow_settings );		// ���v��� �]�t�ɭȱo����  ��ӰѼ�  �@�ӬO��l���  �@�ӬO�i�H�[�Wgauss blur�����

	// �R���ɭȪ�����
	for( i = len; i < len+ct; i++ )
	{
		shadow_data.mono_sub_seq( i-len )	=	shadow_data_tmp.mono_sub_seq(i);
		shadow_data.shadow_seq( i-len )		=	shadow_data_tmp.shadow_seq(i);
		shadow_data.shadow_depth( i-len )	=	shadow_data_tmp.shadow_depth(i);
		shadow_data.edge					=	shadow_data_tmp.edge - len;
		shadow_data.deepest					=	shadow_data_tmp.deepest - len ;
	}

	return	shadow_data;
}




/***************************************************************************************
	���v�˴��Ϊ�
	�@���ˬd�@�Ӱ϶�
/****************************************************************************************/
SquareData		Channel::shadow_square( PIXEL square_roi )
{
	int		x_left,	x_right,	y_up,	y_down;

	// �W�U���k�|��V
	if( direction == OpenPIV::up || direction == OpenPIV::down ||
		direction == OpenPIV::left || direction == OpenPIV::right )
	{
		x_left	=	square_roi.x;								// x������_�l�I
		x_right	=	square_roi.x + square_roi.square_size-1;	// x���k����I
		y_up	=	square_roi.y;								// y���W��_�l�I
		y_down	=	square_roi.y + square_roi.square_size-1;	// y���U����I
	}
	// ���W���U�k�W�k�U�|�Ӥ�V  �o��|�y�L���I�n �Ȯɥ����B�z
	else
	{
		x_left	=	square_roi.x - square_roi.square_size/2;							// x������_�l�I
		x_right	=	square_roi.x + square_roi.square_size/2 + square_roi.square_size;	// x���k����I
		y_up	=	square_roi.y - square_roi.square_size/2;							// y���W��_�l�I
		y_down	=	square_roi.y + square_roi.square_size/2 + square_roi.square_size;							// y���U����I
	}

	PIXEL				p1,	p2;
	int					i;

	SquareData		return_data;			// �^�Ǹ��

	// ��l�� squeare data	�@�q����. ( x, y �bswitch���a��~��l�� )
	return_data.direction	=	direction;
	return_data.size		=	square_roi.square_size;
	return_data.x			=	0;
	return_data.y			=	0;
	return_data.line_data	=	new		ShadowReturnData[ return_data.size ];

	switch( direction )
	{
		case OpenPIV::up :
			return_data.x	=	x_left;
			return_data.y	=	y_up;
			for( i = 0; i < square_roi.square_size; i++ )
			{
				p1.x	=	x_left + i;
				p2.x	=	x_left + i;
				p1.y	=	y_up;
				p2.y	=	y_down;
				return_data.line_data[i]	=	shadow_line( p1, p2 );
			}
			break;

		case OpenPIV::down :
			return_data.x	=	x_left;
			return_data.y	=	y_down;
			for( i = 0; i < square_roi.square_size; i++ )
			{
				p1.x	=	x_left + i;
				p2.x	=	x_left + i;
				p1.y	=	y_down;
				p2.y	=	y_up;
				return_data.line_data[i]	=	shadow_line( p1, p2 );
			}
			break;

		case OpenPIV::left :
			return_data.x	=	x_left;
			return_data.y	=	y_up;
			for( i = 0; i < square_roi.square_size; i++ )
			{
				p1.x	=	x_left;
				p2.x	=	x_right;
				p1.y	=	y_up + i;
				p2.y	=	y_up + i;
				return_data.line_data[i]	=	shadow_line( p1, p2 );
			}
			break;

		case OpenPIV::right :
			return_data.x	=	x_right;
			return_data.y	=	y_up;
			for( i = 0; i < square_roi.square_size; i++ )
			{
				p1.x	=	x_right;
				p2.x	=	x_left;
				p1.y	=	y_up + i;
				p2.y	=	y_up + i;
				return_data.line_data[i]	=	shadow_line( p1, p2 );
			}
			break;

		case OpenPIV::left_up :
			// �o��|�W�L���X�˴���  �n�p�� 
			// �P�_�覡�|��W�U���k���������Ǥ��P
			return_data.x	=	x_left;
			return_data.y	=	(y_up + y_down)/2;
			for( i = 0; i < square_roi.square_size; i++ )
			{
				p1.x	=	return_data.x + i;
				p2.x	=	return_data.x + i + square_roi.square_size - 1;
				p1.y	=	return_data.y - i;
				p2.y	=	return_data.y - i + square_roi.square_size - 1;
				return_data.line_data[i]	=	shadow_line( p1, p2 );
			}
			break;
			
		case OpenPIV::left_down :
			return_data.x	=	x_left;
			return_data.y	=	(y_up + y_down)/2;
			for( i = 0; i < square_roi.square_size; i++ )
			{
				p1.x	=	return_data.x + i;
				p2.x	=	return_data.x + i + square_roi.square_size - 1;
				p1.y	=	return_data.y + i;
				p2.y	=	return_data.y + i + square_roi.square_size - 1;
				return_data.line_data[i]	=	shadow_line( p1, p2 );
			}
			break;

		case OpenPIV::right_up :
			return_data.x	=	x_right;
			return_data.y	=	(y_up + y_down)/2;
			for( i = 0; i < square_roi.square_size; i++ )
			{
				p1.x	=	return_data.x - i;
				p2.x	=	return_data.x - i - square_roi.square_size - 1;
				p1.y	=	return_data.y - i;
				p2.y	=	return_data.y - i - square_roi.square_size - 1;
				return_data.line_data[i]	=	shadow_line( p1, p2 );
			}
			break;

		case OpenPIV::right_down :
			return_data.x	=	x_right;
			return_data.y	=	(y_up + y_down)/2;
			for( i = 0; i < square_roi.square_size; i++ )
			{
				p1.x	=	return_data.x - i;
				p2.x	=	return_data.x - i - square_roi.square_size - 1;
				p1.y	=	return_data.y + i;
				p2.y	=	return_data.y + i - square_roi.square_size - 1;
				return_data.line_data[i]	=	shadow_line( p1, p2 );
			}
			break;

		default:
			progress->debuger( "error!!", QColor( 255, 0, 0 ) );
			break;
	}

	return		return_data;
}



/***************************************************************************************
	���v�˴��Ϊ�
	Ū�����ɸ��
/****************************************************************************************/
bVectorInt	Channel::loadimage( PIXEL start, PIXEL end )
{
	bVectorInt	seq;
	int			delta;
	int			i;

	// �W�賱�v
	if( direction == OpenPIV::up )
	{
		seq.resize( end.y - start.y + 1 );
		seq.clear();

		for( i = 0; i < seq.size(); i++ )
			seq( i )	=	image.gray_channel( start.x, start.y + i );
	}

	// �U�賱�v
	else if( direction == OpenPIV::down )
	{
		seq.resize( start.y - end.y + 1 );
		seq.clear();

		for( i = 0; i < seq.size(); i++ )
			seq( i )	=	image.gray_channel( start.x, start.y - i );
	}

	// ���䳱�v
	else if( direction == OpenPIV::left )
	{
		seq.resize( end.x - start.x + 1 );
		seq.clear();

		for( i = 0; i < seq.size(); i++ )
			seq( i )	=	image.gray_channel( start.x + i, start.y );
	}

	// �k�䳱�v
	else if( direction == OpenPIV::right )
	{
		seq.resize( start.x - end.x + 1 );
		seq.clear();

		for( i = 0; i < seq.size(); i++ )
			seq( i )	=	image.gray_channel( start.x - i, start.y );
	}

	// ���W ���v   (���W->�k�U)
	else if( direction == OpenPIV::left_up )
	{
		seq.resize( end.x - start.x + 1 );
		seq.clear();

		for( i = 0; i < seq.size(); i++ )
			seq( i )	=	image.gray_channel( start.x + i, start.y + i );
	}

	// ���U���v   (���U->�k�W)
	else if( direction == OpenPIV::left_down )
	{
		seq.resize( end.x - start.x + 1 );
		seq.clear();

		for( i = 0; i < seq.size(); i++ )
			seq( i )	=	image.gray_channel( start.x + i, start.y - i );
	}

	// �k�W���v
	else if( direction == OpenPIV::right_up )
	{
		seq.resize( start.x - end.x + 1 );
		seq.clear();

		for( i = 0; i < seq.size(); i++ )
			seq(i)	=	image.gray_channel( start.x - i, start.y + i );
	}

	// �k�U���v
	else if( direction == OpenPIV::right_down )
	{
		seq.resize( start.x - end.x + 1 );
		seq.clear();

		for( i = 0; i < seq.size(); i++ )
			seq(i)	=	image.gray_channel( start.x - i, start.y - i );
	}
	
	else
		progress->debuger( QString("error!!"), QColor(255,0,0) );

	return	seq;
}




/***************************************************************************************
	�N���G�аO�b�Ϥ��W ���
/****************************************************************************************/
void	Channel::mark_on_image( SquareData square )
{
	int		i,	j;
	int		x,	y;
	int		deepest;
	int		edge;

	// �W�賱�v
	if( direction == OpenPIV::up )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			x	=	square.x + i;
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					y	=	square.y + j;
					final.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				y	=	square.y + deepest;
				final.pixel_channel( x, y, 0, 255, 255 );
			}

			// edge �аO����
			edge	=	square.line_data[i].edge;
			y		=	square.y + edge;
			final.pixel_channel( x, y, 255, 0, 0 );
		}
	}

	// �U�賱�v
	else if( direction == OpenPIV::down )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			x	=	square.x + i;
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					y	=	square.y - j;
					final.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				y	=	square.y - deepest;
				final.pixel_channel( x, y, 0, 255, 255 );
			}
			edge	=	square.line_data[i].edge;
			y		=	square.y - edge;
			final.pixel_channel( x, y, 255, 0, 0 );
		}
	}

	// ���䳱�v
	else if( direction == OpenPIV::left )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			y	=	square.y + i;
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x + j;
					final.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x + deepest;
				final.pixel_channel( x, y, 0, 255, 255 );
			}
		}
	}

	// �k�䳱�v
	else if( direction == OpenPIV::right )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			y	=	square.y + i;
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x - j;
					final.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x - deepest;
				final.pixel_channel( x, y, 0, 255, 255 );
			}
		}
	}

	// ���W���v
	else if( direction == OpenPIV::left_up )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x + i + j;
					y	=	square.y - i + j;
					final.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x + i + deepest;
				y	=	square.y - i + deepest;
				final.pixel_channel( x, y, 0, 255, 255 );
			}
		}
	}

	// ���U���v
	else if( direction == OpenPIV::left_down )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x + i + j;
					y	=	square.y + i - j;
					final.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x + i + deepest;
				y	=	square.y + i - deepest;
				final.pixel_channel( x, y, 0, 255, 255 );
			}
		}
	}

	// �k�W���v
	else if( direction == OpenPIV::right_up )
	{
		for( i = 0; i < square.size; i++ )
		{
			for( j = 0; j < square.size; j++ )
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x - i - j;
					y	=	square.y - i + j;
					final.pixel_channel( x, y, 0, 0, 255 );
				}

				deepest		=	square.line_data[i].deepest;
				if( square.line_data[i].shadow_seq[ deepest ] == 1 )
				{
					x	=	square.x - i - deepest;
					y	=	square.y - i + deepest;
					final.pixel_channel( x, y, 0, 255, 255 );
				}
			}
		}
	}

	// �k�U���v
	else if( direction == OpenPIV::right_down )
	{
		for( i = 0; i < square.size; i++ )
		{
			for( j = 0; j < square.size; j++ )
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x - i - j;
					y	=	square.y + i - j;
					final.pixel_channel( x, y, 0, 0, 255 );
				}

				deepest		=	square.line_data[i].deepest;
				if( square.line_data[i].shadow_seq[ deepest ] == 1 )
				{
					x	=	square.x - i - deepest;
					y	=	square.y + i - deepest;
					final.pixel_channel( x, y, 0, 255, 255 );
				}
			}
		}
	}

	else
		progress->debuger( QString("error!!"), QColor( 255, 0, 0 ) );
}





/***************************************************************************************
	�N���G�аO�b�Ϥ��W ���
/****************************************************************************************/
void	Channel::mark_on_image( SquareData square, QImage &img )
{
	int		i,	j;
	int		x,	y;
	int		deepest;
	int		edge;

	// �W�賱�v
	if( direction == OpenPIV::up )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			x	=	square.x + i;
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					y	=	square.y + j;
					img.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				y	=	square.y + deepest;
				img.pixel_channel( x, y, 0, 255, 255 );
			}

			// edge �аO����
			edge	=	square.line_data[i].edge;
			y		=	square.y + edge;
			img.pixel_channel( x, y, 255, 0, 0 );
		}
	}

	// �U�賱�v
	else if( direction == OpenPIV::down )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			x	=	square.x + i;
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					y	=	square.y - j;
					img.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				y	=	square.y - deepest;
				img.pixel_channel( x, y, 0, 255, 255 );
			}

			// edge �аO����
			edge	=	square.line_data[i].edge;
			y		=	square.y - edge;
			img.pixel_channel( x, y, 255, 0, 0 );
		}
	}

	// ���䳱�v
	else if( direction == OpenPIV::left )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			y	=	square.y + i;
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x + j;
					img.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x + deepest;
				img.pixel_channel( x, y, 0, 255, 255 );
			}

			// edge �аO����
			edge	=	square.line_data[i].edge;
			x		=	square.x + edge;
			img.pixel_channel( x, y, 255, 0, 0 );
		}
	}

	// �k�䳱�v
	else if( direction == OpenPIV::right )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			y	=	square.y + i;
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x - j;
					img.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x - deepest;
				img.pixel_channel( x, y, 0, 255, 255 );
			}

			// edge �аO����
			edge	=	square.line_data[i].edge;
			x		=	square.x - edge;
			img.pixel_channel( x, y, 255, 0, 0 );
		}
	}

	// ���W���v
	else if( direction == OpenPIV::left_up )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x + i + j;
					y	=	square.y - i + j;
					img.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x + i + deepest;
				y	=	square.y - i + deepest;
				img.pixel_channel( x, y, 0, 255, 255 );
			}

			// edge �аO����
			edge	=	square.line_data[i].edge;
			x		=	square.x + i + edge;
			y		=	square.y - i + edge;
			img.pixel_channel( x, y, 255, 0, 0 );
		}
	}

	// ���U���v
	else if( direction == OpenPIV::left_down )
	{
		for( i = 0; i < square.size; i++ )		// �H�K�@�ӳ��i�H  �ҥH�D0  �̦w��
		{
			for( j = 0; j < square.size; j++ )	// �]���O����� �ҥH�i�H�o�˰� ����έn�ק�
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x + i + j;
					y	=	square.y + i - j;
					img.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			// �̲`���аO�t�@���C��
			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x + i + deepest;
				y	=	square.y + i - deepest;
				img.pixel_channel( x, y, 0, 255, 255 );
			}
			// edge �аO����
			edge	=	square.line_data[i].edge;
			x		=	square.x + i + edge;
			y		=	square.y + i - edge;
			img.pixel_channel( x, y, 255, 0, 0 );
		}
	}

	// �k�W���v
	else if( direction == OpenPIV::right_up )
	{
		for( i = 0; i < square.size; i++ )
		{
			for( j = 0; j < square.size; j++ )
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x - i - j;
					y	=	square.y - i + j;
					img.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x - i - deepest;
				y	=	square.y - i + deepest;
				img.pixel_channel( x, y, 0, 255, 255 );
			}

			// edge �аO����
			edge	=	square.line_data[i].edge;
			x		=	square.x - i - edge;
			y		=	square.y - i + edge;
			img.pixel_channel( x, y, 255, 0, 0 );
	
		}
	}

	// �k�U���v
	else if( direction == OpenPIV::right_down )
	{
		for( i = 0; i < square.size; i++ )
		{
			for( j = 0; j < square.size; j++ )
			{
				if( square.line_data[i].shadow_seq[j] == 1 )
				{
					x	=	square.x - i - j;
					y	=	square.y + i - j;
					img.pixel_channel( x, y, 0, 0, 255 );
				}
			}

			deepest		=	square.line_data[i].deepest;
			if( square.line_data[i].shadow_seq[ deepest ] == 1 )
			{
				x	=	square.x - i - deepest;
				y	=	square.y + i - deepest;
				img.pixel_channel( x, y, 0, 255, 255 );
			}

			// edge �аO����
			edge	=	square.line_data[i].edge;
			x		=	square.x - i - edge;
			y		=	square.y + i - edge;
			img.pixel_channel( x, y, 255, 0, 0 );
			
		}
	}

	else
		progress->debuger( QString("error!!"), QColor( 255, 0, 0 ) );
}



/***************************************************************************************
	�p���ߦ�m
	�s���� = �n���ᰣ�W����
	����	 = �[�_�Ӱ��W����
/****************************************************************************************/
double	Channel::centroid( bVectorInt deep, int start, int end )
{
	int		i;
	double	xsum	=	0;
	double	sum		=	0;
	double	mag;

	for( i = start; i < end; i++ )
	{
		xsum	+=	i * deep(i);
		sum		+=	deep(i);
	}
	mag	=	xsum / sum;

	return	mag;
}







#ifdef _OPENPIV_
/***************************************************************************************
	��ڰ���{���X����m
/****************************************************************************************/
void	Channel::dosomething()
{
//Timer	t;
//t.Start();

	progress->debuger("shadow detector...");

	//QFile::remove( QString("./output/test.txt") ) ;    �R���ɮ׽d��
	//QProcess	prc;
	//prc.start("DEL ./output/*.txt");
	//system("DEL ./output/1.txt");

	bVectorInt	raw;		// ��l���

	int			i,	j,	k;
	int			x,	y;
	int			istart,	iend;
	double		dtmp;

	char	str[100];

	// ��l�Ƹ��
	image			=	filedata->get_image();
//	image			=	QImage( str );
	final			=	image;
	QRgb	rgb		=	settings->getRGB();

	// Ū���˴���
	square			=	filedata->getPixelList();
	threshold		=	settings->shadow_depth_threshold;		// �`�ת�threshold
	direction		=	settings->shadow_dir;					// ��V

	// ��l�� square data
	square_data		=	new		SquareData[square.size()];

	// �׫Ѥ~�ݭn��l�� square_dat2
	if( direction == OpenPIV::left_down || direction == OpenPIV::left_up ||
		direction == OpenPIV::right_down || direction == OpenPIV::right_up )
			square_data2	=	new		SquareData[square.size()];

	// �w��C���˴��ϥh�������v
	for( i = 0; i < square.size(); i++ )
	{
		// �@���˴��@�Ӱ϶�
		square_data[i]	=	shadow_square( square.at(i) );

		// �צV�����h���@��  (�ثe���W�U)
		// ���W & �k�W
		if( direction == OpenPIV::left_up || direction == OpenPIV::right_up )
		{
			PIXEL	square_tmp	=	square.at(i);		// �W�r�s�� PIXEL �u�O�]����K�i�o�ﵲ�c�W�� XD   ����O�o�קﱼ
			square_tmp.y--;
			square_data2[i]	=	shadow_square( square_tmp );
		}
		// ���U & �k�U
		else if( direction == OpenPIV::left_down || direction == OpenPIV::right_down )
		{
			PIXEL	square_tmp	=	square.at(i);
			square_tmp.y++;
			square_data2[i]	=	shadow_square( square_tmp );
		}

	}


	// �N��ƼаO�b�Ϥ��W
	for( i = 0; i < square.size(); i++ )
		mark_on_image( square_data[i] );

	if( direction == OpenPIV::left_down || direction == OpenPIV::left_up ||
		direction == OpenPIV::right_down || direction == OpenPIV::right_up )
	{
		for( i = 0; i < square.size(); i++ )
			mark_on_image( square_data2[i] );
	}
	final.save("output_1.bmp");


	// �Q��DensityKernel �o�����T
	KernelDensity	kd,		kd2;
	bMatrixDouble	bm_tmp,	bm_tmp2;
	QImage			final2	=	image;

	for( i = 0; i < square.size(); i++ )
	{
		kd.set_data( square_data[i] );
		bm_tmp	=	kd.dosomething();

		if( direction == OpenPIV::left_down || direction == OpenPIV::left_up ||
			direction == OpenPIV::right_down || direction == OpenPIV::right_up )
		{
			kd2.set_data( square_data2[i] );
			bm_tmp2	=	kd2.dosomething();
		}

		// �N�o�����v����ưf�V�]�w�^�hsquare_data
		for( j = 0; j < square_data[i].size; j++ )
			for( k = 0; k < square_data[i].size; k++ )
			{
				// �o��ĥΨ��涰���@�k (���I�ܤ� �o��noise�y����)
				if( bm_tmp( j, k ) < 1.5/9.0 )	// �u���@���I���L�o��
					square_data[i].line_data[j].shadow_seq(k)	=	0;

				/*if( bm_tmp( j, k ) > 1.5/9.0 )	// �u���@���I���L�o��
					square_data[i].line_data[j].shadow_seq(k)	=	1;
				else
					square_data[i].line_data[j].shadow_seq(k)	=	0;*/

				if( direction == OpenPIV::left_down || direction == OpenPIV::left_up ||
					direction == OpenPIV::right_down || direction == OpenPIV::right_up )
				{
					if( bm_tmp2( j, k ) < 1.5/9.0 )	// �u���@���I���L�o��
						square_data2[i].line_data[j].shadow_seq(k)	=	0;
				}

			}

		// �N���G��X
		//mark_on_image( square_data[i], final2 );
	}
	




	// ����edge
	for( i = 0; i < square.size(); i++ )
	{
		for( j = 0; j < square_data[i].size; j++ )
		{
			raw									=	fillup_shadow( square_data[i].line_data[j].shadow_seq, square_data[i].line_data[j].raw_data );		// ���o�ɭȫ᪺��l���
			square_data[i].line_data[j].edge	=	find_edge( raw );																					// ���o edge ��m

			//square_data[i].line_data[j].edge	=	find_edge( square_data[i].line_data[j].shadow_seq, square_data[i].line_data[j].raw_data );

			if( direction == OpenPIV::left_down || direction == OpenPIV::left_up ||
				direction == OpenPIV::right_down || direction == OpenPIV::right_up )
			{
				raw									=	fillup_shadow( square_data2[i].line_data[j].shadow_seq,  square_data2[i].line_data[j].raw_data );
				square_data2[i].line_data[j].edge	=	find_edge( raw );
				//square_data2[i].line_data[j].edge	=	find_edge( square_data[i].line_data[j].shadow_seq, square_data[i].line_data[j].raw_data );
			}
		}
		mark_on_image( square_data[i], final2 );
		if( direction == OpenPIV::left_down || direction == OpenPIV::left_up ||
			direction == OpenPIV::right_down || direction == OpenPIV::right_up )
				mark_on_image( square_data2[i], final2 );
	}


	// �}�l�p�⳱�v��m
	for( i = 0; i < square.size(); i++ )
	{
		for( j = 0; j < square_data[i].size; j++ )
		{
			square_data[i].line_data[j].edge_distance.resize(0);

			// �j�M�@�q���v��
			istart	=	0;
			iend	=	1;
			while(1)
			{
				//if( istart >= square_data[i].line_data[j].shadow_seq.size() )		break;

				// �j�M�X�@�q���v�϶�
				while( istart < square_data[i].line_data[j].shadow_seq.size() && square_data[i].line_data[j].shadow_seq( istart ) == 0 )		istart++;
				iend	=	istart+1;
				while( iend <  square_data[i].line_data[j].shadow_seq.size()  && square_data[i].line_data[j].shadow_seq( iend ) == 1  )			iend++;

				if( istart >= square_data[i].line_data[j].shadow_seq.size() )		break;

				// ��쳱�v�q �h�p���ߦ�m
				dtmp	=	centroid( square_data[i].line_data[j].shadow_depth, istart, iend ) - square_data[i].line_data[j].edge;
				square_data[i].line_data[j].edge_distance.push_back( dtmp );

				istart	=	iend + 1;
			}
		}
	}


	// ��X���v
	boost::container::vector<double>	arr;
	for( i = 0; i < square.size(); i++ )
	{
		if( settings->batch_check )
			sprintf( str, "./output/%06d.txt", i );
		else
			sprintf( str, "./output/output.txt");
		ofstream	out( str, ios::app );

		if( settings->id == 0 )
		{
			out << "square " << i << "\n\nsize = " << square_data[0].size 
				<< "\nx = " << square[i].x << ", " << square[i].y 
				<< "\nthreshold = " << settings->shadow_depth_threshold << "\n\n\n";
		}

		arr.clear();

		for( j = 0; j < square_data[i].size; j++ )
		{

			for( k = 0; k < square_data[i].line_data[j].edge_distance.size(); k++ )
			{
				dtmp	=	square_data[i].line_data[j].edge_distance.at(k);
				arr.push_back( dtmp );
			}
				//out2 << square_data[i].line_data[j].edge_distance.at(k) << " ";
			//out2 << endl;

		}

		// �p�⥭�� �зǮt
		double	mean;
		double	devia;
		dtmp	=	0;
		for( k = 0; k < arr.size(); k++ )
			dtmp	+=	arr.at(k);
		mean	=	dtmp / arr.size();
		dtmp	=	0;
		for( k = 0; k < arr.size(); k++ )
			dtmp	+=	(arr.at(k) - mean)*(arr.at(k)-mean);
		devia	=	sqrt( dtmp/(arr.size()-1) );

		out << setw(5) << settings->id << setw(12) << mean << setw(12) << devia << setw(5) << arr.size() << endl;
		out.close();
	}

	

//	ofstream	out2("s_e_distance.txt", ios::app );
//	out2 << setw(5) << id << setw(12) << mean << setw(12) << devia << setw(5) << arr.size() << endl;
//	out2.close();

	progress->outputImage( 0, final2 );		// �NDensityKernel�����G��X

	if( settings->batch_check )
		sprintf( str, "./output/%06d.bmp", settings->id );
	else
		sprintf( str, "./output/final.bmp");
	final2.save(str);



//t.Stop();
//progress->debuger( QString("time = %1 ms").arg( t.Elapsed() ), QColor( 0, 0, 255 ) );

	//progress->outputImage( 0, final );

	progress->debuger("finish extract channel ...", QColor( 255, 0, 0 ) );
	
}
#endif





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ KernelDensity ���� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/***************************************************************************************
	�غc���
/****************************************************************************************/
KernelDensity::KernelDensity()
{}







/***************************************************************************************
	�]�w���
	�|�N�榡���ഫ
/****************************************************************************************/
bMatrixInt		KernelDensity::set_data( SquareData _data )
{
	int		i,	j;

	const int	size	=	_data.size;

	data.resize( size, size );

	// �N����ন�@�ӥ���Ϊ�matrix
	for( i = 0; i < size; i++ )
		for( j = 0; j < size; j++ )
			data( i, j )	=	_data.line_data[i].shadow_seq(j);

	return	data;
}


/***************************************************************************************
	���� Gauss Kernel
/****************************************************************************************/
bMatrixDouble	KernelDensity::kernel_gauss( int n )
{
	kernel.resize( n*2+1, n*2+1 );
	int			i,	j;
	double		deviation	=	n / sqrt( 3.0 );

	boost::math::normal		g( 0, deviation );

	for( i = -n; i <= n; i++ )
		for( j = -n; j <= n; j++ )
			kernel( i+n, j+n )	=	pdf( g, i ) * pdf( g, j );

	return	kernel;
}



/***************************************************************************************
	���Ͱ򥻪������ kernel
/****************************************************************************************/
bMatrixDouble	KernelDensity::kernel_square( int n )
{
	int		m	=	2*n + 1;

	kernel.resize( m, m );

	int		i,	j;

	for( i = 0; i < m; i++ )
		for( j = 0; j < m; j++ )
			kernel( i, j )	=	1.0;

	return	kernel;
}



/***************************************************************************************
	�����Ӽƭp��
/****************************************************************************************/
int		KernelDensity::element_count()
{
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
bMatrixDouble	KernelDensity::dosomething()
{
	int		i,	j,	x,	y;
	int		N;				// �����Ӽ�
	int		N_size;			// �˴��Ϥj�p
	double	dcount;

	kernel_square( 1 );		// 1 �|���� 3*3 �� matrix

	p_data.resize( data.size1(), data.size2() );		// ��l�Ƹ��

	// �p�⤸���Ӽ�
	N		=	element_count();

	N_size	=	data.size1() * data.size2();

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
			dcount	/=	(9.0);							// 9.0 = h^D
			p_data( i, j )	=	dcount ;			// �קKwindows�j�p�z�Z���G
		}

	ofstream	out("kd.txt");
	for( j = 0; j < p_data.size2(); j++ )
	{
		for( i = 0; i < p_data.size1(); i++ )
			out << p_data(i,j) << " ";
		out << endl;
	}

	double	count	=	0;
	for( i = 0; i < p_data.size1(); i++ )
		for( j = 0; j < p_data.size2(); j++ )
			count	+=	p_data( i, j );
	out << "\n\n\n" << count;
	out.close();

	return	p_data;
}
#endif