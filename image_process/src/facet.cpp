// facet.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
#include "facet.h"
#include <fstream>


using namespace std;



/*======================================================================================
	�D�̤j���]��
/*======================================================================================*/
int		GCD( int a, int b )
{
	if( a<0 )
		a	*=	-1;
	if( b<0 )
		b	*=	-1;

	if( a%b == 0 )
		return	b;
	else
		return	GCD(b,a%b);

	return	1;
}



/*======================================================================================
	�غc�禡
	n = 2   ���   �i�}��   2��  3*3
	n = 3   ���   �i�}��   3��  5*5
/*======================================================================================*/
Facet::Facet( int n )
{
	if( n < 2 )
		cout << "error!!!!!!!!!!";

	int		i,	j,	sum;
	int		s;
	int		x,	y;
	int		w,	h;
	int		count1,	count2;

	boost::numeric::ublas::matrix<double>	A,	AT,	AT_A,	inv;			// A   A^T   A^T*A    A^-1    MPA = Moore-Penrose inverse  A^+
	boost::numeric::ublas::vector<double>	f;


	N		=	n;
	sum		=	0;
	for( i=1; i<=(N+1); i++ )
		sum	+=	i;

	w	=	(2*N-1)*(2*N-1);
	h	=	sum;

	A.resize( w, h );
	W.resize( w, w );		// weight matrix
	f.resize( w );

	// �]�wweight function
	W.clear();
	/*for( i=0; i<W.size1(); i++ )
		W(i,i)	=	1;*/
	//W(13,13)	=	1;

	int		count	=	0;
	for( i = -2; i <= 2; i++ )
		for( j = -2; j <= 2; j++ )
		{
			W( count, count )	=	exp(  -1.0*(i*i+j*j)/(2.0*1.3333333333333)  );
			count++;
		}
	// normalized
	double	normal	=	0;
	for( i=0; i<25; i++ )
		normal	+=	W(i,i);
	for( i=0; i<25; i++ )
		W(i,i)	/=	normal;




	/*
		�o�䪺���k�O�o��
		���M�w F(x,y)   �|�� �q -1 0 1    
		���U�ӧQ�� i^n j^m   n+m = const   �h��
		n+m = 1   n+m = 2...��case
		�A���Ʀs�^ A
	*/
	count1	=	0;

	for( x=-(N-1); x<=(N-1); x++ )
		for( y=-(N-1); y<=(N-1); y++ )
		{
			// �q i^n j^m     n+m = s   �}�l���W
			count2	=	0;
			for( s=0; s<=N; s++ )
				for( i=s; i>=0; i-- )
					A( count1 , count2++ )	=	power( x, i, y, s-i );
			
			count1++;
		}

	// ��m trans
	AT		=	trans( A );

	// A^T * A
	AT_A	=	prod( AT, A );
	AT_A	=	prod( AT, W );
	AT_A	=	prod( AT_A, A);

	// (A^T * A)^-1
	inv		=	inverse( AT_A );

	// Moore-Penrose inverse A^+
	MPA		=	prod( inv , AT );
	MPA		=	prod( MPA, W );


	//MPA		/=	2*2*5*5*3*7;

}




/*======================================================================================
	�D�x�}inverse  ��lu����
/*======================================================================================*/
boost::numeric::ublas::matrix<double>	Facet::inverse( boost::numeric::ublas::matrix<double> A )
{
	int		n	=	A.size1();

	boost::numeric::ublas::matrix<double>	A_inverse;

	A_inverse.resize( n, n );

	boost::numeric::ublas::matrix<double>							_A( A );
	boost::numeric::ublas::permutation_matrix<std::size_t>			pm( _A.size1() );

	int		res	=	lu_factorize( _A, pm );
	if( res != 0 )
		cout << "error....!!????";

	A_inverse.assign( boost::numeric::ublas::identity_matrix<double> ( _A.size1() ) );

	lu_substitute( _A, pm, A_inverse );

	return	A_inverse;
}




/*======================================================================================
	�D��
/*======================================================================================*/
bool	Facet::edge( double x, double y, QImage image, double &rtx, double &rty )
{
	boost::numeric::ublas::vector<double>	p;

	int		i,	s,	sum,	count;

	double	xx	=	(int) x + 0.5;		// 4.5�o�˪� �B�I�Ƥ����I�y��
	double	yy	=	(int) y + 0.5;

	double	dx	=	x - xx;
	double	dy	=	y - yy;

	double	C0,	C1,	C2,	C3;
	double	sin,	cos;

	p	=	find_p( x, y, image );

	sin	=	p(1) / sqrt( p(1)*p(1) + p(2)*p(2) );
	cos	=	p(2) / sqrt( p(1)*p(1) + p(2)*p(2) );

	C0	=	p(0);
	C1	=	p(1)*sin + p(2)*cos;
	C2	=	p(3)*sin*sin + p(4)*sin*cos + p(5)*cos*cos;
	C3	=	p(6)*sin*sin*sin + p(7)*sin*sin*cos + p(8)*sin*cos*cos + p(9)*cos*cos*cos;
	
	double	rho	=	-C2/(3.0*C3);

	// �קK������ 0  �o�ر��p�첾���ܦ�0.5
	if( p(1) == 0 && p(2) == 0 )
	{
		rho	=	0;
		cos	=	0;
		sin	=	0;
	}

	dx	=	rho * cos;
	dy	=	rho * sin;

	if( fabs(dx) > 0.5 || fabs(dy) > 0.5 )
	{
		rtx	=	xx;				// �첾�Ӥj  �έ쥻���I (1.5  2.5) �o��
		rty	=	yy;
		return	false;
	}
	else
	{
		rtx	=	 xx + dx;
		rty	=	 yy + dy;
		return	true;
	}

}


/*======================================================================================
	�D�X�Y�� p
	�o��Q�� (5.5 4.5) �o�˪��I�����I
/*======================================================================================*/
boost::numeric::ublas::vector<double>	Facet::find_p( double x, double y, QImage image )
{
	double	xx	=	(int) x + 0.5;		// 4.5�o�˪� �B�I�Ƥ����I�y��
	double	yy	=	(int) y + 0.5;

	int		px	=	(int) xx;			// ����I�y��
	int		py	=	(int) yy;

	double	dx	=	x - xx;
	double	dy	=	y - yy;

	int		i,	j,	count;

	boost::numeric::ublas::vector<double>	p;
	boost::numeric::ublas::vector<double>	f;
	f.resize( (2*N-1) * (2*N-1) );

	if( !(px > 0 && px < image.width()-1 && py > 0 && py < image.height()-1) )
		cout << "error...!!!!!";

	count	=	0;
	for( i=-(N-1); i<=(N-1); i++ )
		for( j=-(N-1); j<=(N-1); j++ )
			f(count++)	=	qGray( image.pixel( px+i, py+j ) );

	p	=	prod( MPA, f );

	return	p;
}



/*======================================================================================
	�o�� sub pixel ��������
/*======================================================================================*/
double	Facet::get_subpixel( double x, double y, QImage image )
{
	boost::numeric::ublas::vector<double>	p;

	int		i,	s,	sum,	count;

	double	xx	=	(int) x + 0.5;		// 4.5�o�˪� �B�I�Ƥ����I�y��
	double	yy	=	(int) y + 0.5;

	double	dx	=	x - xx;
	double	dy	=	y - yy;

	p	=	find_p( x, y, image );

	//for( i=0; i<p.size(); i++ )
	//	cout << p(i) << " ";
	//cout << endl;

	sum		=	0;
	count	=	0;
	
	// �@��  �Q�� s = n+m    i^n * j^m���ޥ�
	for( s=0; s<=N; s++ )
		for( i=s; i>=0; i-- )
			sum		+=	p(count++) * power( dx, i, dy, s-i );

	return	sum;
}



/*======================================================================================
	�D i^n j^m
/*======================================================================================*/
int		Facet::power( int i, int n, int j, int m )
{
	int		r	=	1;
	int		ii;

	if( n*log(1.0*i) + m*log(1.0*j) >= log( 1.0*INT_MAX ) )
		cout << "error...!!";

	for( ii=0; ii<n; ii++ )
		r	*=	i;

	for( ii=0; ii<m; ii++ )
		r	*=	j;

	return	r;
}



/*======================================================================================
	�D i^n j^m
/*======================================================================================*/
double		Facet::power( double i, int n, double j, int m )
{
	double		r	=	1;
	int			ii;

	for( ii=0; ii<n; ii++ )
		r	*=	i;

	for( ii=0; ii<m; ii++ )
		r	*=	j;

	return	r;
}




