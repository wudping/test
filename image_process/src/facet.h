#ifndef _FACET_H
#define _FACET_H

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/vector_proxy.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <cmath>
#include <limits.h>

#include <QImage>


class Facet
{
	private:
		int		N;		// �M�w�O 3*3   5*5

		//boost::numeric::ublas::vector<double>	p;			// �D�X��  �h�������Y��.   p0 + p1*i + p2*j + p3*i^2 + p4*i*j + p5*j^2   �q 0�}�l  ��paper���Ӥ@��.

		boost::numeric::ublas::matrix<double>	MPA;		// Moore-Penrose inverse  A^+   �γo�Өӱ��X p0,p1,....pn
		boost::numeric::ublas::matrix<double>	W;			// weight matrix


	public:

		Facet( int n );

		double	get_subpixel( double x, double y, QImage image );													// �o�� sub pixel ��������

		int		power( int i, int n, int j, int m );																// �D i^n j^m
		double	power( double i, int n, double j, int m );															// �D i^n j^m

		boost::numeric::ublas::matrix<double>	inverse( boost::numeric::ublas::matrix<double> A );					// �D�x�}inverse  ��lu����
		boost::numeric::ublas::vector<double>	find_p( double x, double y, QImage image );							// �D�X�Y�� p

		bool	edge( double x, double y, QImage image, double &rtx, double &rty );									// �D��
	
};






#endif