#include "pivdisplay.h"



/************************************************************************************************
	�غc
/*************************************************************************************************/
PivDisplay::PivDisplay(QWidget* parent) 
	: QGraphicsView(parent)
{
    mouseIsPressed	=	false;
    this->setMouseTracking(true);
    this->setCursor(Qt::OpenHandCursor);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene					=	new QGraphicsScene;
	imPaint					=	NULL;

    viewRectFSet			=	false;
	isSVG					=	false;
}


/************************************************************************************************
	�Ѻc
/*************************************************************************************************/
PivDisplay::~PivDisplay()
{
	delete	imPaint;
	delete 	scene;			// delete���Ǥ����A��
}


/************************************************************************************************
	�]�wsetting
/*************************************************************************************************/
void	PivDisplay::setSettings( Settings *settingsPass )
{
    settings	=	settingsPass;
}

/************************************************************************************************
	�]�wfiledata
/*************************************************************************************************/
void	PivDisplay::setData( DataContainer *filedataPass )
{
    filedata	=	filedataPass;
}

/************************************************************************************************
	�]�wposition label
/*************************************************************************************************/
void	PivDisplay::setPositionLabel( QLabel *_position )
{
	positionLabel	=	_position;
}

/************************************************************************************************
	�]�wrgb label
/*************************************************************************************************/
void	PivDisplay::setRGBLabel( QLabel *_rgb )
{
	RGBLabel	=	_rgb;
}



/************************************************************************************************
	�Y��  
/*************************************************************************************************/
void PivDisplay::wheelEvent(QWheelEvent* event)
{
	// ��j
    if( event->delta() > 0 ) 
		zoomIn();
	// �Y�p
    else 
		zoomOut();
}

/************************************************************************************************
	�Y��  
	��j
/*************************************************************************************************/
void PivDisplay::zoomIn()
{
    this->scale(1.05,1.05);
    viewRectF.setWidth(viewRectF.width()/1.05);
    viewRectF.setHeight(viewRectF.height()/1.05);

    viewRectFSet	=	true;
}

/************************************************************************************************
	�Y��  
	�Y�p
/*************************************************************************************************/
void PivDisplay::zoomOut()
{
    this->scale(1.0/1.05,1.0/1.05);
    viewRectF.setWidth(viewRectF.width()*1.05);
    viewRectF.setHeight(viewRectF.height()*1.05);

    viewRectFSet	=	true;
}

/************************************************************************************************
	�Y��  
	fit
/*************************************************************************************************/
void PivDisplay::zoomFit()
{
    viewRectF		=	rectF;
    viewRectFSet	=	true;
    resize();
}


/************************************************************************************************
	�ƹ���}�ʧ@
/*************************************************************************************************/
void PivDisplay::mouseReleaseEvent( QMouseEvent* event )
{
    mouseIsPressed	=	false;

	QPointF		point;
	QRgb		rgb;
	int			x,	y;

	pix_2	=	MouseEvent2Pixel( event );

    this->setCursor(Qt::OpenHandCursor);
}



/************************************************************************************************
	�N�ƹ��T���ন PIXEL ��T
/*************************************************************************************************/
PIXEL	PivDisplay::MouseEvent2Pixel( QMouseEvent *event )
{
	const QPointF	point	=	mapToScene( QPoint(event->x(), event->y()) );		// �y���ഫ���ù��������y��
	const double	dx		=	point.x();
	const double	dy		=	point.y();
	const int		x		=	floor( dx );
	const int		y		=	floor( dy );
	const QRgb		rgb		=	getRGB( x, y );

	return	PIXEL( x, y, dx, dy, rgb );
}




/************************************************************************************************
	�ƹ����ʰʧ@
/*************************************************************************************************/
void	PivDisplay::mouseMoveEvent(QMouseEvent* event)
{
    double		dx,				dy;
    double		xCenter,		yCenter;
    int			xCenterView,	yCenterView;
	int			x,				y;
	QRgb		rgb;
	QPointF		point;

	// �즲�ƹ����A
    if (mouseIsPressed)
    {
        xCenterView		=	int(double(this->width() / 2.0));
        yCenterView		=	int(double(this->height() / 2.0));

		pix_2			=	MouseEvent2Pixel( event );

        dx				=	double( pix_2.x - pix_1.x );
        dy				=	double( pix_2.y - pix_1.y );

        xCenter			=	QPointF( this->mapToScene(QPoint(xCenterView,yCenterView)) ).x();
        yCenter			=	QPointF( this->mapToScene(QPoint(xCenterView,yCenterView)) ).y();
        centerPoint		=	QPointF( double(xCenter-dx),double(yCenter-dy) );

        this->centerOn(centerPoint);
        viewRectFSet	=	true;
    }
    else
	{
		pix_1			=	MouseEvent2Pixel( event );
	}

	// ��s��ܸ�T  ����ܵe�����U �y�и�T RGB��T
	updateMoveData( pix_1 );


	// ���v�˴� �e����W�h�Х�
	if( settings->set_square )
	{
		// �����e�Ϧbimage�W�����d��
		QImage		image2		=	QImage( image.width(), image.height(), QImage::Format_RGB888 );

		// ��painter�y�I
		QPainter	painter;		
		painter.begin(&image2);
		painter.setPen( QPen(Qt::green, 1) );
		painter.drawImage( 0, 0, image );
		//painter.drawPoint( pix_1.x, pix_1.y );
		painter.drawRect( pix_1.x, pix_1.y, settings->square_size, settings->square_size );
		painter.end();

		// �N��Ƶe�W�h
		scene->removeItem( imPaint );
		delete	imPaint;
		imPaint	=	new	ImagePaint( image2 );
		scene->addItem( imPaint );

		// ��s�e��
		//resize();
		refreshView();	
	}


}


/************************************************************************************************
	�ƹ����ʰʧ@
/*************************************************************************************************/
void	PivDisplay::updateMoveData( PIXEL pixel )
{
	const int	x		=	pixel.x;
	const int	y		=	pixel.y;
	const QRgb	rgb		=	pixel.rgb;
	const int	r		=	qRed( rgb );
	const int	g		=	qGreen( rgb );
	const int	b		=	qBlue( rgb );

	const QString	str1	=	QString( "( %1, %2, %3 )" ).arg(r).arg(g).arg(b);
	const QString	str2	=	QString( "( %1, %2 )" ).arg(x).arg(y);

	positionLabel->setText( str2 );		// ��s��y��
	RGBLabel->setText( str1 );			// ��s���C��
}



/************************************************************************************************
	�ثe�L�@��
/*************************************************************************************************/
void PivDisplay::resizeEvent(QResizeEvent *event)
{
    resize();
}


/************************************************************************************************
	resize
/*************************************************************************************************/
void PivDisplay::resize()
{
    this->fitInView(viewRectF, Qt::KeepAspectRatio);
    this->centerOn(centerPoint);
}


/************************************************************************************************
	�ثe�L�@�� 
/*************************************************************************************************/
QPointF PivDisplay::getPoint1()
{
	QPointF	point1	=	QPointF( pix_1.fx, pix_1.fy );

    return	point1;
}

/************************************************************************************************
	�ثe�L�@��
/*************************************************************************************************/
QPointF PivDisplay::getPoint2()
{
	QPointF	point2	=	QPointF( pix_2.fx, pix_2.fy );

    return point2;
}


/************************************************************************************************
	��s��ܵe��
/*************************************************************************************************/
void PivDisplay::refreshView()
{
    this->setSceneRect(rectF);
    this->setScene( scene );
    if (viewRectFSet)
    {
        //this->fitInView(viewRectF, Qt::KeepAspectRatio);
        //this->centerOn(centerPoint);
    }
    else
    {
        //this->fitInView(rectF, Qt::KeepAspectRatio);
        //centerPoint = QPointF(rectF.width()/2,rectF.height()/2);
        viewRectFSet = true;
    }
    this->show();
}



/************************************************************************************************
	�e�ϤW�h
/*************************************************************************************************/
void PivDisplay::displayImage( QImage imagePass )
{
	image			=	imagePass;
	rect			=	image.rect();
	rectF			=	QRectF( rect );
	viewRectF		=	rectF;
	viewRectFSet	=	true;
	
	// ��Ϥ���W�h
	scene->removeItem( imPaint );						// �R�����ǭn�`�N!!  ���M�|���
	delete	imPaint;
	imPaint		=	new	ImagePaint(image);
	scene->addItem( imPaint );

	isSVG	=	false;

	// ��s�e��
	//resize();
	refreshView();
}





/*======================================================================================
	���SVG�ɮ�
	�ɮ׼Ȧs�b�w��
	���ɦW�өI�s
/*======================================================================================*/
void	PivDisplay::displaySVG( QString name )
{
    if (1/*imPaintAdded*/)
    {
        image	=	filedata->get_image();
        rect	=	image.rect();
        rectF	=	QRectF(rect);

        viewRectF = rectF;
        viewRectFSet = true;

		// �q�ɮ�Ū��svg �b��svg���mainwindows����ܵe���W.
		//imPaint	=	new QGraphicsSvgItem(name);
		QGraphicsSvgItem	*imPaint2	=	new QGraphicsSvgItem(name);
		imPaint	=	new ImagePaint(image);

		scene->removeItem(imPaint);
        scene->addItem(imPaint);
		scene->removeItem( imPaint2 );
		scene->addItem( imPaint2 );

//        imPaintAdded = true;
    }
    else
    {
        image	=	filedata->get_image();
        rect	=	image.rect();
        rectF	=	QRectF(rect);
        if (viewRectF.width() == 0)
        {
            viewRectF = rectF;
            viewRectFSet = true;
        }

		// �q�ɮ�Ū��svg �b��svg���mainwindows����ܵe���W.
		QGraphicsSvgItem	*imPaint2	=	new QGraphicsSvgItem(name);
		imPaint	=	 new ImagePaint(image);
		//QPixmap buffer(image.size());
		//QGraphicsPixmapItem item(buffer);
		/*QGraphicsBlurEffect effect;
		effect.setBlurHints(QGraphicsBlurEffect::QualityHint);
		effect.setBlurRadius(0);
		imPaint2->setGraphicsEffect(&effect);*/ 
		//item.pixmap().toImage().save("blur.png");
		//imPaint	=	new ImagePaint(*image);
        scene->addItem(imPaint);
        scene->addItem(imPaint2);
//        imPaintAdded = true;
    }

	isSVG	=	true;

	resize();
	refreshView();
}


/*==================================================================================================
	���o���I�y�Ъ� RGB ��
/*==================================================================================================*/
QRgb	PivDisplay::getRGB( int i, int j )
{
	QRgb	rgb;

	if( i < 0 || i >= image.width() || j < 0 || j >= image.height() )
		rgb		=	qRgb( 0, 0, 0 );
	else
		rgb		=	image.pixel( i, j );

	return	rgb;
}





/*==================================================================================================
	�I���ƹ�event
	�|�b�e���W���аO�I (�w�]�ά���)
	�o��令�аO�˴��Ϫ��خئb�Ϥ��W 
/*==================================================================================================*/
void PivDisplay::mousePressEvent(QMouseEvent* event)
{
	pix_1	=	MouseEvent2Pixel( event );

    this->setCursor(Qt::ClosedHandCursor);
    mouseIsPressed = true;

	int		x	=	pix_1.x;
	int		y	=	pix_1.y;

	QRgb	rgb		=	getRGB( x, y );

	settings->setRGB( rgb );					// �������I���C���T
	settings->set_measure_X( x );
	settings->set_measure_Y( y );

	PIXEL	pixel( x+1, y+1, pix_1.fx, pix_1.fy, rgb );
	pixel.square_size	=	settings->square_size-1;

	if( event->button() == Qt::LeftButton && settings->set_square )
		emit(mousePressed(pixel));					// �e�Xpixel��T��mainwindow �A�qmainwindow�e��point list

	// �Dsvg�~��mark point
	if( !isSVG && settings->set_square && event->button() == Qt::LeftButton )
		mark_point( x, y );

}



/*==================================================================================================
	�b�e���W�аO�I
/*==================================================================================================*/
void	PivDisplay::mark_point( int x, int y )
{
	// �����e�Ϧbimage�W�����d��
	QImage		image2		=	QImage( image.width(), image.height(), QImage::Format_RGB888 );

	// ��painter�y�I
	QPainter	painter;		
	painter.begin(&image2);
	painter.setPen( QPen(Qt::red, 1) );
	painter.drawImage( 0, 0, image );
	painter.drawPoint( x, y );
	painter.end();

    // �N��Ƶe�W�h
	scene->removeItem( imPaint );
	delete	imPaint;
	imPaint	=	new	ImagePaint( image2 );
	scene->addItem( imPaint );

	// ��s�e��
	//resize();
	refreshView();
}

/*==================================================================================================
	�аO����ΰϰ�
/*==================================================================================================*/
void	PivDisplay::mark_square( PIXEL pixel )
{
	// �����e�Ϧbimage�W�����d��
	QImage		image2		=	QImage( image.width(), image.height(), QImage::Format_RGB888 );

	int		x	=	pixel.x;
	int		y	=	pixel.y;

	int		square_size	=	pixel.square_size;

	// ��painter�y�I
	QPainter	painter;		
	painter.begin(&image2);
	painter.setPen( QPen(Qt::blue, 1) );
	painter.drawImage( 0, 0, image );
	//painter.drawPoint( x, y );
	painter.drawRect( x-1, y-1, square_size+1, square_size+1 );
	painter.end();

    // �N��Ƶe�W�h
	scene->removeItem( imPaint );
	delete	imPaint;
	imPaint	=	new	ImagePaint( image2 );
	scene->addItem( imPaint );

	// ��s�e��
	//resize();
	refreshView();
}

