#ifndef _FFT_H
#define _FFT_H



#include <fftw3.h>
#include <boost/numeric/ublas/matrix.hpp>

#include "../tools.h"

#include <QImage>
#include <QPoint>
#include <QList>
#include <QMutex>


#ifdef _OPENPIV_

#include "../src/andreaengine.h"
#include "../src/settings.h"
#include "../src/progress.h"
#include "../src/datacontainer.h"

#endif


typedef boost::numeric::ublas::matrix<double>	DoubleMatrix;
typedef boost::numeric::ublas::matrix<int>	IntMatrix;



// FFT����
#ifdef _OPENPIV_
class	FFT : public AndreaEngine
#else
class	FFT
#endif
{
	private:
		DoubleMatrix		R,	I;			// FFT�ഫ��|���ͪ� Real part,  Image part

		QImage				image;			// ��l�ϧ�
		QImage				junction;		// �o�쪺junction  edge  boundary
		QImage				edge;
		QImage				boundray;

		DoubleMatrix		filter_cos;		// ���filter
		DoubleMatrix		filter_sin;

		int					w,	h,	N;		// �Ϥ������e �٦��j�p
		double				sigma;			// scale
		double				pi;
		
#ifdef _OPENPIV_
		DataContainer	*filedata;
		Settings		*settings;

		QMutex		mutex;

		// �t�d�^���i�ץΪ�
		Progress	*progress;

	signals:
		void	debugSignal( QString );				// �����T��

	protected:
		/*bool	crossCorrelate(int topLeftRow, int topLeftColumn);
		void	prepareFFT();
		void	fftCore(ImageData *_imageA, ImageData *_imageB, int topLeftRow, int topLeftColumn);*/
#endif


	public:
#ifdef _OPENPIV_
		// �غc
		FFT( Settings *settingsPass, DataContainer *filedataPass );

		// ���Q�γo��funtion�Ӱ��Ʊ�. ���N�쥻��main
		void		dosomething();
#else
		// �غc �L�ǤJ��
		FFT();
#endif

		// �Ѻc
		virtual		~FFT();   

		// �N�Ϥ���FFT�ഫ
		void	c2_FFT();

		// ��J�}�C ��X
		void	output( DoubleMatrix out );
		
		// ��J�}�C ��X  ���w�ɦW
		void	output_map( DoubleMatrix out, char *str );

		// ��X Real part
		void	output_Real();

		// ��X Image part
		void	output_Image();

		// FFT�f�ഫ
		void	c2_IFFT();

		// ���ե� ���Y�ǰʧ@
		void	doing();

		// ���ե� ���Y�ǰʧ@
		void	doing2();

		// ����filter  (�i�H���ܦh���P���o�i��,��ɭԫ��ӽs���ӵ�)   ���� real part (cos)   image part (sin)
		void	filter( int n, boost::numeric::ublas::matrix<double> &FR,  boost::numeric::ublas::matrix<double> &FI );

		// ����filter���ʧ@
		void	do_filter( int n,  boost::numeric::ublas::matrix<double> FR, boost::numeric::ublas::matrix<double> FI, boost::numeric::ublas::matrix<double> &C1, boost::numeric::ublas::matrix<double> &C2 );

		// �f�ഫ�^��
		boost::numeric::ublas::matrix<double>	inverse( boost::numeric::ublas::matrix<double> tR, boost::numeric::ublas::matrix<double> tI );

		// �η��y�Ъ��覡�Ӳ���filter
		void	draw_filter( int n );

		// radial function
		double	Kp( double r );

		// ����polynomial  (�|�q�L�I��)   �Ψӧ�junction  �|�۰ʷj�M�X�̤j��
		void	itepolation( int _x, int _y );

		// ����polynomail���h��������
		double	ite_poly( int dx, int dy, double x, double y, double xx, double yy );

		// subpixel junction  �|�b�o��j�M junction��max  �M��A�h���p��
		void	subpixel_junction( PointInt lu, PointInt rd );


};




#endif