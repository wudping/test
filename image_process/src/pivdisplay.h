/************************************************************************************************************************************

	pivdisplay
	mainwindow���U����ܵ���
	�t�d�U�عϤ������

	QGraphicsView�����T�ӳ���
	View   
	Scene  (model)
	item   

*************************************************************************************************************************************/

#ifndef PIVDISPLAY_H
#define PIVDISPLAY_H



#include <QMouseEvent>
#include <QCoreApplication>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QGraphicsEffect>
#include <QStyleOptionGraphicsItem>
#include <QBrush>
//#include <QSvgGenerator>
//#include <QSvgRenderer>
#include <QLabel>

#include "datacontainer.h"
#include "displaywidget.h"
#include "imagepaint.h"
#include "settings.h"



class PivDisplay : public QGraphicsView
{
    Q_OBJECT

	private:

		QLabel				*positionLabel;											// ��ܵ������U ��ܮy�Ъ�label
		QLabel				*RGBLabel;												// ��ܵ������U ���RGB��label

		PIXEL				pix_1;													// �Ψ��point�������U�ƹ� ��}�ƹ����y��
		PIXEL				pix_2;

        bool				mouseIsPressed;
		bool				isSVG;

        QImage				image;													// �Ϥ����
		QRect				rect;													// ��l�Ϥ��j�p
		QRectF				rectF;													// ��l�Ϥ��j�p
        QRectF				viewRectF;												// ��ܵ����j�p  �i�H�Φb�Y��
        bool				viewRectFSet;		
		
        QGraphicsScene		*scene;													// ��ܥ� ����R   scene = model   
        ImagePaint			*imPaint;												// item
		QPointF				centerPoint;

		QPointF				preMarkPoint;											// �s�W�@���аO�I����m
		QRgb				preMarkRgb;												// �s�W�@���аO�I����

        Settings			*settings;
        DataContainer		*filedata;

		PIXEL				MouseEvent2Pixel( QMouseEvent *event );					// �N�ƹ��T���ন PIXEL ��T

    public:
        PivDisplay(QWidget* parent = 0);
        ~PivDisplay();
		
        QPointF				getPoint1();											// �ثe�L�@��
        QPointF				getPoint2();											// �ثe�L�@��

        void				setSettings( Settings *settingsPass );					
        void				setData( DataContainer *filedataPass );
		void				setPositionLabel( QLabel *_position );
		void				setRGBLabel( QLabel *_rgb );

		void				mark_point( int x, int y );								// �аO�I�b�e���W.
		QRgb				getRGB( int i, int j );									// ���o���I�y�Ъ� RGB ��

		void				mark_square( PIXEL pixel );								// �аO����ΰϰ�

    signals:
        void				mouseMoved( PIXEL pixel );								// openPIV�쥻��
		void				mousePressed(PIXEL pixel);								// �I���e�� �e�XPIXEL���

    public slots:
        void				refreshView();
		void				displaySVG( QString name );								// ���SVG�ɮ� �ɮ׼Ȧs�b�w��
		void				displayImage(QImage imagePass);							// ��ܹϤ�

    protected:
        void				resize();												// resize

        void 				wheelEvent(QWheelEvent* event);							// �Y�� (�u��)
        void				zoomIn();
        void				zoomOut();
        void				zoomFit();

        void 				mouseReleaseEvent( QMouseEvent* event );				// �ƹ���}�ʧ@
		void 				mouseMoveEvent( QMouseEvent* event );					// �ƹ����ʰʧ@
		void				updateMoveData( PIXEL pixel );							// ��s��ܸ�T (��ܵe�����U �y�� RGB���)
        void 				mousePressEvent(QMouseEvent* event);
        void 				resizeEvent(QResizeEvent* event);						// �ثe�L�@��





};
#endif // PIVDISPLAY_H
