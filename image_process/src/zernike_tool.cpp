#include "stdafx.h"
#include "../h/zernike_tool.h"
using namespace std;

	//const double	PI	=	3.1415926;
#define PI 3.1415926



/*=================================================
	���Ͷ��h����
	�u�వ�� 12!   
	�W�L���ܭn���S��B�z,���M�|overflow
/*=================================================*/
void	factorial_list(){

	ofstream	out( "factorial.txt" );

	int			i;
	int			fac;
	const int	n	=	N_MAX;	// �w�]����12���h

	out << "1\n" ;				// 0! = 1
	for( i=1, fac=1; i<=n; i++ ){
		fac *= i;
		out << fac << endl;
	}

	out.close();
}



/*=================================================
	Ū�����h����ƨ�O���餤
/*=================================================*/
void	factorial_load( int *a ){

	ifstream	in( "factorial.txt" );

	int		i;

	for( i=0; i<=N_MAX; i++ )
		in >> a[i];


	in.close(); 
}


/*=================================================
	����r����
	r = (-1)^l (n-l)! / (...)
	for n-m ����
/*=================================================*/
void	r_list( int *fac ){

	ofstream	out("r_list.txt");

	int		n,	m,	l;
	int		PN	=	1;			// �M�w���t   -1^l

	for( n=0; n<=N_MAX; n++ )
		for( m=0; m<=N_MAX; m++ ){

			if( (n-m)%2 == 1 )	continue;

			PN	=	1;						// �Q��l��l�� = 0
			for( l=0; l<=(n-m)/2; l++ ){
			
				//cout << "(" << n << "," << m << "," << l << ") = " ;
				out << PN * fac[n-l] / fac[l] / fac[ (n+m)/2 - l ] / fac[ (n-m)/2 - l ] << "\n";
				PN	*=	-1;
			}


		}

	out.close();
}


/*=================================================
	Ū�� r �����
/*=================================================*/
void	r_load( int *r_data ){

	ifstream	in("r_list.txt");

	int		n,	m,	l;
	int		tmp;
	//int		PN	=	1;			// �M�w���t   -1^l

	for( n=0; n<=N_MAX; n++ )
		for( m=0; m<=N_MAX; m++ ){

			if( (n-m)%2 == 1 )	continue;

			//PN	=	1;						// �Q��l��l�� = 0
			for( l=0; l<=(n-m)/2; l++ ){
			
				//cout << "(" << n << "," << m << "," << l << ") = " ;
				//out << PN * fac[n-l] / fac[l] / fac[ (n+m)/2 - l ] / fac[ (n-m)/2 - l ] << "\n";
				//PN	*=	-1;
				in >> tmp;
				*( r_data + n*(N_MAX+1)*(N_MAX+1) + m*(N_MAX+1) + l )	=	tmp;
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
double	R_nm( int n, int m, double x, double y, int *r_data ){

	const double	rho	=	sqrt(x*x + y*y);
	double			r;
	double			sum;

	int		l;

	// �o��ϥΪ��O n-m �_�� Rnm = 0���w�q
	if( (n-m)%2 == 1 )
		return	0;

	l	=	(n-m)/2;
	r	=	pow( rho, n-2*l );


	for( sum=0; l>=0; l-- ){
		
		//cout << *( r_data + n*(N_MAX+1)*(N_MAX+1) + m*(N_MAX+1) + l ) << "*r^" << (n-2*l) << " ";
		sum	+=	*( r_data + n*(N_MAX+1)*(N_MAX+1) + m*(N_MAX+1) + l ) * r;
		r	*=	(rho*rho);

	}
	
	return	sum;
}




/*=================================================
	�^��V_nm(x,y)����
	x^2 + y^2 = radios^2    radios = rho
	�o��n�d�N, �p�G�����I���O(0,0)
	�n���y���ഫ
	x' = x-Rx
	y' = y-Ry  
	R �O���
/*=================================================*/
void	V_nm( int n, int m, double x, double y, int *r_data, complex<double> &v ){


	double	theta;   //	=	atan( y/x );
	double	real,	image;

	if( x == 0 && y == 0 ){
		v	=	complex<double> (0, 0);
		return;
	}

	else if( x == 0 && y > 0 )
		theta	=	PI/2;

	else if( x == 0 && y < 0 )
		theta	=	-PI/2;

	else
		theta	=	atan( y/x );
	


	real	=	R_nm( n, m, x, y, r_data ) * cos( m*theta );
	image	=	R_nm( n, m, x, y, r_data ) * sin( m*theta );

	v	=	complex<double> ( real, image );
}