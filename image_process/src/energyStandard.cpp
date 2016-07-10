#include "energyStandard.h"



/**********************************************************************
	�غc
/***********************************************************************/
EnergyStandard::EnergyStandard( mrfFeatureAbstract &ft, QImage img )
	:	mrfEnergyAbstract( ft, img )
{
	feature->cal_feature(img);

}


/**********************************************************************
	�ƻs�غc
/***********************************************************************/
EnergyStandard::EnergyStandard( const EnergyStandard &e )
	:	mrfEnergyAbstract( e )
{
	*this	=	e;
}


/**********************************************************************
	�Ѻc
/***********************************************************************/
EnergyStandard::~EnergyStandard()
{}




/**********************************************************************
	= operator
/***********************************************************************/
void	EnergyStandard::operator = ( const EnergyStandard &e )
{
	mrfEnergyAbstract::operator = (e);

	// �ثe�S���e
}



/**********************************************************************
	smooth Energy
/***********************************************************************/
double		EnergyStandard::E_smooth( int pix1, int pix2, int lab1, int lab2 )
{
	assert( is_feature() );
	assert( check_image() );

	const double	weight	=	0.25;	// �[�v �H��A�ӫ�ҫ��ק�  �ثe�O�ծեX�Ӫ���

	if( lab1 == lab2 )	return	-weight;
	else				return	weight;
}


/**********************************************************************
	data Energy
/***********************************************************************/
double		EnergyStandard::E_data( int pix, int lab )
{
	//assert( is_feature() );
	//assert( check_image() );

	double	value	=	feature->get_norm_squ( pix );
	return	value ;
}


/**********************************************************************
	�^�Ǧۤv�O��������
/***********************************************************************/
MRF_ENERGY_TYPE		EnergyStandard::type()
{
	return	MRF_ENERGY_STANDARD;
}


/**********************************************************************
	�^�Ǿ��v�K�ר�ƭ�
/***********************************************************************/
double	EnergyStandard::Prob( int pix, int lab )
{
	assert( is_feature() );
	assert( check_image() );

	double	value	=	feature->get_norm_squ(pix);
	double	devia	=	feature->get_devia_det();	// �`�N�o��^�Ǫ��O����
	int		dim		=	feature->dim();
	double	r_data;

	assert( devia > 0 );		// �]�����w...

	r_data	=	exp( -value ) / sqrt( pow(2.0*PI,dim)*devia );	// ��i�h�ڸ��O�]������

	return	r_data;
}