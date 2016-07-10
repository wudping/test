// FFT.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#ifndef _OPENPIV_
#include "stdafx.h"
#endif

#include <fftw3.h>
#include <boost/numeric/ublas/matrix.hpp>
#include "FFT.h"

using namespace std;

#ifdef _OPENPIV_

/*=======================================================================================================
	�غc
	OPENPIV ���U�ϥ�
/*=======================================================================================================*/
FFT::FFT(Settings *settingsPass, DataContainer  *filedataPass)	
	: AndreaEngine( )
{	
	progress	=	settingsPass->getProgress();	
	settings	=	settingsPass;
	filedata	=	filedataPass;

    mutex.lock();
	// ��ݭnlock���ʧ@
	sigma	=	pow( 0.00625, 2 );						// �w�]��
	//sigma	=	0.0005;
	pi		=	3.141592653589793;
    mutex.unlock();
}

#else
/*=======================================================================
	�غc
/*=======================================================================*/
FFT::FFT()
{
	img.load("test.bmp");
	w		=	img.width();
	h		=	img.height();
	N		=	w * h;
	sigma	=	0.01;//0.5/sqrt(3.0);
	pi		=	3.141592653589793;
}
#endif



/*=================================================================================
	�w�]function
	�Ѻc
===================================================================================*/
FFT::~FFT()
{}


/*=======================================================================
	���ե� 
	���Y�ǰʧ@
/*=======================================================================*/
void	FFT::doing2()
{
	int		i,	j;

	for( i=0; i<R.size1(); i++ )
		for( j=0; j<R.size2(); j++ )
		{
			R( i, j )	*=	filter_sin( i, j );
			I( i, j )	*=	filter_sin( i, j );
		}

	//output_map( R, "R_map.bmp" );
	//output_map( I, "I_map.bmp" );

}



/*==============================================================================================
	radial function
/*==============================================================================================*/
double	FFT::Kp( double r )
{
	//return	r*r*exp( -0.5 *sigma*r*r );					// �ª���
	return	r*exp( -0.5 *sigma*r*r );						// �s����
	//return		sqrt(r) * exp( -0.5 * sigma * r*r );
}





/*==============================================================================================
	����polynomial  (�|�q�L�I��)   
	�Ψӧ�junction
	model����:
	f = sum Z* (x-xi)(x-xj)(y-yi)(y-yj)/(x0-xi)(x0-xj)(y0-yi)(y0-yj)
	x0,y0�OZ*�����쪺�I
	(xi,yi)  (xi, yj)  (xj,yi)  (xj,yj) �O���A�P�@���u�W���I  
	�U���@�b�ӳy�h����, �ëD�ϥΥ|���I

	(xi,yi)---(xi,yj)---(xi,y0)
	   |         |         |
    (xj,yi)---(xj,yj)---(xj,y0)
	   |         |         |
    (x0,yi)---(x0,yj)---(x0,y0)    Z* = f(x0,y0)

	�i�}��U���̦���   
	x^2 y^2 + x^2 y + x y^2 + x^2 + y^2 + xy + x + y + c
/*==============================================================================================*/
double	FFT::ite_poly( int dx, int dy, double x, double y, double xx, double yy )
{
	double	r	=	1;
	//double	xx	=	X + 0.5;
	//double	yy	=	Y + 0.5;

	int		i,	j;
	int		N	=	1;


	// ���U���k�O���F���ͤW����������model
	for( i=-N; i<=N; i++ )
	{
		if( i != dx )
		{
			r	*=	( x - xx - i );
			r	/=	( i - dx );
		}
	}

	for( j=-N; j<=N; j++ )
	{
		if( j != dy )
		{
			r	*=	( y - yy - j );
			r	/=	( j - dy );
		}
	}

	return	r;

}


/*==============================================================================================
	����polynomail���h��������
/*==============================================================================================*/
void	FFT::itepolation( int _x, int _y )
{
	double		rtx,	rty;
	double		xx	=	_x + 0.5;		// 4.5�o�˪� �B�I�Ƥ����I�y��
	double		yy	=	_y + 0.5;
	double		sum	=	0;
	double		x,	y,	t;
	int			i,	j;

	// �i�椺��  �åB��X�̤j��
	double		max		=	0;
	double		max_x	=	-1;
	double		max_y	=	-1;

	int			N		=	1;

	// �j�M�̤j��
	for( rtx = -1; rtx <= 1; rtx += 0.01 )
	{
		for( rty = -1; rty <= 1; rty += 0.01 )
		{

			x	=	rtx + xx;
			y	=	rty + yy;

			// �W�X���
			//if( fabs(x) > 1.5 /*+ N*/ || fabs(y) > 1.5 /*+ N*/ )	break;

			sum		=	0;
			for( i = -N;  i <= N;  i++ )
				for( j = -N;  j <= N;  j++ )
					sum		+=	junction.gray_channel( i + (int)xx, j + (int)yy ) * ite_poly( i, j, x , y , xx, yy  );

			if( sum > max ) 
			{
				max		=	sum;
				max_x	=	x ;
				max_y	=	y  ;
			}
		}
	}

	progress->debuger( QString("Find junction at ( %1  %2 )").arg(max_x).arg(max_y), QColor(0,0,255) );
	//cout << max_x << " " << max_y << endl;
}











/*==============================================================================================
	�η��y�Ъ��覡�Ӳ���filter
/*==============================================================================================*/
void	FFT::draw_filter( int n )
{
	IntMatrix			count_map;				// �p���map�̭�  �C��pixel ���h�֭��I

	count_map.resize( image.width(), image.height() );
	filter_cos.resize( image.width(), image.height() );
	filter_sin.resize( image.width(), image.height() );
	count_map.clear();
	filter_cos.clear();
	filter_sin.clear();

	int			i,	j;
	//int			w,	h;
	int			size;
	double		x,	y,	xx,	yy;
	double		r,	phi;
	double		value,	radial;
	double		_cos,	_sin;				// cos, sin  ���F�קK��function�� cos()  sin()�d�V.
	char		str[100];

	size	=	(image.width() > image.height() ) ? image.width() : image.height();
	//size	=	(size+1) / 2;

	//w		=	image.width();
	//h		=	image.height();


	// �η��y�ШӲ���filter
	for( r = 0.002; r <= size; r += 0.002 )
		for( phi = 0; phi < 2*pi; phi += 0.002 )
		{
			x		=	r * cos(phi) ;
			y		=	r * sin(phi) ;
			xx		=	fabs(x);				// �Υ��ȥh���|�ˤ��J�A��^�쥻��sign
			yy		=	fabs(y);
			i		=	(int) (xx + 0.5);
			j		=	(int) (yy + 0.5);

			// �קK�O������~
			if( i>w/2 || j>h/2 || i<-(w+1)/2+1 || j<-(h+1)/2+1 ){}

			else
			{
				i		*=	( x>=0 ) ? 1 : -1;
				j		*=	( y>=0 ) ? 1 : -1;		//  ��o�䬰�� �o�쪺�O (-w/2 ~ w/2���y��)

				i		=	(i<0) ? w+i : i;
				j		=	(j<0) ? h+j : j;

				// �p�� cos n*phi    sin n*phi
				_cos	=	cos( n*phi );
				_sin	=	sin( n*phi );
				
				radial	=	Kp( r );

				count_map( i, j )++;							// �p����I�|���h���I,�̫᮳�h��  normalized
				filter_cos(i,j)		+=	_cos * radial;
				//filter_cos(i,j)	=	1;
				filter_sin(i,j)		+=	_sin * radial;
			}
		}

	for( i=0; i<filter_cos.size1(); i++ )
		for( j=0; j<filter_cos.size2(); j++ )
		{
			if( count_map(i,j) != 0 )
			{
				filter_cos(i,j)		/=	count_map(i,j);	
				filter_sin(i,j)		/=	count_map(i,j);
			}
		}


	sprintf( str, "FFT/%d_cos.bmp", n );
	output_map( filter_cos, str );

	sprintf( str, "FFT/%d_sin.bmp", n );
	output_map( filter_sin, str );

}



/*=======================================================================
	��X Real part
/*=======================================================================*/
void	FFT::output_Real()
{
	double	max,	min,	mag;
	int		i,	j;

	QImage	img		=	image;
	
	max		=	0;
	min		=	123456;

	for( i=0; i<R.size1(); i++ )
		for( j=0; j<R.size2(); j++ )
		{
			if( max < R(i,j) )
				max	=	R(i,j);
			if( min > R(i,j) )
				min	=	R(i,j);
		}

	//image.setwidth_height( R.size1(), R.size2() );

	for( i=0; i<R.size1(); i++ )
		for( j=0; j<R.size2(); j++ )
		{
			mag		=	255.0 * ( R(i,j) - min ) / ( max - min );
			img.gray_channel( i, j,	(int)mag );
		}

	//image.save_image("fourier_R.bmp");
	img.save("FFT/fourier_R.bmp");
}


/*=======================================================================
	 ��J�}�C ��X
/*=======================================================================*/
void	FFT::output( boost::numeric::ublas::matrix<double> out )
{
	double	max,	min,	mag;
	int		i,	j;

	QImage	img		=	image;
	
	max		=	0;
	min		=	123456;

	for( i=0; i<out.size1(); i++ )
		for( j=0; j<out.size2(); j++ )
		{
			if( max < out(i,j) )
				max	=	out(i,j);
			if( min > out(i,j) )
				min	=	out(i,j);
		}

//	image.setwidth_height( out.size1(), out.size2() );

	for( i=0; i<out.size1(); i++ )
		for( j=0; j<out.size2(); j++ )
		{
			mag		=	255.0 * ( out(i,j) - min ) / ( max - min );
			img.gray_channel( i, j,	(int)mag );
		}

	img.save("FFT/output.bmp");

	//image.save_image("out.bmp");
}


/*=======================================================================
	subpixel junction  
	�|�b�o��j�M junction��max  
	�M��A�h���p��
/*=======================================================================*/
void	FFT::subpixel_junction( PointInt lu, PointInt rd )
{
	/*itepolation( 358, 608 );
	itepolation( 357, 610 );
					  
	itepolation( 362, 604 );
	itepolation( 353, 614 );
	itepolation( 362, 615 );
	itepolation( 353, 604 );

	return	;*/


	int		i,	j;
	int		max	=	0;
	int		max_i	=	-1;
	int		max_j	=	-1;

	for( i = lu.x;  i <= rd.x;  i++  )
		for( j = lu.y;  j <= rd.y;  j++  )
		{
			if( max < junction.gray_channel( i, j ) )
			{
				max		=	junction.gray_channel( i, j );
				max_i	=	i;
				max_j	=	j;
			}

		}
	if( max_i != -1 && max_j != -1 )
		itepolation( max_i, max_j );

	// �j�M�ĤG����
	max		=	0;

	int		max_i2	=	-1;
	int		max_j2	=	-1;
	for( i = lu.x;  i <= rd.x;  i++  )
		for( j = lu.y;  j <= rd.y;  j++  )
		{
			if( max < junction.gray_channel( i, j ) )
			{
				if( i != max_i && j != max_j )
				{
					max		=	junction.gray_channel( i, j );
					max_i2	=	i;
					max_j2	=	j;
				}
			}

		}
	if( max_i2 != -1 && max_j2 != -1 )
		itepolation( max_i2, max_j2 );

	//itepolation( 560, 450 );

}



/*=======================================================================
	 ��J�}�C ��X
	 ���w�ɦW
/*=======================================================================*/
void	FFT::output_map( DoubleMatrix out, char *str )
{
	double	max,	min,	mag;
	int		i,	j;

	QImage	img		=	image;
	
	max		=	0;
	min		=	123456;

	for( i=0; i<out.size1(); i++ )
		for( j=0; j<out.size2(); j++ )
		{
			if( max < out(i,j) )
				max	=	out(i,j);
			if( min > out(i,j) )
				min	=	out(i,j);
		}

	//image.setwidth_height( out.size1(), out.size2() );

	for( i=0; i<out.size1(); i++ )
		for( j=0; j<out.size2(); j++ )
		{
			mag		=	255.0 * ( out(i,j) - min ) / ( max - min );
			img.gray_channel( i, j,	(int)mag );
		}

	img.save(str);
}


/*=======================================================================
	 ��X Image part
/*=======================================================================*/
void	FFT::output_Image()
{
	double	max,	min,	mag;
	int		i,	j;

	QImage		img		=	image;
	
	max		=	0;
	min		=	123456;

	for( i=0; i<I.size1(); i++ )
		for( j=0; j<I.size2(); j++ )
		{
			if( max < I(i,j) )
				max	=	I(i,j);
			if( min > I(i,j) )
				min	=	I(i,j);
		}


	for( i=0; i<I.size1(); i++ )
		for( j=0; j<I.size2(); j++ )
		{
			mag		=	255.0 * ( I(i,j) - min ) / ( max - min );
			img.gray_channel( i, j,	(int)mag );
		}

	image.save("FFT/fourier_I.bmp");
}


/*=======================================================================
	FFT�f�ഫ
/*=======================================================================*/
void	FFT::c2_IFFT()
{
	int		i,	j,	k;
	double	mag;

	fftw_complex	*dft;
	fftw_complex	*out;

	fftw_plan		plan_b;

	dft		=	(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out		=	(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);	

	plan_b	=	fftw_plan_dft_2d(w, h, dft, out, FFTW_BACKWARD, FFTW_ESTIMATE);

	/* Populate input data in row-major order */
	for (i = 0, k = 0; i < w; i++)
		for (j = 0; j < h; j++, k++)
		{
			dft[k][0]	=	R( i, j );
			dft[k][1]	=	I( i, j );
		}
	
	/* Forward & inverse DFT */
	fftw_execute(plan_b);

	/* Convert DFT result to output image */
	for ( i = 0, k = 0; i < w; i++)
		for (j = 0; j < h; j++, k++)
		{
			mag		=	out[k][0] / N;
			//image.gray_channel( i, j, (int)mag );
		}

	//image.save_image("output.bmp");

	/* Free memory */
	fftw_destroy_plan(plan_b);

	fftw_free(dft);
	fftw_free(out);

}



/*=======================================================================
	�� FFT �ഫ
/*=======================================================================*/
void	FFT::c2_FFT()
{
	fftw_complex	*in,	*dft;		
	fftw_plan		plan_f;				
	int				i, j, k;
	double			mag;

	/* Allocate input data for FFTW */
	in		=	(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	dft		=	(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	/* Create plans */
	plan_f	=	fftw_plan_dft_2d(w, h, in, dft, FFTW_FORWARD, FFTW_ESTIMATE);


	/* Populate input data in row-major order */
	for (i = 0, k = 0; i < w; i++)
		for (j = 0; j < h; j++, k++)
		{
			in[k][0] = image.gray_channel( i, j );
			in[k][1] = 0.0;

		}

	/* Forward & inverse DFT */
	fftw_execute(plan_f);

	// �N���ͪ���Ʀs��� R I �̭�
	R.resize( w, h );
	I.resize( w, h );

	k	=	0;
	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			// Real part
			mag	=	dft[k][0];
			R( i, j )	=	mag;

			// Image part
			mag	=	dft[k][1];
			I( i, j )	=	mag;

			k++;
		}

	/* Free memory */
	fftw_destroy_plan(plan_f);

	fftw_free(in);
	fftw_free(dft);

}

#if 1
/*=======================================================================
	����filter  (�i�H���ܦh���P���o�i��,��ɭԫ��ӽs���ӵ�)
/*=======================================================================*/
void	FFT::filter( int n, DoubleMatrix  &FR, DoubleMatrix  &FI )
{
	filter_cos.resize( image.width(), image.height() );
	filter_sin.resize( image.width(), image.height() );
	filter_cos.clear();
	filter_sin.clear();

	int			i,	j,	x,	y;
	int			w,	h,	size;
	double		xx,	yy;
	double		phi;
	double		radial;

	size	=	(image.width() > image.height() ) ? image.width() : image.height();
	size	=	(size+1) / 2;
	w		=	image.width();
	h		=	image.height();



	for( i = -w; i < w;  i++  )
		for( j = -h; j < h; j++ )
		{
			xx	=	i;
			yy	=	j;

			// �קK�O������~
			if( i>w/2 || j>h/2 || i<-(w+1)/2+1 || j<-(h+1)/2+1 ){}

			else
			{
				x		=	(i<0) ? w+i : i;
				y		=	(j<0) ? h+j : j;

				// �p�� cos n*phi    sin n*phi
				phi		=	atan2( yy, xx );
				phi		*=	n;
				
				radial	=	Kp( sqrt(xx*xx + yy*yy) );

				filter_cos( x, y )	=	cos( phi ) * radial;
				filter_sin( x, y )	=	sin( phi ) * radial;
			}
		}


	FR	=	filter_cos;
	FI	=	filter_sin;

	/*QImage	image;
	image	=	Matrix2Image( FR, M2I::normalized );
	image.save("fr.bmp");
	image	=	Matrix2Image( FI, M2I::normalized );
	image.save("fi.bmp");*/


}
#endif

#if 0
/*=======================================================================
	����filter  (�i�H���ܦh���P���o�i��,��ɭԫ��ӽs���ӵ�)
/*=======================================================================*/
void	FFT::filter( int n, DoubleMatrix  &FR, DoubleMatrix  &FI )
{
	IntMatrix			count_map;				// �p���map�̭�  �C��pixel ���h�֭��I

	count_map.resize( image.width(), image.height() );
	filter_cos.resize( image.width(), image.height() );
	filter_sin.resize( image.width(), image.height() );
	count_map.clear();
	filter_cos.clear();
	filter_sin.clear();

	int			i,	j;
	int			w,	h;
	int			size;
	double		x,	y,	xx,	yy;
	double		r,	phi;
	double		value,	radial;
	double		_cos,	_sin;				// cos, sin  ���F�קK��function�� cos()  sin()�d�V.
	double		Delta;						// phi���W�[�T��

	size	=	(image.width() > image.height() ) ? image.width() : image.height();
	//size	=	(size+1) / 2;
	w		=	image.width();
	h		=	image.height();


	// �η��y�ШӲ���filter
	for( r = 0.002; r <= size; r += 0.002 )
	{
		progress->setGress( 100.0*r/size );

		Delta	=	(r-0.002)*0.018/size + 0.02;
		for( phi = 0; phi < 2*pi; phi += Delta/*0.002*/ )
		{
			x		=	r * cos(phi) ;
			y		=	r * sin(phi) ;
			xx		=	fabs(x);				// �Υ��ȥh���|�ˤ��J�A��^�쥻��sign
			yy		=	fabs(y);
			i		=	(int) (xx + 0.5);
			j		=	(int) (yy + 0.5);

			// �קK�O������~
			if( i>w/2 || j>h/2 || i<-(w+1)/2+1 || j<-(h+1)/2+1 ){}

			else
			{
				i		*=	( x>=0 ) ? 1 : -1;
				j		*=	( y>=0 ) ? 1 : -1;		//  ��o�䬰�� �o�쪺�O (-w/2 ~ w/2���y��)

				i		=	(i<0) ? w+i : i;
				j		=	(j<0) ? h+j : j;

				// �p�� cos n*phi    sin n*phi
				_cos	=	cos( n*phi );
				_sin	=	sin( n*phi );
				
				radial	=	Kp( r );

				count_map( i, j )++;							// �p����I�|���h���I,�̫᮳�h��  normalized
				filter_cos(i,j)		+=	_cos * radial;
				//filter_cos(i,j)	=	1;
				filter_sin(i,j)		+=	_sin * radial;
			}
		}
	}

	for( i=0; i<filter_cos.size1(); i++ )
		for( j=0; j<filter_cos.size2(); j++ )
		{
			if( count_map(i,j) != 0 )
			{
				filter_cos(i,j)		/=	count_map(i,j);	
				filter_sin(i,j)		/=	count_map(i,j);
			}
		}

	FR	=	filter_cos;
	FI	=	filter_sin;

	QImage	image;
	image	=	Matrix2Image( FR, M2I::normalized );
	image.save("fr.bmp");
	image	=	Matrix2Image( FI, M2I::normalized );
	image.save("fi.bmp");

}
#endif



/*=======================================================================
	�f�ഫ�^��
/*=======================================================================*/
boost::numeric::ublas::matrix<double>	FFT::inverse( boost::numeric::ublas::matrix<double> tR, boost::numeric::ublas::matrix<double> tI )
{	
	int		i,	j,	k;
	double	mag;

	boost::numeric::ublas::matrix<double>	r;
	r.resize( w, h );

	fftw_complex	*dft;
	fftw_complex	*out;

	fftw_plan		plan_b;

	dft		=	(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out		=	(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);	

	plan_b	=	fftw_plan_dft_2d(w, h, dft, out, FFTW_BACKWARD, FFTW_ESTIMATE);

	/* Populate input data in row-major order */
	for (i = 0, k = 0; i < w; i++)
		for (j = 0; j < h; j++, k++)
		{
			dft[k][0]	=	tR( i, j );
			dft[k][1]	=	tI( i, j );
		}
	
	/* Forward & inverse DFT */
	fftw_execute(plan_b);

	/* Convert DFT result to output image */
	for ( i = 0, k = 0; i < w; i++)
		for (j = 0; j < h; j++, k++)
		{
			mag			=	1.0 * out[k][0] / N;
			r( i, j )	=	mag;
		}

	/* Free memory */
	fftw_destroy_plan(plan_b);

	fftw_free(dft);
	fftw_free(out);

	return	r;
}



/*=======================================================================
	����filter���ʧ@
/*=======================================================================*/
void	FFT::do_filter( int n,  boost::numeric::ublas::matrix<double> FR, boost::numeric::ublas::matrix<double> FI, boost::numeric::ublas::matrix<double> &C1, boost::numeric::ublas::matrix<double> &C2 )
{
	int		i,	j;

	boost::numeric::ublas::matrix<double>	tR,	tI;		// �Ȧs fourier domain �U�����

	tR.resize( w, h );
	tI.resize( w, h );

	if( n == 0 )
	{
		// �� Real ����
		for( i=0; i<w; i++ )
			for( j=0; j<h; j++ )
			{
				tR( i, j )	=	R( i, j ) * FR( i, j );
				tI( i, j )	=	I( i, j ) * FR( i, j );
				//tR( i, j )	=	R(i,j) * FR(i,j) - I(i,j)*FI(i,j);
				//tI( i, j )	=	I(i,j) * FR(i,j) + R(i,j)*FI(i,j);
			}

		C1	=	inverse( tR, tI );		// �f�ഫ�^��
		output_map( C1 , "FFT/0_C1.bmp" ) ;

		//  �� Image ����
		for( i=0; i<w; i++ )
			for( j=0; j<h; j++ )
			{
				tR( i, j )	=	 R( i, j ) * FI( i, j );
				tI( i, j )	=	 I( i, j ) * FI( i, j );
			}

		C2	=	inverse( tR, tI );		// �f�ഫ�^��
		output_map( C2 , "FFT/0_C2.bmp" );
	}

	else if( n == 1 )
	{
		// �� Real ����
		for( i=0; i<w; i++ )
			for( j=0; j<h; j++ )
			{
				tR( i, j )	=	-I( i, j ) * FR( i, j );
				tI( i, j )	=	 R( i, j ) * FR( i, j );

				//tR( i, j )	=	 R( i, j ) * FR( i, j );
				//tI( i, j )	=	 I( i, j ) * FR( i, j );

				//tR( i, j )	=	R(i,j) * FR(i,j) - I(i,j)*FI(i,j);
				//tI( i, j )	=	I(i,j) * FR(i,j) + R(i,j)*FI(i,j);
			}

		C1	=	inverse( tR, tI );		// �f�ഫ�^��
		output_map( C1, "FFT/1_C1.bmp" );

		//  �� Image ����
		for( i=0; i<w; i++ )
			for( j=0; j<h; j++ )
			{
				tR( i, j )	=	 -I( i, j ) * FI( i, j );
				tI( i, j )	=	 R( i, j ) * FI( i, j );

				//tR( i, j )	=	 R( i, j ) * FI( i, j );
				//tI( i, j )	=	 I( i, j ) * FI( i, j );
			}

		C2	=	inverse( tR, tI );		// �f�ഫ�^��
		output_map( C2, "FFT/1_C2.bmp"  );
	}

	else if( n == 2 )
	{
		// �� Real ����
		for( i=0; i<w; i++ )
			for( j=0; j<h; j++ )
			{
				tR( i, j )	=	R( i, j ) * FR( i, j );
				tI( i, j )	=	I( i, j ) * FR( i, j );
				//tR( i, j )	=	R(i,j) * FR(i,j) - I(i,j)*FI(i,j);
				//tI( i, j )	=	I(i,j) * FR(i,j) + R(i,j)*FI(i,j);
			}

		C1	=	inverse( tR, tI );		// �f�ഫ�^��
		output_map( C1 , "FFT/2_C1.bmp" );

		//  �� Image ����
		for( i=0; i<w; i++ )
			for( j=0; j<h; j++ )
			{
				tR( i, j )	=	R( i, j ) * FI( i, j );
				tI( i, j )	=	I( i, j ) * FI( i, j );
			}

		C2	=	inverse( tR, tI );		// �f�ഫ�^��
		output_map( C2 , "FFT/2_C2.bmp" );
	}

}


/*=======================================================================
	���ե� ���Y�ǰʧ@
/*=======================================================================*/
void	FFT::doing()
{
	int		i,	j;
	double	tmp;
	double	E0,		E1,		E2;
	double	min,	max;
	double	c0,	c11,	c12,	c21,	c22;

	boost::numeric::ublas::matrix<double>	FR,	FI;
	boost::numeric::ublas::matrix<double>	C_0_1,	C_0_2,	C_1_1,	C_1_2,	C_2_1,	C_2_2;		// ���ͪ��Y�� (�w�g�fFFT�^�ӤF)
	
	// 0-th
	filter( 0, FR, FI );
	do_filter( 0, FR, FI, C_0_1, C_0_2 );
	progress->setGress( 15 );

	// 1-th
	filter( 1, FR, FI );
	do_filter( 1, FR, FI, C_1_1, C_1_2 );
	progress->setGress( 30 );

	// 2-th
	filter( 2, FR, FI );
	do_filter( 2, FR, FI, C_2_1, C_2_2 );
	progress->setGress( 45 );

	QImage	img		=	image;
	
	boost::numeric::ublas::matrix<double>	Boundary,	Edge,	Junction;
	Boundary.resize( w, h );
	Edge.resize( w, h );
	Junction.resize( w, h );

	// boundary����
	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			//E0	=	C_0_1(i,j) * C_0_1(i,j) + C_0_2(i,j) * C_0_2(i,j);
			//E1	=	C_1_1(i,j) * C_1_1(i,j) + C_1_2(i,j) * C_1_2(i,j);
			//E2	=	C_2_1(i,j) * C_2_1(i,j) + C_2_2(i,j) * C_2_2(i,j);
			c0	=	C_0_1(i,j);
			c11	=	C_1_1(i,j);
			c12	=	C_1_2(i,j);
			c21	=	C_2_1(i,j);
			c22	=	C_2_2(i,j);	

			tmp	=	c11*c11 + 0.25*( (c0+c21)*(c0+c21) + c22*c22 ) + c12*c12 + 0.25*( (c0-c21)*(c0-c21) + c22*c22 );
			//tmp		=	0.5*E0 + E1 + 0.5*E2;
			//image.gray_channel( i, j, tmp );
			Boundary( i, j )	=	tmp;
		}

	min		=	1234567;
	max		=	0;

	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			if( min > Boundary(i,j) )
				min	=	Boundary(i,j);
			if( max < Boundary(i,j) )
				max	=	Boundary(i,j);
		}

	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			tmp	=	255.0 * (Boundary(i,j)-min) / ( max - min );
			img.gray_channel( i, j, (int)tmp );
		}

	//image.save_image("boundary.bmp");
	img.save("FFT/boundary.bmp");
	boundray	=	img;
	progress->setGress( 60 );

	// Edge ����
	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			c0	=	C_0_1(i,j);
			c11	=	C_1_1(i,j);
			c12	=	C_1_2(i,j);
			c21	=	C_2_1(i,j);
			c22	=	C_2_2(i,j);

			tmp	=	( c11*c11 - c12*c12 + c0*c21 ) * ( c11*c11 - c12*c12 + c0*c21 ) + ( 2.0*c11*c12 + c0*c22 ) * ( 2.0*c11*c12 + c0*c22 ) ;
			Edge( i, j )	=	sqrt(tmp);
		}

	min		=	1234567;
	max		=	0;

	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			if( min > Edge(i,j) )
				min	=	Edge(i,j);
			if( max < Edge(i,j) )
				max	=	Edge(i,j);
		}

	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			tmp	=	255.0 * (Edge(i,j)-min) / ( max - min );
			img.gray_channel( i, j, (int)tmp );
		}

	//image.save_image("edge.bmp");
	img.save("FFT/edge.bmp");
	edge	=	img;
	progress->setGress( 80 );

	// Junction ����

	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
			Junction( i, j )	=	Boundary(i,j) - Edge(i,j) ;

	min		=	1234567;
	max		=	0;

	//progress->debuger( QString("%1 %2 ").arg( Junction( 353, 614 ) ).arg( Junction( 352, 615 ))  );

	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			if( min > Junction(i,j) )
				min	=	Junction(i,j);
			if( max < Junction(i,j) )
				max	=	Junction(i,j);
		}

	for( i=0; i<w; i++ )
		for( j=0; j<h; j++ )
		{
			tmp	=	255.0 * (Junction(i,j)-min) / ( max - min );
			img.gray_channel( i, j, (int)tmp );

			//if( tmp > 10 )
			//	image.pixel_channel( i, j, 255, 0, 0 );
		}

	img.save("FFT/junction.bmp");
	junction	=	img;
	progress->setGress( 99 );

}




#ifdef _OPENPIV_
/*=======================================================================
	openPIV
	�{�Ǫ��i�J�I
/*=======================================================================*/
void	FFT::dosomething()
{
	// �]�w�D�e��tab����r����
	progress->setTabText( 2, "boundary" );
	progress->setTabText( 3, "edge" );

	// ��l�ʧ@
	image	=	filedata->get_image();



	// normalized
	/*double	max	=	0;
	double	min	=	1000;
	double	mag;
	for( int i=0; i<image.width(); i++ )
		for( int j=0; j<image.height(); j++ )
		{
			if( max < image.gray_channel( i, j ) )
				max		=	image.gray_channel( i, j );
			if( min > image.gray_channel( i, j) )
				min		=	image.gray_channel( i, j);
		}
	for( int i=0; i<image.width(); i++ )
		for( int j=0; j<image.height(); j++ )
		{
			mag		=	255.0 * ( 1.0*image.gray_channel(i,j) - min ) / (max-min);
			image.gray_channel( i,j, (int)mag);
		}
		image.save("normalized.bmp");*/





	w		=	image.width();
	h		=	image.height();
	N		=	w * h;

	c2_FFT();							// �N�Ϥ����ť߸��ഫ
	output_Real();						// ��Xreal part
	output_Image();						// ��Ximage part

	doing();							// �i��junction detector

	//c2_IFFT();						// �N�ɮװ��ť߸��f�ഫ

	junction.save("junction.bmp");		// �ɮ׿�X

	PointInt	lu(544,441),	rd( 578, 479 );
	//subpixel_junction( lu, rd );

	//emit( filedata->output(junction) );
	//emit( filedata->output3(boundray) );
	//emit( filedata->output4(edge) );

	progress->outputImage( 0, junction );
	progress->outputImage( 2, boundray );
	progress->outputImage( 3, edge );

	progress->debuger("<font color=\"Red\"> finish FFT Junction detector... </font>");

}

#else
/*=======================================================================
	main function
	���DopenPIV�M�רϥ�
/*=======================================================================*/
int _tmain(int argc, _TCHAR* argv[])
{
	FFT		fft;


	// ���ժ��� ����filter
	//fft.c2_FFT();
	//fft.draw_filter(2);
	//fft.doing2();
	//fft.c2_IFFT();

	fft.c2_FFT();
	fft.output_Real();
	fft.output_Image();

	fft.doing();

	// �N���G��X
	fft.c2_IFFT();

 

	//system("PAUSE");
	return 0;
}
#endif

