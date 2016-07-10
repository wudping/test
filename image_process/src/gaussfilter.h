#ifndef _AMATH_H
#define _AMATH_H

#include <vector>
#include <cmath>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/math/distributions/normal.hpp>

#include "../tools.h"

#include <QImage>
#include <QColor>
#include <QProgressBar>

#include "../src/Progress.h"
#include <fstream>
#include <iostream>


using namespace std;





/*======================================================================================
	typedef��
========================================================================================*/


/*======================================================================================
	struct��
========================================================================================*/




/*======================================================================================
	class��
========================================================================================*/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    �ΨӰ�Gauss Filter
	���ӥi��|�X�R�����U��filter
	�o�ӬO 1D ��filter,���ӧƱ�Ψ��1D�����@��2D��filter
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
class	GaussFilter_1D
{

	private:
		int					scale;					// ���q�ث� �|�v�Tmean deviation

		double				mean;					// ����
		double				deviation;				// �зǮt

		vector<double>		filter_map;				// filter���x�}
		vector<double>		DoG;					// Laplacian of Gaussian���x�}

		void				generate_map();			// ����filter map


	public:
		// �غc�禡
		GaussFilter_1D( int _scale );

		// �� map �ȥ�, �|�۰ʱNindex �ন �H 0 ������
		double&		operator [] ( int index );

		// �� map �ȥ�, �|�۰ʱNindex �ন �H 0 ������
		double&		operator () ( int index );

		// ����filter map  �T�w k = 2
		void	generate_DoG_map();		

		// ���XDog��Map
		double	get_DoG( int index );
	

};


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	�Q�Ψ�� Gauss filter 1D �Ӱ� 2D ��filter
	���ӷ|�Ҽ{�i�H�B�z w != l �����p
	�ثe���� scale = max(w,l) �����p.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
class	GaussFilter_2D
{
	private:
		int				w,	l;										// ��Ӥ�V���ث�

		double			mean_w,	mean_l;								// ������  �P�˰Ϥ�����Ӥ�V  �򧡤@������
		double			mean;

		double			deviation_w;								// �зǮt
		double			deviation_l;
		double			deviation;

		double			normalized;									// normalized�Y��

		bVectorDouble	filter_map_w;								// ��V��filter
		bVectorDouble	filter_map_l;								// �a�V��filter
		bMatrixDouble	filter_map;									// filter���x�}
		bMatrixDouble	DOG;										// DoG���x�}  �Τj�g�O���F�קK��1D�d�V

	public:
		// �غc�禡
		GaussFilter_2D();
		GaussFilter_2D( int _w, int _l );

		// ����filter map
		void	generate_map();

		// �� map �ȥ�, �|�۰ʱNindex �ন �H 0 ������
		// ���ӬݬO�_��令 [] ���覡. (�]��boost��matrix�S���ѳo�˪��@�k...�o�ۤv�g)
		//double&		get_map ( int i, int j );
		double&		operator	()	( int i, int j );

#ifdef _OPENPIV_
		// �N����Ū�i��, ��filter���ʧ@
		QImage	do_filter( QImage passImage, Progress *progress );
#else
		QImage	do_filter( QImage passImage );
#endif


		// ����filter, �u���T�w�϶�   lu = left up    rd = right down
		QImage	do_filter( QImage passImage, PointInt lu, PointInt rd );

		
		// ���XDog��Map
		double	get_DoG( int i, int j );


		// ���Xfilter_map
		double	get( int i, int j );

		// �]�w w
		void	set_w( int _w );

		// �]�w l
		void	set_l( int _l );

		// ����DoG map
		void	generate_DoG_map();

		// �^��filter�x�}
		boost::numeric::ublas::matrix<double>	get_map();

};












/*======================================================================================
	function��
========================================================================================*/



#endif