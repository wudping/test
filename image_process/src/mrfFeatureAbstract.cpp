#include "mrfFeatureAbstract.h"
#include <iostream>
using namespace std;


/********************************************************************
	�غc
/*********************************************************************/
mrfFeatureAbstract::mrfFeatureAbstract( QImage imagePass, int dim )
{
	sample		=	imagePass;
	image		=	imagePass;

	dimension	=	dim;

	mean		=	new double[dimension];
	devia		=	new double[dimension];
}

/********************************************************************
	�غc
/*********************************************************************/
mrfFeatureAbstract::mrfFeatureAbstract( int dim )
{
	dimension	=	dim;
	//mean		=	NULL;
	//devia		=	NULL;
	mean		=	new double[dim];
	devia		=	new double[dim];
}



/********************************************************************
	copy construct
/*********************************************************************/
mrfFeatureAbstract::mrfFeatureAbstract( const mrfFeatureAbstract &ft )
{
	mean	=	NULL;
	devia	=	NULL;

	*this	=	ft;
}


/********************************************************************
	�Ѻc
/*********************************************************************/
mrfFeatureAbstract::~mrfFeatureAbstract()
{
	delete	[] mean;		mean	=	NULL;
	delete	[] devia;		devia	=	NULL;
}


/********************************************************************
	�]�w�Ϥ�
/*********************************************************************/
void	mrfFeatureAbstract::set_image( QImage img )
{
	sample	=	img;
	image	=	img;
}



/********************************************************************
	�^�ǹϤ�
/*********************************************************************/
QImage		mrfFeatureAbstract::get_image()
{
	return	image;
}

/********************************************************************
	 = operator
/*********************************************************************/
void	mrfFeatureAbstract::operator	=	( const mrfFeatureAbstract &ft )
{
	sample		=	ft.sample;
	image		=	ft.image;
	dimension	=	ft.dimension;

	delete	[]	mean;		mean	=	new double[dimension];
	delete	[]	devia;		devia	=	new double[dimension];

	for( int i = 0; i < dimension; i++ )
	{
		mean[i]		=	ft.mean[i];
		devia[i]	=	ft.devia[i];
	}

}


/********************************************************************
	�]�w������
/*********************************************************************/
void	mrfFeatureAbstract::set_mean( double *_m )
{
	for( int i = 0; i < dimension; i++ )
		mean[i]		=	_m[i];
}


/********************************************************************
	�^�ǥ�����
/*********************************************************************/
void	mrfFeatureAbstract::get_mean( double *_m )
{
	for( int i = 0; i < dimension; i++ )
		_m[i]	=	mean[i];
}


/********************************************************************
	�]�w�зǮt
/*********************************************************************/
void	mrfFeatureAbstract::set_devia( double *_d )
{
	for( int i = 0; i < dimension; i++ )
		devia[i]	=	_d[i];
}


/********************************************************************
	�^�ǼзǮt
/*********************************************************************/
void	mrfFeatureAbstract::get_devia( double *_d )
{
	for( int i = 0; i < dimension; i++ )
		_d[i]	=	devia[i];
}