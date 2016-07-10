#ifndef _NJET_H
#define _NJET_H

#include <QImage>

#include "../tools.h"

#include <boost/numeric/ublas/matrix.hpp>

//test
typedef boost::numeric::ublas::matrix<double> bMatrixDouble;


struct pointF
{
	double	x;
	double	y;
};

struct point
{
	int		x;
	int		y;
};

struct window
{
	int		width;
	int		height;
};

class	Njet
{
	private:
		QImage		image;															// Ū�J������

		int			win_width,	win_height;											// windows �� size   �b�o��windows�̭����������p  �� 1  ���  2*1 = 3   3*3��window�h��

		bMatrixDouble		mask;								// ���Ͱ���function
		bMatrixDouble		r_x,	r_y;						// �@���L����
		bMatrixDouble		r_xx,	r_xy,	r_yy;				// �G���L����
		bMatrixDouble		r_xxx,	r_xxy,	r_xyx,				// �T���L����
							r_xyy,	r_yyx,	r_yyy;				// �n�p�� r_xyx = r_yxx   �|�h�X�ⶵ
		bMatrixDouble		det_H;								// �s��C���I�� det(H)  H = Hessian

	public:
		Njet( QImage imagePass );													// �غc���
		void			diff();														// �D�L����
		double			jet1( int x, int y, double dx, double dy );					// ��ڤW�O 1-jet   �N x, y � dx dy�O���F�קK�~�t   4.663 = 4 + 0.663    x = 4   y = 0.663
		double			jet2( int x, int y, double dx, double dy );					// 2-jet
		double			jet3( int x, int y, double dx, double dy );					// 3-jet
		double			get_Hessian( int x, int y );								// �p��Hessain����   ( det  tr ����  ���ե�)
		void			get_ew( int x, int y, double &nx, double &ny );				// �o�� ew     gradient direction 
		double			get_cur( int x, int y );									// �o�� norm of gradient  (����curvature)
		void			get_ev( int x, int y, double &tx, double &ty );				// �o�� ev   �� ew ����

		double			get_fx( int x, int y );										// �^�� fx
		double			get_fy( int x, int y );										// �^�� fy

		double			get_fw( int x, int y );										// �o�� fw  (v,w)�����W  ����L  �@�Ӥ��q = 0   �۷��^��  sqrt(  r_x^2 + r_y^2 )
		double			get_fww( int x, int y );									// �o�� fww (�Q���O����᪺Hessian)
		double			get_fvv( int x, int y );									// �o�� fvv (�Q���O����᪺Hessian)
		void			get_edge( int x, int y, double &px, double &py );			// �w��X�ӨD�� (subpixel����) �����k
		void			get_RH( int x, int y );										// �o�����᪺ Hessian
		void			corner();													// �o��corner
		void			corner2();													// �o��corner  ��k�G �ĥ�Hessian���@�k
		void			eigenvector( double A, double B, double C, 
											double &vx, double &vy );				// ��ٯx�}�Deigenvector  (��eigenvalue����ȸ��j����V)
		double			min( double a, double b, double c, double d );				// �Ǧ^ a, b, c, d ���̤p��

		void			output( boost::numeric::ublas::matrix<double>	map, char *str );		// �N���normalized���X�����

		double			MAX( double a, double b );
		double			MIN( double a, double b );

		bMatrixDouble	pix_corner( PointInt lu, PointInt rd );						// pixel ���Ū� corner detector.  

		// ��openCV�ӧ諸subpixel  ���Φb  get_corner�̭�
		// get_corner�O�ۤv�ק諸 �٦����D
		int				get_corner( int x, int y, double &dx, double &dy );																									// �o��corner��subpixel��m   �Q�� (dx,dy)    �^�ǭȬO��ܦ��S�����subpixel����m
		// corner_subpixel�OopenCV��L�Ӫ�  �]�٬O�����D
		void			corner_subpixel( QImage src,  boost::numeric::ublas::matrix<double> &dst, double cx, double cy );																				// �p��windows�̭� �����᪺pixel���
		void			cornet_gradient( boost::numeric::ublas::matrix<double> &gx, boost::numeric::ublas::matrix<double> &gy, double cx, double cy );													// �p��window�̭� �����᪺�@���L��
		void			cornet_gradient( boost::numeric::ublas::matrix<double> src, boost::numeric::ublas::matrix<double> &gx, boost::numeric::ublas::matrix<double> &gy, double cx, double cy );		// �ϥέ�l��ư�����Ӥ���
		void			cornet_gradient2( bMatrixDouble &dst, bMatrixDouble buffer );													// openCV���@�k

};



#endif