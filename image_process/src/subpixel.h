#ifndef _SUBPIXEL_H
#define _SUBPIXEL_H

#include "facet.h"

#include <QImage>
#include <QVector>

#include "./njet.h"
#include "./CV.h"
#include "../tools.h"

#include <qwt_painter.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_map.h>
#include <QGraphicsSvgItem>
#include <QRect>
#include <qframe.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_map.h>
#include <QGraphicsSvgItem>
#include <QPainter>
#include <qapplication.h>
#include <qpainter.h>
#include <qwt_math.h>
#include <qwt_symbol.h>
#include <qwt_curve_fitter.h>
#include <qwt_plot_grid.h>
#include <qwt_plot.h>



#include <iostream>
#include <fstream>

#ifdef _OPENPIV_
#include "../src/andreaengine.h"
#include "../src/settings.h"
#include <QSvgGenerator>
#include <QPainter>
#endif


/*=================================================================================================================
	Edge 
	�i�H����subpixel��edge 
/*=================================================================================================================*/
#ifdef _OPENPIV_
class	Edge : public AndreaEngine
#else
class	Edge
#endif
{
	private:
		int						X,	Y;			// �w�惡�y�ШӰ� subpixel��edge�p��
		int						N;				// window size
		QImage					image;
		//QImage					curvature;		// �s��curvature�����
		bMatrixDouble			curvature;

#ifdef _OPENPIV_
		DataContainer			*filedata;
		Settings				*settings;
		QMutex					mutex;
		Progress				*progress;		// �t�d�^���i�ץΪ�

	signals:
		void					debugSignal( QString );				// �����T��
#endif

	public:

#ifdef _OPENPIV_
		Edge(  Settings *settingsPass, DataContainer *filedataPass  );									// �غc���
#else
		Edge();
#endif

		virtual					~Edge();																// �Ѻc
	
		void					dosomething();															// OPENPIV���{���i�J�I

		void					draw_subpixel_point( double x, double y, double xx, double yy );		// �esubpixel���I�W�h
		void					draw_subpixel_point( QVector<QPointF> qf, QVector<QPointF> bf );		// �esubpixel���I�W�h

		PointDouble				itepolation( int _x, int _y, double dir_x, double dir_y );				// ���ե� �ϥιL�I�������k �h����curvature �A�h�j�Mmax��edge subpixel
		double					ite_poly( int dx, int dy, double x, double y, double xx, double yy );	// �����Ϊ��h����

		QVector<QPointF>		extract_edge( PointInt lu, PointInt rd );								// ����edge �o��ĥΪ��O�u��edge��V�h�j�M������

		void					testSVG();																// ���ղ���SVG  (�ϥ�QWT)

};



/*=================================================================================================================
	corner
	�i�H����subpixel��corner
/*=================================================================================================================*/
#ifdef _OPENPIV_
class	Corner : public AndreaEngine
#else
class	Corner 
#endif
{
	private:
		int						X,	Y;			// �w�惡�y�ШӰ� subpixel�� corner �p��
		QImage					image;

#ifdef _OPENPIV_
		DataContainer			*filedata;
		Settings				*settings;
		QMutex					mutex;
		Progress				*progress;		// �t�d�^���i�ץΪ�

	signals:
		void					debugSignal( QString );				// �����T��
#endif

	public:

#ifdef _OPENPIV_
		Corner(  Settings *settingsPass, DataContainer *filedataPass  );						// �غc���
#else
		Corner();
#endif
		virtual			~Corner();																// �Ѻc
	
		void			dosomething();															// OPENPIV���{���i�J�I

		void			draw_subpixel_point( double x, double y, double xx, double yy );		// �esubpixel���I�W�h
		void			draw_subpixel_point( QVector<QPointF> qf );								// �esubpixel���I�W�h

};




#endif