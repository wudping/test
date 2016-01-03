#ifndef _BLOB_H
#define _BLOB_H

#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include "stdafx.h"
#include "fitting.h"
#include "gaussfilter.h"
#include "NChernov.h"

#include "../MEASURE/facet.h"
#include "../MEASURE/njet.h"
#include "../MEASURE/CV.h"

#include "../timer.h"

#include <QPoint>
#include <QList>
#include <QMutex>
#include <QProcess>

#ifdef _OPENPIV_
#include "../src/andreaengine.h"
#include "../src/settings.h"
#include "../src/progress.h"
#include "../src/datacontainer.h"
#endif


/*--------------           NOTE          -------------------------

	2012/10/15 �}�l�ק令�i�ܪ���.

------------------------------------------------------------------*/

// �~�t�e�\�d��
//#define ERROR 0.00001

// �j�M�䪺���e��
#define EDGE_THRESHOLD 20

//-----------------------struct��-----------------------------------------------------------------




//-----------------------class��-----------------------------------------------------------------
/*=============================================================
	blob����
	�ϥ�sub pixel precise blob detection
/*=============================================================*/
#ifdef _OPENPIV_
class	SUBPIX_BLOB : public AndreaEngine
#else
class	SUBPIX_BLOB
#endif
{
	private:

		int		scale_w,	scale_l;								// scale �ث�
		
		boost::numeric::ublas::matrix<double>		curva_map;		// �scurvature
		boost::numeric::ublas::matrix<double>		r_x,	r_y;	// �@���L����
		boost::numeric::ublas::matrix<double>		r_xx,	r_yy;	// �G���L����
		boost::numeric::ublas::matrix<double>		r_xy;		

		int		_intLengthX, _intLengthY;
		
		QImage		image;						// �B�z���.  ��l
		QImage		filter;						// filter�᪺���
		//QImage		cur;						// curvature�����
		QImage		final;						// ���u�����


#ifdef _OPENPIV_
		QMutex			mutex;
		DataContainer	*filedata;
		Settings		*settings;
		Progress		*progress;					// �t�d�^���i�ץΪ�

	signals:
		void	debugSignal( QString );				// �����T��

	protected:
#endif

	public:

#ifdef _OPENPIV_
		SUBPIX_BLOB( Settings *settingsPass, DataContainer *filedataPass );					// �غc�禡

		void			dosomething();														// ���Q�γo��funtion�Ӱ��Ʊ�. ���N�쥻��main
		void			dosomething3( QImage graytone );														// ���ե�
#else
		SUBPIX_BLOB();
#endif
		
		// �Ѻc
		virtual			~SUBPIX_BLOB();   

		// dosomething 2�� ���ե�
		void			dosomething2( char *str, int index );

		// �ˬd�O�_�O�����I
		bool			is_center( int x, int y, double *subpx, double *subpy );

		// �M��Xblob
		void			find_blob();

		// �M��Xblob  ���w�d��
		void			find_blob( PointInt lu, PointInt rd );

		// ����curvature
		void			curvature();

		// ����curvature  ���w�d��  ���W �k�U
		void			curvature( PointInt lu, PointInt rd );

		// �p��eigenvector   
		void			eigenvector( int x, int y, double *evx, double *evy, double rx, double ry, double rxx, double ryy, double rxy );

		// �D�Xpolygen
		void			polygen( double subpx, double subpy );

		// �D rx, ry, rxx, ryy, rxy
		void			diff();

		// �D rx, ry, rxx, ryy, rxy  ���w�d��
		void			diff( PointInt lu, PointInt rd );

		// �D�X�b�Ӥ�V, �h�ֶZ�����X�{ edge. �䤣�쪺�� tmax = -1
		double			find_edge( double nx, double ny, double px, double py, double scale );

		// �e�X���
		void			ellipse_draw();

		// �p��eigenvector   
		void			eigenvector( double A, double B, double C, double &vx, double &vy, double &wx, double &wy );

		// �p��eivenvector�Ϊ��lfunction ���@�ǳB�z
		void			eigenvector_1( double A, double B, double C, double &nx, double &ny , double eigen );

		void			eigenvector2( int x, int y, double *evx, double *evy, double rx, double ry, double rxx, double ryy, double rxy );

		// ��Jmatrix �ন���� 
		void			output( boost::numeric::ublas::matrix<double> map, char *str );
		void			output( boost::numeric::ublas::matrix<int> map );

		// �j�Mlocal max min  �ϥ� non maximal suppression    pmap = �W�@��map   nmap = �U�@��map   26��neighbor����
		int				local_extreme( boost::numeric::ublas::matrix<double> map, boost::numeric::ublas::matrix<double> pmap, boost::numeric::ublas::matrix<double> nmap );

		// �j�Mlocal max min  �ϥ� non maximal suppression    pmap = �W�@��map   nmap = �U�@��map	8��neighbor����
		void			local_extreme( boost::numeric::ublas::matrix<double> map );

		// �p�⤤�ߪ��j��
		double			center_value( int x, int y );

		// �Q��non maximal suppression �h�o�줤���I
		boost::numeric::ublas::matrix<int>	center_detector();

		// ���ծĪG��corner
		void			corner( int index );


};


#endif