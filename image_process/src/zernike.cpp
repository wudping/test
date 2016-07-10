#include "stdafx.h"

#include "zernike.h"




/*=================================================
	�غc���
/*=================================================*/
Zernike::Zernike()
{
	PI	=	3.14159265358979323846;
	N	=	12;

	// ���͸��
	generate_pdata();
}




/*=================================================
	���Ͷ��h����
	�u�వ�� 12!   
	�W�L���ܭn���S��B�z,���M�|overflow
/*=================================================*/
void	Zernike::factorial_list()
{
	ofstream	out( "factorial.txt" );

	int			i;
	int			fac;
	const int	n	=	N;	// �w�]����12���h

	fac_list.resize( n+1 );
	fac_list.clear();

	out << "1\n" ;				// 0! = 1
	fac_list(0)		=	1;

	for( i=1, fac=1; i<=n; i++ )
	{
		fac				*=	i;
		fac_list( i )	=	fac;
		out << fac << endl;
	}

	out.close();
}



/*=================================================
	Ū�����h����ƨ�O���餤
/*=================================================*/
void	Zernike::factorial_load()
{
	ifstream	in( "factorial.txt" );
	int			i;

	fac_list.resize( N+1 );

	for( i=0; i<=N; i++ )
		in >> fac_list(i);

	in.close(); 
}


/*=================================================
	����r����
	r = (-1)^l (n-l)! / (...)
	for n-m ����
/*=================================================*/
void	Zernike::r_list()
{
	ofstream	out("r_list.txt");

	int			n,	m,	l;
	int			i,	j;
	int			PN	=	1;			// �M�w���t   -1^l
	int			mag;

	// ��l�� r_data
	r_data.resize( N+1, N+1 );
	for( i = 0; i < r_data.size1(); i++ )
		for( j = 0; j < r_data.size2(); j++ )
			r_data( i, j ).resize( N+1 );

	for( n = 0; n <= N; n++ )
		for( m = 0; m <= N; m++ )
		{
			if( (n-m)%2 == 1 )	continue;

			PN	=	1;						// �Q��l��l�� = 0
			for( l=0; l<=(n-m)/2; l++ )
			{
				//out << PN * fac_list[n-l] / fac_list[l] / fac_list[ (n+m)/2 - l ] / fac_list[ (n-m)/2 - l ] << "\n";
				//cout << "(" << n << "," << m << "," << l << ") = " ;
				mag		=	PN * fac_list[n-l] / fac_list[l] / fac_list[ (n+m)/2 - l ] / fac_list[ (n-m)/2 - l ];		// �O�o�o��u�|�O���
				PN		*=	-1;

				r_data( n, m )( l )		=	mag;
				out << mag << endl;
			}

		}

	out.close();
}


/*=================================================
	Ū�� r �����
/*=================================================*/
void	Zernike::r_load()
{
	ifstream	in("r_list.txt");

	int		n,	m,	l;
	int		i,	j;
	int		tmp;
	//int		PN	=	1;			// �M�w���t   -1^l

	// ��l�� r_data
	r_data.resize( N+1, N+1 );
	for( i = 0; i < r_data.size1(); i++ )
		for( j = 0; j < r_data.size2(); j++ )
			r_data( i, j ).resize( N+1 );


	for( n=0; n<=N; n++ )
		for( m=0; m<=N; m++ )
		{
			if( (n-m)%2 == 1 )	continue;

			//PN	=	1;						// �Q��l��l�� = 0
			for( l=0; l<=(n-m)/2; l++ )
			{
				//cout << "(" << n << "," << m << "," << l << ") = " ;
				//out << PN * fac[n-l] / fac[l] / fac[ (n+m)/2 - l ] / fac[ (n-m)/2 - l ] << "\n";
				//PN	*=	-1;
				in >> tmp;
				r_data( n, m )( l )		=	tmp;

				//*( r_data + n*(N+1)*(N+1) + m*(N+1) + l )	=	tmp;
			}

		}

	in.close();
}



/*=================================================
	�^��R_nm(x,y)����
	x^2 + y^2 = radios^2    radios = rho
	�o��n�d�N, �p�G�����I���O(0,0)
	�n���y���ഫ
	x' = x-Rx
	y' = y-Ry  
	R �O���
/*=================================================*/
double	Zernike::R_nm( int n, int m, double x, double y )
{
	const double	rho		=	sqrt(x*x + y*y);
	double			r;
	double			sum;

	int		l;

	// �o��ϥΪ��O n-m �_�� Rnm = 0���w�q
	if( (n-m)%2 == 1 )
		return	0;

	l	=	(n-m)/2;
	r	=	pow( rho, n-2*l );


	for( sum=0; l>=0; l-- )
	{	
		//cout << *( r_data + n*(N_MAX+1)*(N_MAX+1) + m*(N_MAX+1) + l ) << "*r^" << (n-2*l) << " ";
		//sum	+=	*( r_data + n*(N+1)*(N+1) + m*(N+1) + l ) * r;
		sum		+=	r * r_data( n, m )( l );
		r		*=	(rho*rho);

	}
	
	return	sum;
}



/*=================================================
	���͸��  
	���h  r table �o���
/*=================================================*/
void	Zernike::generate_pdata()
{
	factorial_list();
	r_list();
}

/*=================================================
	Ū�����  
	���h r table �o���
/*=================================================*/
void	Zernike::load_pdata()
{
	factorial_load();
	r_load();
}



/*=================================================
	�^�� V_nm(x,y) ����
	x^2 + y^2 = radios^2    radios = rho
	�o��n�d�N, �p�G�����I���O(0,0)
	�n���y���ഫ
	x' = x-Rx
	y' = y-Ry  
	R �O���
/*=================================================*/
complex<double>		Zernike::V_nm( int n, int m, double x, double y )
{
	double	theta;   //	=	atan( y/x );
	double	real,	image;

	if( x == 0 && y == 0 )
	{
		//v	=	complex<double> (0, 0);
		return	complex<double> (0, 0);;
	}

	/*else if( x == 0 && y > 0 )
		theta	=	PI/2;

	else if( x == 0 && y < 0 )
		theta	=	-PI/2;

	else
		theta	=	atan( y/x );*/

	theta	=	atan2( y, x );
	

	real	=	R_nm( n, m, x, y ) * cos( m*theta );
	image	=	R_nm( n, m, x, y ) * sin( m*theta );

	//v	=	complex<double> ( real, image );
	return		complex<double> ( real, image );
}


/*=================================================
	Ū�����ɸ��
/*=================================================*/
void	Zernike::loadBMP( QImage passImage )
{
	image	=	passImage;
}


/*=================================================
	����zernike moment �ɮ�
/*=================================================*/
void	Zernike::calculate_zernike( POINT center, double radius )
{
	//ofstream	out( "Zernike.txt");

	//int					n,	m;
	int					i,	j;
	//complex<double>		c;

	Z.resize( N+1, N+1 );
	for( i = 0; i < Z.size1(); i++ )
		for( j = 0; j < Z.size2(); j++ )
			Z( i, j )	=	complex<double>(0,0);

	// ����Zernike Moment�ɮ�
#pragma omp parallel for
	for( int n = 0; n <= N; n++ )
	{
		cout << "*";
		for( int m = 0; m<=n; m++ )
		{
			if( (n-m)%2 == 1 )	continue;

			complex<double>		c	=	Z_nm( n, m, center, radius );

			Z( n, m )		=	c;

			//out << n << " " << m  << "\t" << c.real() << "\t\t" << c.imag() << endl;
		}
	}

	//out.close();
}



/*=================================================
	�p�� Anm   ( zernike moment)    
	�Y�Ǧa��|�gZnm
	Anm
	���w���� �b�| �h���n�� 
	(�b�|�|����normalized)
/*=================================================*/
complex<double>		Zernike::Z_nm( int n, int m, POINT center, double radius )
{
	int					i,		j;
	double				theta,	r;
	double				x,		y;
	double				xx,		yy;
	double				real_p,	image_p;

	complex<double>		v,	c2(0,0);
	
	bMatrixInt			count_map;									// �p��C��pixel�Q�p��F�X��
	count_map.resize( image.width(), image.height() );		// ��ڤW���ܦh���I����ƬO�Τ��쪺 ���F��K�p��Ӥw
	count_map.clear();

	boost::numeric::ublas::matrix< complex<double> >	integral;
	integral.resize( image.width(), image.height() );

	for( i = 0; i < image.width(); i++ )
		for( j  = 0; j < image.height(); j++ )
			integral( i, j )	=	complex<double>(0,0);


	// �η��y�ШӰ��n��   0���κ�  �]��zernike moment�br=0   Vnm = 0
	for( r = 0.001; r <= 1; r += 0.001 )
		for( theta = 0; theta < 2*PI; theta += 0.001 )
		{
			// �o�䪺�p��O���F���C�p��~�t
			x		=	r * cos(theta) ;
			y		=	r * sin(theta) ;

			xx		=	fabs( x*radius );			// �Υ��ȥh���|�ˤ��J�A��^�쥻��sign
			yy		=	fabs( y*radius );			// radius: �Ψ��ഫ��Ϥ��W���y��
			i		=	(int) (xx + 0.5);
			j		=	(int) (yy + 0.5);

			i		*=	( x>=0 ) ? 1 : -1;
			j		*=	( y>=0 ) ? 1 : -1;

			i		+=	center.x;
			j		+=	center.y;

			if( i < 0 || i >= image.width() || j < 0 || j >= image.height() )		continue;

			v		=	V_nm( n, m, x, y );
	
			real_p	=	image.gray_channel( i, j ) * v.real();
			image_p	=	image.gray_channel( i, j ) * (-v.imag());			// ���@�m

			integral( i, j )	+=	complex<double>( real_p, image_p );
			count_map( i, j )++;
		}


	// �p�⧹�� �N�C���I���W������count
	for( i = 0; i < image.width(); i++ )
		for( j = 0; j < image.height(); j++ )
		{
			if( count_map(i,j) != 0 )	
				integral( i, j )	/=	count_map( i, j );
		}

	// ����^ 1/radius�����
	for( i = 0; i < image.width(); i++ )
		for( j = 0; j < image.height(); j++ )
		{
				integral( i, j )	*=	1.0 / (radius*radius);
		}



	// �N�����[�_�� ���n��
	c2	=	complex<double>(0,0);
	for( i = 0; i < image.width(); i++ )
		for( j = 0; j < image.height(); j++ )
		{
			if( count_map(i,j) != 0 )	
				c2	+=	integral( i, j );
		}

	c2	*=	1.0 * (n+1) / PI;

	return	c2;
}


/*=================================================
	���جY����moment�Ϲ�
/*=================================================*/
void	Zernike::recon_nm( int n, int m, POINT center, double radius )
{
	int					i,		j;
	double				theta,	r;
	double				x,		y;
	double				xx,		yy;
	double				real_p,	image_p;

	complex<double>		v,	c2(0,0);

	QImage				final( image.width(), image.height(), QImage::Format_RGB666 );
	final.fill( 0 );
	
	bMatrixInt			count_map;									// �p��C��pixel�Q�p��F�X��
	count_map.resize( image.width(), image.height() );		// ��ڤW���ܦh���I����ƬO�Τ��쪺 ���F��K�p��Ӥw
	count_map.clear();

	boost::numeric::ublas::matrix< complex<double> >	integral;
	integral.resize( image.width(), image.height() );

	for( i = 0; i < image.width(); i++ )
		for( j  = 0; j < image.height(); j++ )
			integral( i, j )	=	complex<double>(0,0);


	// �η��y�ШӰ��n��   0���κ�  �]��zernike moment�br=0   Vnm = 0
	for( r = 0.01; r <= 1; r += 0.01 )
		for( theta = 0; theta < 2*PI; theta += 0.01 )
		{
			// �o�䪺�p��O���F���C�p��~�t
			x		=	r * cos(theta) ;
			y		=	r * sin(theta) ;

			xx		=	fabs( x*radius );			// �Υ��ȥh���|�ˤ��J�A��^�쥻��sign
			yy		=	fabs( y*radius );			// radius: �Ψ��ഫ��Ϥ��W���y��
			i		=	(int) (xx + 0.5);
			j		=	(int) (yy + 0.5);

			i		*=	( x>=0 ) ? 1 : -1;
			j		*=	( y>=0 ) ? 1 : -1;

			i		+=	center.x;
			j		+=	center.y;

			if( i < 0 || i >= image.width() || j < 0 || j >= image.height() )		continue;

			v		=	V_nm( n, m, x, y );

			c2		=	Z( n, m ) * v;
	
			integral( i, j )	+=	c2;

			count_map( i, j )++;
		}

	// �p�⧹�� �N�C���I���W������count
	for( i = 0; i < image.width(); i++ )
		for( j = 0; j < image.height(); j++ )
			if( count_map(i,j) != 0 )	
				integral( i, j )	/=	count_map( i, j );


	// �N�����[�_�� ���n��
	c2	=	complex<double>(0,0);
	for( i = 0; i < image.width(); i++ )
		for( j = 0; j < image.height(); j++ )
			if( count_map(i,j) != 0 )	
				cout << integral( i, j ).real() << " " << integral( i, j ).imag() << endl;


}



/*=================================================
	���عϹ�
/*=================================================*/
void	Zernike::reconstruct( POINT center, double radius )
{



}




/*=================================================
	���եΨ��
/*=================================================*/
void	Zernike::test()
{
	int			i,	j;
	ofstream	out("test.txt");

	for( i = 0; i <= N; i++ )
	{
		for( j = 0; j <= i; j++ )
		{
			if( (i - j) % 2 != 0 )	continue;
				
			out << i << " " << j << "\t\t" << norm( Z( i, j ) ) << endl;

		}
	}

	out.close();
	
}
