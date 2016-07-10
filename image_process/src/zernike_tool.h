#ifndef _ZERNIKE_TOOL_H
	#define _ZERNIKE_TOOL_H

#include <complex>
using namespace std;


#define		N_MAX	12




// ���Ͷ��h����
void	factorial_list();

// Ū�����h����
void	factorial_load( int *a );

// ���� r ����
void	r_list( int *fac );

// Ū�� r �����
void	r_load( int *r_data );

// �^�� R_nm(x,y) ����
double	R_nm( int n, int m, double x, double y, int *r_data );

// �^�� V_nm(x,y) ����
void	V_nm( int n, int m, double x, double y, int *r_data, complex<double> &v );



#endif