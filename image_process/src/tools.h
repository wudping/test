/*=============================================================================================================================

	�U�ؤu��
	�Z�O���Q����class
	�Ϊ̦@�q��define typedef 
	����b�o��.

/*=============================================================================================================================*/

#ifndef _TOOLS_H
#define _TOOLS_H

#include <QImage>
#include <QVector>

#include <boost/numeric/ublas/matrix.hpp>



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// point���c  �i�H�M�w�O����I�٬O�B�I���I
template<class T>
struct	CCPOINT
{
	T	x;
	T	y;
	CCPOINT<T>( T _x = 0, T _y = 0 ) : x(_x), y(_y) {}
	//POINT( int _x = 0, int _y = 0 ) : x(_x), y(_y) {}
};

// window���c  �M�w�d���
// ��ڤW�O�� width*2+1   height*2+1  ���j�M�d��   �����I���k�Z��
struct	WINDOW
{
	int		width;
	int		height;
	WINDOW( int _w, int _h ) : width(_w), height(_h) {}
};


// pixel  ���I�y��(��� �B�I��)  RGB
struct	PIXEL
{
	int		x,	y;
	int		square_size;			// ���v�Ϊ� ����Ϊ��j�p
	int		direction;				// �M�w��V
	double	fx,	fy;
	QRgb	rgb;

	PIXEL( int _x, int _y, double _fx, double _fy, QRgb _rgb )
		:	x(_x), y(_y), fx(_fx), fy(_fy), rgb(_rgb)  {}

	PIXEL()	:	x(0), y(0), fx(0.), fy(0.), rgb(0)  {}
};



namespace M2I
{
	const int	default_value	=	0;			// �w�]��
	const int	normalized		=	1;			// �зǤ�
};



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef		CCPOINT<int>							PointInt;
typedef		CCPOINT<double>							PointDouble;
typedef		boost::numeric::ublas::matrix<int>		bMatrixInt;
typedef		boost::numeric::ublas::matrix<double>	bMatrixDouble;
typedef		boost::numeric::ublas::vector<int>		bVectorInt;
typedef		boost::numeric::ublas::vector<double>	bVectorDouble;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QImage				Matrix2Image( bMatrixDouble map, int m2i = M2I::default_value );								// �Nmatrix�নimage �۰�normalized
bMatrixDouble		matrix_normalized( bMatrixDouble map );															// ��matrix��normalized  (��0~255)
void				m2i_copy( bMatrixDouble &map, QImage &image );													// �Nmatrix����ƽƻs��image
bMatrixInt			local_max( bMatrixDouble map , WINDOW win, double threshold );									// ��Xlocal max
QVector<QRgb>		Pixel2Rgb( QVector<PIXEL> pixel );																// �Npixel vector�ন qrgb
QVector<QPoint>		Pixel2Point( QVector<PIXEL> pixel );
QVector<QPointF>	Pixel2PointF( QVector<PIXEL> pixel );

#endif