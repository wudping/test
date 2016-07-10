
#include "stdafx.h"
#include "gaussfilter.h"


using namespace std;

#define MAX(A,B)   A>B ? A : B
#define MIN(A,B)   A<B ? A : B





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	�o��� GaussFilter 2D
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/



/*======================================================================================
	�^��filter�x�}
========================================================================================*/
boost::numeric::ublas::matrix<double>	GaussFilter_2D::get_map()
{
	return	filter_map;
}


/*======================================================================================
	���XDog��Map
========================================================================================*/
double	GaussFilter_2D::get_DoG( int i, int j )
{

	int		k	=	2;
	int		w2	=	w * k,
			l2	=	l * k;

	//if( i<-w2 || i>w2 || j<-l2 || j>l2 )	error_msg( "Gauss::Filter_2D::get_DoG" );

	return		DOG( i + w2,  j + l2  );
}









/*======================================================================================
	����filter, �u���T�w�϶�   
	lu = left up    
	rd = right down
========================================================================================*/
QImage	GaussFilter_2D::do_filter( QImage passImage, PointInt lu, PointInt rd )
{
	//int			/*i,*/	/*j,*/	x,	y;
	//int		tmp;
	//double		dsum;
	
	const int	width	=	passImage.width();
	const int	height	=	passImage.height();

	QImage	image	=	passImage;
	
	image.fill(0);

	QRgb	rgb;
	int		gray;
	
//#pragma omp parallel for
	for(  int   i = lu.x;       i <= rd.x;    i++    )
	{

		for(  int  j = lu.y;    j<= rd.y;     j++     )
		{
			// �w�惡 pixel �� filter
			double		dsum	=	0;

			for( int  x = -w;    x <= w;    x++  )
				for(  int  y = -l;    y <= l;   y++  )
				{
					if( i+x < 0 || i+x >= image.width() || j+y < 0 || j+y >= image.height() )
						continue;
					dsum	+=	passImage.gray_channel( i+x, j+y ) * get(x,y);
				}
			
			// �g�J��� (�קK�v�T��쥻�����)
			int		tmp		=	static_cast<int>(dsum);
			image.gray_channel( i, j, tmp );
		}
	}

	return	image;
}





#ifdef _OPENPIV_
/*======================================================================================
	�N����Ū�i��, ��filter���ʧ@
========================================================================================*/
QImage	GaussFilter_2D::do_filter( QImage passImage , Progress *progress = NULL )
#else
QImage	GaussFilter_2D::do_filter( QImage passImage )
#endif
{
	
	const int	width	=	passImage.width();
	const int	height	=	passImage.height();

	QImage			img		=	passImage;								// ��l�Ϲ�
	bMatrixDouble	wmatrix( width, height );							// �s���Vfilter���G���Ϲ�
	bMatrixDouble	lmatrix( width, height );							// �s���a�Vfilter���G���Ϲ�   (�̫᪺���G)

	int				i,	j,	x,	y;
	double			dsum;

	wmatrix.clear();
	lmatrix.clear();

#ifdef _OPENPIV_
	// �i�ױ�����
	if( progress != NULL )
		progress->setGress(0);
#endif

	// �}�l�i��filter
	for(  i = 0;  i < width;  i++  )
	{

#ifdef _OPENPIV_
		if( progress != NULL )
			progress->setGress( (i-w)*100/(width-2*w) );
#endif

		for(  j = 0;  j < height;  j++ )
		{
			// ��C�@��pixel����m����V��filter
			dsum	=	0;
			for(  x = -w;  x <= w;  x++  )
			{
				if( i+x < 0 || i+x >= width )	continue;

				dsum	+=	filter_map_w( x+w ) * img.gray_channel( i+x, j );
			}
			wmatrix( i, j )	=	dsum;
		}
	}


	// �i���a�V��filter
	for( i = 0;  i < width;  i++  )
	{
#ifdef _OPENPIV_
		if( progress != NULL )
			progress->setGress( (i-w)*100/(width-2*w) );
#endif

		for( j = 0;  j < height;  j++ )
		{
			// ��wimg���a�V��filter
			dsum	=	0;
			for( y = -l;  y <= l;  y++ )
			{
				if(  j+y < 0 || j+y >= height )		continue;

				dsum	+=	filter_map_l( y+l ) * wmatrix( i, j+y );
			}
			lmatrix( i, j )	=	dsum;
		}
	}

	QImage	filter( width, height, QImage::Format_RGB888 );

	for( i = 0;  i < width;  i++  )
		for(  j = 0;  j < height;  j++  )
			filter.gray_channel( i, j, static_cast<int>( lmatrix(i,j) ) );

	return		filter;
}





/*======================================================================================
	GaussFilter 2D
	�غc�禡
========================================================================================*/
GaussFilter_2D::GaussFilter_2D()
{
	w		=	1;
	l		=	1;

	// ���� filter map
	generate_map();
}



/*======================================================================================
	GaussFilter 2D
	�غc�禡
========================================================================================*/
GaussFilter_2D::GaussFilter_2D( int _w, int _l )
{
	w		=	_w;
	l		=	_l;

	// ���� filter map
	generate_map();
}


/*======================================================================================
	����DoG map
========================================================================================*/
void	GaussFilter_2D::generate_DoG_map()
{	
	int		i,	j;
	int		w2,	l2;
	int		k	=	2;		// �i�H�վ�
	double	dsum;

// �P�_�O�nsingle�ث� �٬Ow,l��ث�
#ifdef SINGLE_SCALE



#else

	// �o��S��k�Ψ��1D���Ӱ��ۭ�....

	w2	=	k * w;
	l2	=	k * l;

	double		d1	=	1. * w / sqrt(3.0) ;
	double		d2	=	1. * l / sqrt(3.0) ;
	double		d3	=	2. * d1;
	double		d4	=	2. * d2;

	boost::math::normal		gauss1( 0, d1  );
	boost::math::normal		gauss2( 0, d2  );

	boost::numeric::ublas::matrix<double>	map;


	/*
		deviation	=	scale / sqrt(3)
		�j�M�d��	=	deviation * sqrt(3)	=	scale
	*/
	map.resize(   2*w2 + 1,    2*l2 + 1   );
	

	for( i = -w2;    i <= w2;    i++ )
		for( j = -l2;    j <= l2;  j++ )
			map( i+w2, j+l2 )	=	pdf( gauss1, i ) * pdf( gauss2, j );

	
	boost::math::normal		gauss3( 0, d3  );
	boost::math::normal		gauss4( 0, d4  );

	boost::numeric::ublas::matrix<double>	map2;


	/*
		deviation	=	scale / sqrt(3)
		�j�M�d��	=	deviation * sqrt(3)	=	scale
	*/
	map2.resize(   2*w2 + 1,    2*l2 + 1   );
	

	for( i = -w2;    i <= w2;    i++ )
		for( j = -l2;    j <= l2;  j++ )
			map2( i+w2, j+l2 )	=	pdf( gauss3, i ) * pdf( gauss4, j );



	DOG.resize(   w2*2 + 1, l2*2 + 1   );

	// ���� DoG map
	for( i = -w2;   i <= w2;   i++ )
		for(   j = -l2;   j <= l2;    j++  )
			DOG(  i + w2,  j + l2  )	=	map2(   i+w2 , j+l2   ) - map(   i+w2,   j+l2   );

	// ����normalize �Y��
	dsum	=	0;
	for( i = -w2;   i <= w2;  i++ )
		for( j = -l2;   j <= l2;   j++   )
			dsum	+=	map( i + w2, j + l2 );

	normalized	=	1.0 / dsum ;

#endif


}




/*======================================================================================
	����filter map
========================================================================================*/
void	GaussFilter_2D::generate_map()
{	
	int			i,	j;
	double		dsum;
	double		normalized;

	// �Ψ�� 1D ���Ӱ� 2D �� filter
	GaussFilter_1D		gw( w ),	gl( l );
	filter_map.resize( w*2+1, l*2+1 );

	// ���� filter map
	for( i = -w;   i <= w;   i++ )
	{
		for(   j = -l;   j <= l;    j++  )
		{
			filter_map(  i + w,  j + l  )	=	gw[i] * gl[j] ;

			//printf("%.3lf ", filter_map( i+w, j+l ) );
		}
		//printf("\n");
	}


	// ����normalize �Y��
	dsum	=	0;
	for( i = -w;   i <= w;  i++ )
		for( j = -l;   j <= l;   j++   )
			dsum	+=	(*this)(i,j);
	normalized	=	1.0 / dsum ;

	// ��normalized���ʧ@
	for( i = -w;   i <= w;  i++ )
		for( j = -l;   j <= l;   j++   )
			(*this)(i,j)	*=	normalized;


	for( i = 0;  i < filter_map.size1(); i++ )
	{
		for(  j = 0;  j < filter_map.size2(); j++ )
			printf("%.3lf ", filter_map(i, j) );
		printf("\n");
	}



	// ���;�V��filter
	filter_map_w.resize( 2*w + 1 );
	for(  i = -w;  i <= w;  i++ )
		filter_map_w( i+w )		=	gw[i];

	printf("\n\n\n");
	for(  i = 0;  i < filter_map_w.size();  i++ )
		cout << filter_map_w(i) << " ";
	cout << endl;

	// normalized
	/*dsum	=	0;
	for(  i = 0;  i < filter_map_w.size();  i++  )
		dsum	+=	filter_map_w( i );
	normalized	=	1.0 / dsum;
	for(  i = 0; i < filter_map_w.size();  i++ )
		filter_map_w( i )	*=	normalized;*/

	// �����a�V��filter
	filter_map_l.resize( 2*l + 1 );
	for( i = -l;  i <= l;  i++  )
		filter_map_l( i+l )		=	gl[i];

	// normalized
	/*dsum	=	0;
	for( i = 0;  i < filter_map_l.size();  i++ )
		dsum	+=	filter_map_l(i);
	normalized	=	1.0 / dsum;*/
	for( i = 0;  i < filter_map_l.size();  i++ )
		filter_map_l(i)		*=	normalized;

	for(  i = 0;  i < filter_map_l.size();  i++  )
		cout << filter_map_l(i) << " ";
	cout << endl;


	printf("\n\n");

	for( i = 0;  i < filter_map.size1();  i++  )
	{
		for( j = 0; j < filter_map.size2();  j++ )
			printf("%.3lf ", filter_map_w(i) * filter_map_l(j) );
		printf("\n");
	}

}



/*======================================================================================
	�]�w w
========================================================================================*/
void	GaussFilter_2D::set_w( int _w )
{
	w	=	_w;
}

/*======================================================================================
	�]�w l
========================================================================================*/
void	GaussFilter_2D::set_l( int _l )
{
	l	=	_l;
}




/*======================================================================================
	�� map �ȥ�, �|�۰ʱNindex �ন �H 0 ������
========================================================================================*/
double	GaussFilter_2D::get( int i, int j )
{
#ifdef SINGLE_SCALE

	if( i<-scale || i>scale || j<-scale || j>scale )
		error_msg( "Gauss::Filter_2D::get_map" );

	return		filter_map( i + scale,  j + scale  );

#else

	//if( i<-w || i>w || j<-l || j>l )	error_msg( "Gauss::Filter_2D::get_map" );

	return		filter_map( i + w,  j + l  );
#endif	
}



/*======================================================================================
	�� map �ȥ�, �|�۰ʱNindex �ন �H 0 ������
========================================================================================*/
//double&		GaussFilter_2D::get_map ( int i, int j )
double&		GaussFilter_2D::operator () ( int i, int j )
{
	//if( i<-w || i>w || j<-l || j>l )	
	//	error_msg( "Gauss::Filter_2D::get_map" );

	return		filter_map( i + w,  j + l  );
}





/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	�o��� GaussFilter 1D
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/


/*======================================================================================
	GaussFilter 1D
	�غc�禡
========================================================================================*/
GaussFilter_1D::GaussFilter_1D( int _scale )
{

	//if( _scale %2 == 0 )
	//	error_msg( "scale must be odd" );
	//if( _scale <= 0 )	error_msg( "scale must lager than 0");

	scale		=	_scale;
	mean		=	0;
	deviation	=	scale / sqrt( 3.0 );

	// ����filter map
	generate_map();

}




/*======================================================================================
	����filter map  �T�w k = 2
========================================================================================*/
void	GaussFilter_1D::generate_DoG_map()
{	
	int			i;//,	j;
	int			scale2;
	int			k	=	2;			// �i�H�վ� k ���j�p

	boost::math::normal		gauss1( 0, deviation );
	boost::math::normal		gauss2( 0, k * deviation );


	/*
		deviation	=	scale / sqrt(3)
		�j�M�d��	=	deviation * sqrt(3)	=	scale
	*/
	scale2	=	scale * k;
	DoG.resize( 2*scale2 + 1 );
	

	for( i = -scale2; i<=scale2; i++ )
		DoG[ i+scale2 ]	=	pdf( gauss2, i ) - pdf( gauss1, i );

}




/*======================================================================================
	����filter map
========================================================================================*/
void	GaussFilter_1D::generate_map()
{
	int			i,	j;

	boost::math::normal		gauss( 0, deviation );


	/*
		deviation	=	scale / sqrt(3)
		�j�M�d��	=	deviation * sqrt(3)	=	scale
	*/
	filter_map.resize( 2*scale+1 );
	

	for( i = -scale; i<=scale; i++ )
		filter_map[ i+scale ]	=	pdf( gauss, i );


}




/*======================================================================================
	�� map �ȥ�, �|�۰ʱNindex �ন �H 0 ������  
	�i�H�B�z -index ~ index
	�åB���ˬdindex�O�_�X�z����
========================================================================================*/
double&		GaussFilter_1D::operator [] ( int index )
{
	//if( index < -scale || index > scale )	error_msg( "GaussFilter_1D [] out of range" );

	return	filter_map[ index + scale ];
}


/*======================================================================================
	���XDog��Map
========================================================================================*/
double	GaussFilter_1D::get_DoG( int index )
{
	int		k	=	2;
	int		scale2	=	k*scale;

	//if( index < -scale2 || index > scale2 )	error_msg("GaussFilter_1D::get_DoG");

	return	DoG[ index + scale2 ];
}


/*======================================================================================
	�� map �ȥ�, �|�۰ʱNindex �ন �H 0 ������  
	�i�H�B�z -index ~ index
	�åB���ˬdindex�O�_�X�z����
========================================================================================*/
double&		GaussFilter_1D::operator () ( int index )
{
	//if( index < -scale || index > scale )		error_msg( "GaussFilter_1D () out of range" );

	return	filter_map[ index + scale ];
}