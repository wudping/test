#include "mrfEnergyAbstract.h"




/********************************************************************
	�غc
/*********************************************************************/
mrfEnergyAbstract::mrfEnergyAbstract()
{
	feature		=	NULL;
}



/********************************************************************
	�غc
	�o�䪺�Ҧ��Osupervised
	ft�|����n�˱i,�b�o��h�p��ؼЪ�feature
/*********************************************************************/
mrfEnergyAbstract::mrfEnergyAbstract( mrfFeatureAbstract &ft, QImage img )
{
	//assert( ft.get_image() == img );

	image	=	img;

	// �ھ�type�M�wfeature����
	switch( ft.type() )
	{
		case	MRF_FEATURE_MOMENT:
			feature		=	new	Moment( (Moment&)ft );
			break;

		case	MRF_FEATURE_DISTANCE:
			feature		=	new	Distance( (Distance&)ft );
			break;

		case	MRF_FEATURE_GABOR:
			feature		=	new	Gabor( (Gabor&)ft );
			break;

		case	MRF_FEATURE_COCCM:
			feature		=	new	COCCM( (COCCM&)ft );
			break;

		case	MRF_FEATURE_MRSAR:
			feature		=	new MRSAR( (MRSAR&)ft );
			break;

		default:
			assert(false);
	}

	//feature->cal_feature(image);	// �p��feature  ���ʨ�~���h�p��
}


/********************************************************************
	�p�� feature
/*********************************************************************/
void	mrfEnergyAbstract::cal_feature()
{
	assert( feature != NULL );

	feature->cal_feature(image);
}


/********************************************************************
	copy construct
/*********************************************************************/
mrfEnergyAbstract::mrfEnergyAbstract( const mrfEnergyAbstract &e )
{
	*this	=	e;
}



/********************************************************************
	�Ѻc
/*********************************************************************/
mrfEnergyAbstract::~mrfEnergyAbstract()
{
	delete	feature;
	feature		=	NULL;
}


/********************************************************************
	operator =
/*********************************************************************/
void	mrfEnergyAbstract::operator = ( const mrfEnergyAbstract &e )
{
	assert( e.feature != NULL );	// �p�G���w�]�غc  ���ਫ�o��

	switch( e.feature->type() )
	{
		case	MRF_FEATURE_MOMENT:
			feature		=	new	Moment( (Moment&)(*e.feature) );
			break;
		case	MRF_FEATURE_DISTANCE:
			feature		=	new	Distance( (Distance&)(*e.feature) );
			break;

		case	MRF_FEATURE_GABOR:
			feature		=	new Gabor( (Gabor&)(*e.feature) );
			break;

		case	MRF_FEATURE_COCCM:
			feature		=	new	COCCM( (COCCM&)(*e.feature) );
			break;

		case	MRF_FEATURE_MRSAR:
			feature		=	new MRSAR( (MRSAR&)(*e.feature) );
			break;

		default:
			assert(false);
	}

	image		=	e.image;
}



/********************************************************************
	�^��image
/*********************************************************************/
QImage		mrfEnergyAbstract::get_image()
{
	return	image;
}



/********************************************************************
	�^�ǹϤ��e
/*********************************************************************/
int		mrfEnergyAbstract::get_img_width()
{
	return	image.width();
}


/********************************************************************
	�^�ǹϤ���
/*********************************************************************/
int		mrfEnergyAbstract::get_img_height()
{
	return	image.height();
}


/********************************************************************
	�]�w�Ϥ�
/*********************************************************************/
void	mrfEnergyAbstract::set_image( QImage img )
{
	image	=	img;

	feature->set_image(image);
}




/********************************************************************
	�^�� feature type
/*********************************************************************/
MRF_FEATURE_TYPE	mrfEnergyAbstract::get_feature_type()
{
	assert( feature != NULL );

	return	feature->type();
}


/********************************************************************
	�ˬd��feature��image�O�_�ۦP
/*********************************************************************/
bool	mrfEnergyAbstract::check_image()
{
	assert( feature != NULL );

	bool	is;
	is	=	(image == feature->get_image());

	return	is;
}


/********************************************************************
	�ˬdfeature�O�_��l��
/*********************************************************************/
bool	mrfEnergyAbstract::is_feature()
{
	if( feature == NULL )	return	false;
	else					return	true;
}



/********************************************************************
	�^��feature��dim
/*********************************************************************/
int		mrfEnergyAbstract::dim()
{
	assert( feature != NULL );

	return	feature->dim();
}


/********************************************************************
	���o��pixel��feature
/*********************************************************************/
void	mrfEnergyAbstract::get_feature( int pix, double *_f )
{
	assert( feature != NULL );

	feature->get_feature( pix, _f );
}



/********************************************************************
	�]�w���� (��feature)
/*********************************************************************/
void	mrfEnergyAbstract::set_mean( double *_m )
{
	assert( feature != NULL );

	feature->set_mean( _m );
}


/********************************************************************
	�^��feature����
/*********************************************************************/
void	mrfEnergyAbstract::get_mean( double *_m )
{
	assert( feature != NULL );

	feature->get_mean(_m);
}



/********************************************************************
	�]�w�зǮt
/*********************************************************************/
void		mrfEnergyAbstract::set_devia( double *_d )
{
	assert( feature != NULL );

	feature->set_devia(_d);
}


/********************************************************************
	�^�ǼзǮt
/*********************************************************************/
void	mrfEnergyAbstract::get_devia( double *_d )
{
	assert( feature != NULL );

	feature->get_devia(_d);
}



/********************************************************************
	�ˬd dimension
/*********************************************************************/
bool	mrfEnergyAbstract::check_dim()
{
	assert( feature != NULL );

	return	feature->check_dim();
}



/********************************************************************
	��X��� (���ե�)
/*********************************************************************/
void	mrfEnergyAbstract::output()
{
	QImage	img(640, 640, QImage::Format_RGB888);

	for( int i = 0; i < 640; i++ )
		for( int j = 0; j < 640; j++ )
			img.gray_channel( i, j, ((Moment*)feature)->get_data(i,j) );
	img.save("../TEMP/test.bmp");
}
