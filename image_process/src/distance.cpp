#include "distance.h"


using namespace std;


/**********************************************************************
	�غc
/***********************************************************************/
Distance::Distance( QImage imagePass )
	:	mrfFeatureAbstract( imagePass, dim() )
{
	anchor_x		=	0;		
	anchor_y		=	0;
	anchor_width	=	0;
	anchor_height	=	0;
}


/**********************************************************************
	�غc
/***********************************************************************/
Distance::Distance()
	:	mrfFeatureAbstract( dim() )
{
	anchor_x		=	0;		
	anchor_y		=	0;
	anchor_width	=	0;
	anchor_height	=	0;
}


/**********************************************************************
	copy construct
/***********************************************************************/
Distance::Distance( const Distance &dt )
	:	mrfFeatureAbstract(dt)
{
	*this	=	dt;
}


/**********************************************************************
	�Ѻc
/***********************************************************************/
Distance::~Distance()
{}


/**********************************************************************
	���դ���
/***********************************************************************/
void	Distance::segmentation( QImage img )
{
	assert(false);
}


/**********************************************************************
	�p��feature
/***********************************************************************/
void	Distance::cal_feature( QImage img )
{
	image	=	img;

	const int	width	=	image.width();
	const int	height	=	image.height();

	assert( anchor_x >= 0 && anchor_x + anchor_width < width );
	assert( anchor_y >= 0 && anchor_y + anchor_height < height );
	assert( anchor_width > 0 && anchor_height > 0 );

	// �]��feature�ȴN�O�Ϥ�������RGB��  ���Υt�~�� �����D������зǮt

	//cal_mean();		// �p�⥭����
	//cal_devia();	// �p��зǮt
}



/**********************************************************************
	��X�ɮ�
/***********************************************************************/
void	Distance::output_txt()
{}


/**********************************************************************
	�p�⥭����
/***********************************************************************/
void	Distance::cal_mean()
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	assert( anchor_x >= 0 && anchor_x + anchor_width < width );
	assert( anchor_y >= 0 && anchor_y + anchor_height < height );
	assert( anchor_width > 0 && anchor_height > 0 );

	int		i,	j;
	int		size	=	anchor_width * anchor_height;
	double	sum		=	0;

	for( i = anchor_x; i < anchor_x + anchor_width; i++ )
		for( j = anchor_y; j < anchor_y + anchor_height; j++ )
			sum		+=	image.gray_channel( i, j );

	mean[0]	=	sum / size;			// distance���� = 1
	
	cout << "mean = " << mean[0] << endl;
}



/**********************************************************************
	�p��зǮt
/***********************************************************************/
void	Distance::cal_devia()
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	assert( anchor_x >= 0 && anchor_x + anchor_width < width );
	assert( anchor_y >= 0 && anchor_y + anchor_height < height );
	assert( anchor_width > 0 && anchor_height > 0 );

	int		i,	j;
	int		size	=	anchor_width * anchor_height - 1;
	double	sum		=	0;
	double	value;

	assert( size > 0 );

	for( i = anchor_x; i < anchor_x + anchor_width; i++ )
		for( j = anchor_y; j < anchor_y + anchor_height; j++ )
		{
			value	=	image.gray_channel( i, j ) - mean[0];
			sum		+=	pow( value, 2 );
		}

	devia[0]	=	sqrt( sum/size );		// �]�����׬O 1
	
	cout << "devia = " << devia[0] << endl;
}


/**********************************************************************
	�^�Ǹ� pixel �� feature
/***********************************************************************/
void	Distance::get_feature( int pix, double *_f )
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	int		i	=	pix / height;
	int		j	=	pix % height;

	assert( i >= 0 && i < width && j >= 0 && j < height );

	//sVectorDouble	vec(DIMENSION);

	_f[0]	=	image.gray_channel( i, j );			// �]��DIMENSION = 1
}



/**********************************************************************
	�ˬd����
/***********************************************************************/
bool	Distance::check_dim()
{
	//assert( DIMENSION == devia.size() );
	//assert( DIMENSION == mean.size() );

	//bool	is	=	DIMENSION == devia.size() && DIMENSION == mean.size();

	//return	is;
	return	true;		// �]���令����
}



/**********************************************************************
	�p��˱i 
	��cal_feature���e�|�ܹ� �t�b�o��|�⥭���зǮt
/***********************************************************************/
void	Distance::cal_sample_feature( QImage img )
{
	assert(0);
}

/**********************************************************************
	�p�� |u|
	�|�}�ڸ�
/***********************************************************************/
double		Distance::get_norm( int pix )
{
	double	value	=	get_norm_squ(pix);

	return	sqrt(value);
}



/**********************************************************************
	�^�� norm ������  |u|^2
	(�����Wdeviation)

		(f - m )^2
	_______________
		2 sigma^2


/***********************************************************************/
double		Distance::get_norm_squ( int pix )
{
	const int	width	=	image.width();
	const int	height	=	image.height();

	assert( width > 0 && height > 0 );

	int		i,	j;
	double	value;
	double	r;

	i	=	pix / height;
	j	=	pix % height;

	assert( i >= 0 && i < width && j >= 0 && j < height );

	// ��ڤW�����Wdeviation!!
	value	=	image.gray_channel( i, j ) - mean[0];	// �]������ = 1
	r		=	pow(value,2) / (2.0*pow(devia[0],2));	// �]������ = 1

	return	r;
}



/**********************************************************************
	�p����pixel��norm |u-v|
/***********************************************************************/
double		Distance::get_norm( int pix1, int pix2 )
{
	// �ثe�L���e
	assert(false);
	return	0;
}


/**********************************************************************
	�^�ǥثe��feature type
/***********************************************************************/
MRF_FEATURE_TYPE	Distance::type()
{
	return	MRF_FEATURE_DISTANCE;
}



/**********************************************************************
	�]�w���˽d��
/***********************************************************************/
void	Distance::set_anchor( int x, int y, int w, int h )
{
	anchor_x		=	x;
	anchor_y		=	y;
	anchor_width	=	w;
	anchor_height	=	h;
}






/**********************************************************************
	�^�Ǻ���
/***********************************************************************/
int		Distance::dim()
{
	return	DIMENSION;
}


/**********************************************************************
	�^��deviation��determine  
	(�]����Ӫ��פ�O�[�W |Sigma| ���T�w�Onorm�٬Odet)
	�`�N�o��O�^�ǥ���!!
/***********************************************************************/
double		Distance::get_devia_det()
{
	return	devia[0] * devia[0];
}




/**********************************************************************
	operator = 
/***********************************************************************/
void	Distance::operator	=	( const Distance &dt )
{
	mrfFeatureAbstract::operator = (dt);

	anchor_x		=	dt.anchor_x;
	anchor_y		=	dt.anchor_y;
	anchor_width	=	dt.anchor_width;
	anchor_height	=	dt.anchor_height;
}

