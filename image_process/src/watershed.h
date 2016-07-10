#ifndef _WATERSHED_H
#define _WATERSHED_H

// INT_MAX�]�t�b�o��
#include <limits.h>

#include <boost/pending/queue.hpp>
#include <boost/heap/priority_queue.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include "mser_process.h"
#include "../MEASURE/FFT.h"
#include "gaussfilter.h"

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




struct	POINT_W
{
	int		x;	
	int		y;
	POINT_W( int _x = 0, int _y = 0 ) : x(_x), y(_y) {};

	//   = operator
	POINT_W	operator	=	( POINT_W b );
};

//   != operator
bool	operator	!=	( POINT_W a, POINT_W b );

//   == operator
bool	operator	==	( POINT_W a, POINT_W b );



/*===================================================================================================
	Watershed��class
/*===================================================================================================*/
#ifdef _OPENPIV_
class	Watershed : public AndreaEngine
#else
class	Watershed
#endif
{
	private:

		int		_intLengthX, _intLengthY;

#ifdef _OPENPIV_
		DataContainer	*filedata;
		Settings		*settings;

		QMutex		mutex;

		// �t�d�^���i�ץΪ�
		Progress	*progress;
#endif

		QImage		image;
		QImage		final;


		// �s�� lower complete����� (�]���|�W�L255)
		bMatrixInt		lc;

		// �s��label���
		bMatrixInt		lab;

		// �s��dist���
		bMatrixInt		dist;

		// �s��sort�᪺�I���
		bVectorInt		sort_data;

		// �s��sln�����
		boost::numeric::ublas::matrix<POINT_W>	sln;

		// �O�_����ۤv�p���F�~
		bool	has_low_neib( int i, int j );
		
		// �Φb4.1
		bool	has_low_neib_2( QImage *image, int i, int j );

		// NG �D�����ǾF�~
		void	NG( POINT_W p, POINT_W *pointlist, int &size, int width, int height );

		// ���j�ϥΪ� Resolve
		POINT_W	Resolve( POINT_W p );

		// �إ� sln �� function
		void	sln_maker();

#ifdef _OPENPIV_
		
	signals:
		void	debugSignal( QString );				// �����T��

	protected:

#endif



	public:

#ifdef _OPENPIV_
		// �غc
		Watershed( Settings *settingsPass, DataContainer *filedataPass );
#else
		// �غc �L�ǤJ��
		Watershed();
#endif

		// �Ѻc
		virtual		~Watershed();   

		// ���Q�γo��funtion�Ӱ��Ʊ�. ���N�쥻��main
		void		dosomething();

		// lower completion
		void	LowerCompletion();

		// minima detetor
		void	LevelComponents();

		// ����watershed
		void	doWatershed();

		// �̭�l��watershed   alg 4.1
		void	doWatershed_4_1();

		// �ϥ�
		void	highlight();

		// ��X
		void	output();

		// ����gradient image    sqrt( fx^2 + fy^2 )
		QImage	gradient();

		// �N�ϧΰ�truncate   i+-5 ���ܦ� i ������
		QImage	truncate( int n );

		// �N�ϧΤ��}��  ��Х�
		QImage	divide( QImage	imagePass );
};



#endif