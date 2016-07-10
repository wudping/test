#ifndef _CHANNEL_H
#define _CHANNEL_H


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	�����m�q�D
	�B�z��m�������\��
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <QImage>
#include <QList>
#include <QFile>
#include <QProcess>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfoListIterator>

#include "../tools.h"
//#include "../timer.h"

#include <fstream>
#include <iostream>
#include <iomanip>

#ifdef _OPENPIV_
//#if defined(_OPENPIV_) || defined(_GTEST_)
#include "../src/andreaengine.h"
#include "../src/settings.h"
#include "../src/progress.h"
#include "../src/datacontainer.h"
#endif

#include "shadow.h"

#include <boost/math/distributions/normal.hpp>


#if 0



// square���� 
struct	SquareData
{
	int					direction;						// ��V
	int					x,	y;							// ���W�����y��
	int					size;							// �]���O����� �u�ݭn�O���@��size���
	ShadowReturnData	*line_data;						// �N����Τ����ܦh�� 1D ��� 
	SquareData()
	{
		line_data	=	NULL;
		size		=	0;
		x			=	0;
		y			=	0;
		direction	=	0;
	}
	SquareData( const SquareData &sd )
	{
		direction	=	sd.direction;
		x			=	sd.x;
		y			=	sd.y;
		size		=	sd.size >= 0 ? sd.size : 0;

		if( size > 0 )
		{
			line_data	=	new	ShadowReturnData[size];
			for( int i = 0; i < size; i++ )
				line_data[i]	=	sd.line_data[i];
		}
		else
			line_data	=	NULL;
	}
	~SquareData()
	{
		if( line_data )
			delete	[]	line_data;
		size	=	0;
		x		=	0;
		y		=	0;
	}
	void	operator = ( SquareData right )
	{
		direction	=	right.direction;
		x			=	right.x;
		y			=	right.y;
		size		=	right.size >= 0 ? right.size : 0;
		if( size > 0 )
		{
			line_data	=	new ShadowReturnData[size];
			for( int i = 0; i < size; i++ )
				line_data[i]	=	right.line_data[i];
		}
		else
			line_data	=	NULL;
	}
};





// KernelDensity ����  �o��ΨӹL�o���T
class	KernelDensity
{
	private:
		bMatrixInt		data;										// �ΨӤ��R�����
		bMatrixDouble	p_data;										// �p�⧹�����G
		bMatrixDouble	kernel;										// smooth �� kernel

	public:
		KernelDensity();											// �غc���

		bMatrixInt		set_data( SquareData _data );				// �]�w��� �|�N�榡���ഫ
		bMatrixDouble	dosomething();								// ����
		bMatrixDouble	kernel_gauss( int n );						// ���� Gauss Kernel
		bMatrixDouble	kernel_square( int n );						// ���Ͱ򥻪������ kernel
		int				element_count();							// �����Ӽƭp��

};






// channel����
#ifdef _OPENPIV_
class	Channel	:	public	AndreaEngine
#else
class	Channel
#endif
{
	private:

		int			det( int a, int b, int c, int d );								// cross�ϥ� �~�n
		double		norm_max( double a, double b, double c );						// maximanorm
		QImage		image;
		QImage		final;

		// ���U�O���v�˴���
		QVector<PIXEL>	square;														// �˴��϶�		
		int				direction;													// ��V
		int				threshold;													
		SquareData		*square_data;												// �˴��ϸ��.
		SquareData		*square_data2;												// �]���צV���������⦸ (�]���_�I���ӬO������)


#ifdef _OPENPIV_
		DataContainer	*filedata;
		Settings		*settings;
		QMutex			mutex;

		// �t�d�^���i�ץΪ�
		Progress		*progress;

	signals:
		void	debugSignal( QString );												// �����T��

	protected:
#endif


	public:
#ifdef _OPENPIV_
		Channel( Settings *settingsPass, DataContainer *filedataPass );				// �غc
		void				dosomething();											// ���Q�γo��funtion�Ӱ��Ʊ�. ���N�쥻��main
#else
		Channel();																	// �غc �L�ǤJ��
#endif

		virtual				~Channel();												// �Ѻc


		// ���v�˴��Ϊ�
		bVectorInt			loadimage( PIXEL p1, PIXEL p2 );						// Ū���Ϥ����
		SquareData			shadow_square( PIXEL square_roi );						// �@���ˬd�@�Ӱ϶�
		ShadowReturnData	shadow_line( PIXEL start, PIXEL end );					// �@���ˬd�@���u
		void				mark_on_image( SquareData square );						// �N���G�аO�b�Ϥ��W ��� 
		void				mark_on_image( SquareData square, QImage &img );		// �N���G�аO�b�Ϥ��W ��� 
		double				centroid( bVectorInt deep, int start, int end );		// �p���ߦ�m


};
















#endif







#endif