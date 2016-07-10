#include "Energy_test.h"



/**********************************************************************
	�غc
/***********************************************************************/
EnergyTest::EnergyTest( mrfFeatureAbstract &ft, QImage img )
	:	mrfEnergyAbstract( ft, img )
{}


/**********************************************************************
	�ƻs�غc
/***********************************************************************/
EnergyTest::EnergyTest( const EnergyTest &e )
	:	mrfEnergyAbstract( e )
{
	*this	=	e;
}


/**********************************************************************
	�Ѻc
/***********************************************************************/
EnergyTest::~EnergyTest()
{}


/**********************************************************************
	operator =
/***********************************************************************/
void	EnergyTest::operator = ( const EnergyTest &e )
{
	mrfEnergyAbstract::operator = (e);

	// �ثe�S���e
}



/**********************************************************************
	smooth Energy
/***********************************************************************/
double		EnergyTest::E_smooth( int pix1, int pix2, int lab1, int lab2 )
{
	const double	weight	=	0.25;	// �[�v �H��A�ӫ�ҫ���

	//double	value	=	feature->get_norm( pix1, pix2 );

	// �o�ӬO 2 label ����
	/*if( lab1 == lab2 )
	{
		value	=	0.25*value;
		if( value > 1 )
			value	=	1;
		//value	*=	1.0;
		//value	=	1 - exp(value);
	}
	else
	{
		value	=	-0.25*value + 1;
		if( value < 0 )
			value	=	0;
		//value	*=	-1.0;
		//value	=	exp(value);
	}*/

	//return	weight * value;
	if( lab1 == lab2 )	return	-weight;
	else				return	weight;
}


/**********************************************************************
	data Energy
/***********************************************************************/
double		EnergyTest::E_data( int pix, int lab )
{
	double	value	=	feature->get_norm_squ( pix );

	//if( lab == 1 )
	//	value	=	feature2->get_norm(pix);

	// ���� 2 label
	/*if( lab == 0 )
	{
		value	=	0.25*value;
		if( value > 1 )
			value	=	1;
		//value	*=	1.0;
		//value	=	1 - exp(value);
	}
	else
	{
		value	=	-0.25*value + 1;
		if( value < 0 )
			value	=	0;
		//value	*=	-1.0;
		//value	=	exp(value);
	}*/

	return	value;
}


/**********************************************************************
	�^�Ǧۤv�O��������
/***********************************************************************/
MRF_ENERGY_TYPE		EnergyTest::type()
{
	return	MRF_ENERGY_TEST;
}


/**********************************************************************
	�^�Ǿ��v�K�ר�ƭ�
/***********************************************************************/
double	EnergyTest::Prob( int pix, int lab )
{
	double	value	=	feature->get_norm_squ(pix);
	double	devia	=	feature->get_devia_det();

	double	r_data;

	r_data	=	exp( -value ) / (devia*sqrt(2.0*PI));

	return	r_data;
}