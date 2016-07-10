#ifndef _ZERNIKE
#define _ZERNIKE

#include <complex>
#include <fstream>
#include <boost/numeric/ublas/matrix.hpp>

#include <QImage>

using namespace std;



//----------------------------------------------typedef------------------------------------------------------
typedef		boost::numeric::ublas::matrix<double>	bMatrixDouble;
typedef		boost::numeric::ublas::matrix<int>		bMatrixInt;
typedef		boost::numeric::ublas::vector<int>		bVectorInt;

//----------------------------------------------struct------------------------------------------------------
struct	POINT
{
	int		x;
	int		y;
	POINT( int _x, int _y ) : x(_x), y(_y) {}
};


//----------------------------------------------class------------------------------------------------------
class	Zernike
{
	private:
		int				N;											// size
		double			PI;			
		
		QImage			image;

		bVectorInt		fac_list;									// ���h�����

		boost::numeric::ublas::matrix<bVectorInt>		r_data;		// r lsit �����
		boost::numeric::ublas::matrix<complex<double>>	Z;			// Zernike moment


	public:
		// �غc���
		Zernike();
		
		// ���Ͷ��h����
		void				factorial_list();

		// Ū�����h����
		void				factorial_load();

		// ���� r ����
		void				r_list();

		// Ū�� r �����
		void				r_load();

		// �^�� R_nm(x,y) ����
		double				R_nm( int n, int m, double x, double y );

		// �^�� V_nm(x,y) ����
		complex<double>		V_nm( int n, int m, double x, double y );

		// ���͸��  ���h  r table �o���
		void				generate_pdata();

		// Ū�����  ���h r table �o���
		void				load_pdata();

		// Ū�����ɸ��
		void				loadBMP( QImage passImage );

		// ����zernike moment �ɮ�
		void				calculate_zernike( POINT cenater, double radius );

		// �p�� Anm   ( zernike moment)    ���w���� �b�| �h���n�� (�b�|�|����normalized)
		complex<double>		Z_nm( int n, int m, POINT center, double radius );

		// ���عϹ�
		void				reconstruct( POINT center, double radius );

		// ���جY����moment�Ϲ�
		void				recon_nm( int n, int m, POINT center, double radius );

		// ���եΨ��
		void				test();

};






#endif