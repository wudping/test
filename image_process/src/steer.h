#ifndef _STEER_H
#define _STEER_H

#include <boost/numeric/ublas/matrix.hpp>
#include <fstream>

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


using namespace std;

// �w�qboost�� double matrix
typedef boost::numeric::ublas::matrix<double> boost_matrix;

// �w�qboost�� double vector
typedef boost::numeric::ublas::vector<double> boost_vector;





#ifdef _OPENPIV_
class Steer : public AndreaEngine
#else
class Steer
#endif
{
	private:
		int						X,	Y;									// �惡�y�Хh���B��
		int						N;										// ��ܥثe�O�� N ��
		int						g_size;									// g_map �� size.    �Q�� 2*g_size +1  = ���size  ���@�k
		int						o_size;									// g_map��l��size ����Adown sample�� g_size
		static const int		MAX_THETA	=	100;
		double					pi;
		double					*w;										// weight

		QImage					image;
		QImage					polar;

		boost_matrix			g_map;									// g(r)   radial function
		boost_matrix			he[MAX_THETA];							// even part
		boost_matrix			ho[MAX_THETA];							// odd part
		boost_matrix			E[MAX_THETA];							// energy

		boost_vector			theta_data;								// ���Ӱ���ܥΪ���� polar �� theta
		boost_vector			value_data;								// ���Ӱ���ܥΪ���� polar �� value

		double					exp(int);								// �����ϥΪ�exp  �i�H�ǤJ��ƭ�
		double					SMAX( double a, double b );				// �^�ǳ̤j��


#ifdef _OPENPIV_
		DataContainer			*filedata;
		Settings				*settings;

		QMutex					mutex;

		// �t�d�^���i�ץΪ�
		Progress				*progress;

	signals:
		void					debugSignal( QString );				// �����T��
#endif

	public:
#ifdef _OPENPIV_
		Steer(  Settings *settingsPass, DataContainer *filedataPass  );							// �غc���
#else
		Steer();																				// �غc���
#endif

		// �Ѻc
		virtual			~Steer(); 
	
		boost_matrix	downsample( boost_matrix map );											// down sample �ʧ@�b�o��

		void			output_map( boost::numeric::ublas::matrix<double> map );				// ��Jmap��� �L�X�ϧ� (�۰�normalized)
		void			output_map( boost::numeric::ublas::matrix<double> map, char *name );	// ��Jmap��� �L�X�ϧ� (�۰�normalized)  ���w�ɦW

		double			g_radial( double r );													// radial ���
		void			generate_map();															// ���� g_map

		double			get_Energy( int ii, int x, int y );										// ���o���I�� energy

		void			draw_filter( int ii );													// ���ե� �efilter
		void			polar_draw();															// �β��ͦn����Ƶe�Xpolar�ϧ�

		void			local_max( double &L, double &T, double &X );							// �M��local max
	
#ifdef _OPENPIV_
		void			dosomething();															// OPENPIV���{���i�J�I
#endif
	
		//double&		get_g_map( int i, int j );												// ���o g_map   (���������Ū����g�J)
};


























#endif