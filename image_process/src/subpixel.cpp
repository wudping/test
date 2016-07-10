#include "stdafx.h"

#include "subpixel.h"

using namespace std;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~�o��O Edge ��function~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/*=============================================================================================
	�غc���
/*=============================================================================================*/
#ifdef _OPENPIV_
Edge::Edge(  Settings *settingsPass, DataContainer *filedataPass  )
	:	AndreaEngine( )
{
	progress	=	settingsPass->getProgress();	
	settings	=	settingsPass;
	filedata	=	filedataPass;

    mutex.lock();

	X		=	settings->get_measure_X();
	Y		=	settings->get_measure_Y();

	N		=	1;			// window size   2*N+1

    mutex.unlock();
}
#else
Edge::Edge() {}
#endif


/*=============================================================================================
	�Ѻc���
/*=============================================================================================*/
Edge::~Edge()
{}


/*=============================================================================================
	�esubpixel���I�W�h
/*=============================================================================================*/
void	Edge::draw_subpixel_point( double x, double y, double xx, double yy )
{
#ifdef _OPENPIV_
	QPointF		qf,	qf2;
	QString		path	=	"temp.svg";

	// �Nbmp��svg �d��
 
    QSvgGenerator	generator;
    generator.setFileName(path);

	generator.setSize( QSize(image.width(), image.height()) );
	generator.setViewBox( QRect(0, 0, image.width(), image.height()) );

    QPainter painter(&generator);
	painter.drawImage( QPoint(0,0), image  );

	painter.setPen( QPen(Qt::red, 0.02 ) );
	
	qf.setX( static_cast<qreal>(xx) );
	qf.setY( static_cast<qreal>(yy) );
	
	qf2.setX( static_cast<qreal>(x) );
	qf2.setY( static_cast<qreal>(y) );
	painter.drawLine( qf, qf2 );

    painter.begin(&generator);
    painter.end();
#endif
}


/*=============================================================================================
	�esubpixel���I�W�h
	bf = breakList  �z�����I�M��
/*=============================================================================================*/
void	Edge::draw_subpixel_point( QVector<QPointF> qf, QVector<QPointF> bf )
{
#ifdef _OPENPIV_
	QString		path	=	"temp.svg";
	QPointF		breaklist[1000];

	// �Nbmp��svg �d��
 
    QSvgGenerator	generator;
    generator.setFileName(path);

	generator.setSize( QSize(image.width(), image.height()) );
	generator.setViewBox( QRect(0, 0, image.width(), image.height()) );


    QPainter painter(&generator);
	painter.drawImage( QPoint(0,0), image  );

	// �]�w����
	painter.setPen( QPen(Qt::yellow, 1) );
	// �N��Ʀs��}�C
	for( int i=0; i<bf.size(); i++ )
		breaklist[i]	=	bf.at(i);
	// �e�z�����I
	painter.drawPoints( breaklist, bf.size() );


	// �]�w����
	painter.setPen( QPen(Qt::red, 0.1 ) );
	// �}�l�e�u 
	painter.drawLines( qf );

    painter.begin(&generator);
    painter.end();
#endif

}

/*=============================================================================================
	�����Ϊ��h����
	3*3  �|�ϥ�  1 + x + y + xy + x^2 + y^2 + x^2y + xy^2 + x^2y^2 �o�˪��h����
	��facet model���Ӥ@��
	dx, dy �O �����q
/*=============================================================================================*/
double	Edge::ite_poly( int dx, int dy, double x, double y, double xx, double yy )
{
	double	r	=	1;
	//double	xx	=	X + 0.5;
	//double	yy	=	Y + 0.5;

	int		i,	j;


	// ���U���k�O���F���ͤW����������model
	for( i=-N; i<=N; i++ )
	{
		if( i != dx )
		{
			r	*=	( x - xx - i );
			r	/=	( i - dx );
		}
	}

	for( j=-N; j<=N; j++ )
	{
		if( j != dy )
		{
			r	*=	( y - yy - j );
			r	/=	( j - dy );
		}
	}

	return	r;

}


/*=============================================================================================
	���ղ���SVG  (�ϥ�QWT)
/*=============================================================================================*/
void	Edge::testSVG()
{
	QString			path("QWT.svg");
	QSvgGenerator	generator;
	QImage			img		=	image;

	generator.setFileName(path);
	generator.setSize( QSize(img.width(), img.height()) );
	generator.setViewBox( QRect(0, 0, img.width(), img.height()) );
	generator.setTitle( "SVG Generator Example Drawing" );
	generator.setDescription( "An SVG drawing created by the SVG Generator Example provided with Qt." );

	//generator.setResolution(10);

	QPainter	painter( &generator );
	QwtPainter	qwtPainter;

	QRect		cf( 0,0, img.width(),img.height() );

	qwtPainter.drawImage( &painter, cf, img );

	QwtScaleMap		xMap;
    QwtScaleMap		yMap;
	xMap.setScaleInterval( 0, img.width() );
	yMap.setScaleInterval( 0, img.height() );


	/*QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin( true );
    grid->setMajPen( QPen( Qt::white, 0, Qt::DotLine ) );
    grid->setMinPen( QPen( Qt::gray, 0 , Qt::DotLine ) );
	grid->draw( &painter, xMap, yMap, cf );*/




	QwtPlotCurve	curve;
	int				Size	=	0,	a;
	double			xval[1000],	yval[1000];

	ifstream		in("subpix.txt");
	while( (in >> a) != NULL )
	{
		in >> xval[Size] >> yval[Size];
		Size++;
	}
	in.close();



	curve.setPen( QPen( QColor( 255, 0, 0 ), 0.02 ) );
    curve.setStyle( QwtPlotCurve::Lines );
	curve.setCurveAttribute( QwtPlotCurve::Fitted, true );
	
	QwtSplineCurveFitter* curveFitter;
	curveFitter = new QwtSplineCurveFitter();
    curveFitter->setSplineSize( 200 );
    
	curve.setCurveFitter( curveFitter );
	
    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::XCross );
    symbol->setPen( QPen( Qt::yellow, 0.1 ) );
    symbol->setSize( 0.1 );
    curve.setSymbol( symbol );
    //const QRect cr = contentsRect();

	curve.setRawSamples( xval, yval, Size );

	//QwtPlotItem::Rtti_PlotGrid

	xMap.setPaintInterval( cf.left(), cf.right() );
    yMap.setPaintInterval( cf.top(), cf.bottom() );

	//curve.setRenderHint( (QwtPlotItem::RenderHint)1, false );


	curve.draw( &painter, xMap, yMap, cf );



	painter.begin(&generator);
	//painter.setBrush( Qt::SolidPattern );

	painter.end();

}


/*=============================================================================================
	����edge 
	�o��ĥΪ��O�u��edge��V�h�j�M������

	�Q��gradient�h�M�wedge����V,�u�ۤ�V��X�I
	�A��o���Ilink��edge

	lu, rd �O���w�d��
/*=============================================================================================*/
QVector<QPointF>	Edge::extract_edge( PointInt lu, PointInt rd )
{
	int				i,		j;
	int				max_i,	max_j;
	const double	threshold	=	10000000;
	double			max,	min;
	double			dx,		dy;
	double			dtmp1,	dtmp2;
	double			normal;
	QImage			img;
	PointInt		start( X, Y );				// edge���j�M�_�I
	//PointInt		start( 770, 55 );
	//PointInt		start( 785, 263 );
	bMatrixDouble	corner;

	QVector<QPointF>	outputList;
	outputList.clear();

	// ���j�Mcorner�j���I
	Njet	njet(image);
	/*corner	=	njet.pix_corner( lu, rd );
	img		=	Matrix2Image( corner );			// �ন���� ��K����

	// non maximal suppression   �j�M�Xlocal maxima  (��ڤW�٬O�|������Ĳ�o �n�[threshold
	bMatrixInt		lm	=	local_max( corner, WINDOW(3,3), threshold );*/

	// ���U�O��X ��K����
	/*for( i = 0;  i < lm.size1(); i++ )
		for( j = 0; j < lm.size2(); j++ )
		{
			if( lm(i,j) == 1 )
				img.pixel_channel( i, j, 255, 0, 0 );
		}
	img.save("corner.bmp");*/


	// ����curvature
	curvature.resize( image.width(), image.height() );
	curvature.clear();
	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
			curvature( i, j )	=	njet.get_cur( i, j );
	/*max		=	0;
	min		=	12345678;
	for( i = 0;  i < image.width();  i++ )
		for( j = 0;  j < image.height();  j++ )
		{
			if( max < curvature( i, j ) )
				max		=	curvature(i,j);
			if( min > curvature( i, j ) )
				min		=	curvature(i,j);
		}
	for( i=0; i<image.width(); i++ )
		for( j=0; j<image.height(); j++ )
		{
			double	mag	=	(255.0)*(curvature(i,j)-min) / (max-min);
			curvature(i,j)	=	mag;
		}
	img	=	Matrix2Image(curvature);
	img.save("curvature.bmp");*/


	
	// �q�Y�Ӱ_�I�}�l  �u���䪺��V  �h��X��
	ofstream		out("subpix.txt");

	PointDouble		now( start.x + 0.5, start.y + 0.5 );
	//PointInt		pre		=	now;
	PointInt		step;
	//PointDouble		direction;
	PointDouble		delta;
	int				itr		=	0;
	while(1)
	{
		img.pixel_channel( now.x, now.y, 255, 0, 0 );

		i		=	now.x;
		j		=	now.y;

		// ���p��direction
		dtmp1	=	0.5 * ( image.gray_channel( i+1, j ) - image.gray_channel( i-1, j ) );
		dtmp2	=	0.5 * ( image.gray_channel( i, j+1 ) - image.gray_channel( i, j-1 ) );

		//dtmp1	=	image.gray_channel( i+1,j-1 ) - image.gray_channel( i-1, j-1 )  +  image.gray_channel( i+1, j+1) - image.gray_channel( i-1, j+1 )  +  5.0*(image.gray_channel(i+1,j) - image.gray_channel(i-1,j) );
		//dtmp2	=	image.gray_channel( i+1,j+1 ) - image.gray_channel( i+1, j-1 )  +  image.gray_channel( i-1, j+1) - image.gray_channel( i-1, j-1 )  +  5.0*(image.gray_channel(i,j+1) - image.gray_channel(i,j-1) );

		dx		=	dtmp2;
		dy		=	-dtmp1;
		normal	=	sqrt( dx*dx + dy*dy + 0.00001);
		dx		/=	normal;
		dy		/=	normal;
		dtmp1	/=	normal;
		dtmp2	/=	normal;

		if( dtmp1 == 0 && dtmp2 == 0 )
			dtmp1	=	0.5;

		delta	=	itepolation( now.x, now.y, dtmp1, dtmp2 );

		out << itr << " " << delta.x << " " << delta.y << endl;

		outputList.append( QPointF(delta.x, delta.y)  );

		// �U�@���I
		now.x	=	delta.x + dx;
		now.y	=	delta.y + dy;

		itr++;
		if( itr >= 1000 )
			break;

	}

	out.close();

	img.save("edge.bmp");

	return	outputList;
}



/*=============================================================================================
	���ե� 
	�ϥιL�I�������k 
	�h����curvature 
	�A�h�j�Mmax��edge subpixel
	dir_x, dir_y �O��V  (edge����V)
/*=============================================================================================*/
PointDouble		Edge::itepolation( int _x, int _y, double dir_x, double dir_y )
{
	double		rtx,	rty;
	double		xx	=	_x + 0.5;		// 4.5�o�˪� �B�I�Ƥ����I�y��
	double		yy	=	_y + 0.5;
	double		sum	=	0;
	double		x,	y,	t;
	int			i,	j;

	// �i�椺��  �åB��X�̤j��
	double		max		=	0;
	double		max_x	=	-1;
	double		max_y	=	-1;


	// �j�M�̤j��
	for( t = 0; ; t += 0.01 )
	{
		x	=	t*dir_x;
		y	=	t*dir_y;

		// �W�X���
		if( fabs(x) > 1.0 /*+ N*/ || fabs(y) > 1.0 /*+ N*/ )	break;

		sum		=	0;
		for( i = -N;  i <= N;  i++ )
			for( j = -N;  j <= N;  j++ )
				sum		+=	curvature( i + (int)xx, j + (int)yy ) * ite_poly( i, j, x + xx, y + yy, xx, yy  );

		if( sum > max )
		{
			max		=	sum;
			max_x	=	x + xx;
			max_y	=	y + yy ;
		}
	}

	// �ϦV�A�@��
	for( t = 0; ; t -= 0.01 )
	{
		x	=	t*dir_x;
		y	=	t*dir_y;

		// �W�X���
		if( fabs(x) > 1.0 /*+ N*/ || fabs(y) > 1.0 /*+ N*/ )	break;

		sum		=	0;
		for( i = -N;  i <= N;  i++ )
			for( j = -N;  j <= N;  j++ )
				sum		+=	curvature( i + (int)xx, j + (int)yy ) * ite_poly( i, j, x + xx, y + yy, xx, yy );

		if( sum > max )
		{
			max		=	sum;
			max_x	=	x + xx;
			max_y	=	y + yy ;
		}
	
	}

	if(max_x == -1 && max_y == -1 )
	{
		max_x	=	xx;
		max_y	=	yy;
	}



	return	PointDouble( max_x, max_y );

}

/*=============================================================================================
	OPENPIV���{���i�J�I
/*=============================================================================================*/
void	Edge::dosomething()
{
#ifdef _OPENPIV_
	progress->setTabText( 2, "vector pic" );

	image	=	filedata->get_image();

	//PointInt	lu( 1009, 82 ),	rd( 1095, 155 );
	PointInt	lu( 1, 1 ),	rd( image.width()-1, image.height()-1 );

	X		=	settings->get_measure_X();
	Y		=	settings->get_measure_Y();


	QVector<QPointF>	outputList;

	outputList	=	extract_edge( lu, rd );

	//testSVG();		// ���ղ���SVG  �ϥ�QWT

	//itepolation();
	//return;

// �P�_�O�B�z���I  �٬OPointList
#ifdef _SINGLE_

	double		rtx,	rty;
	
	double	xx	=	X + 0.5;		// 4.5�o�˪� �B�I�Ƥ����I�y��
	double	yy	=	Y + 0.5;

	// ��l�ʧ@
	image	=	filedata->get_image();

	// �Q��facet����ӳB�zsubpixel
	Facet	facet(3);

	facet.edge( (0.1+X), (0.1+Y), image, rtx, rty );		// ���F�קK�B�I�Ƴy�����~

	draw_subpixel_point( rtx, rty, xx, yy );
	
#ifdef _OPENPIV_
	progress->debuger( QString("subpixel edge = %1, %2").arg( rtx ).arg(rty ), QColor(24,0,240) );
#endif

#else
	int		i,		j;
	int		tmp;
	int		x,		y;
	int		count	=	0;
	double	rtx,	rty;

	// ��l�ʧ@

	image	=	filedata->get_image();


	// �Q��facet����ӳB�zsubpixel
	Facet	facet(3);

	QVector<QPointF>::iterator		itr;

	//QVector<QPointF>				PointList	=	filedata->getPointList();

	QVector<PIXEL>					pixel	=	filedata->getPixelList();
	QVector<QPointF>				PointList;

	for( i=0; i<pixel.size(); i++ )
		PointList.append( QPointF(pixel[i].fx, pixel[i].fy) );






	QVector<QPointF>				/*outputList,*/	pairList;
	QVector<QPointF>				breakList;						// �z�����I�M��
	QPointF							qf,	qf2;


	//outputList.clear();
	pairList.clear();
	breakList.clear();

	// �i��subpixel���p��
	for( itr = PointList.begin();        itr != PointList.end();         itr++ )
	{
		break;

		progress->setGress( (count++)%100 );

		qf	=	*itr;
		x	=	floor( qf.x() );
		y	=	floor( qf.y() );

		if( facet.edge( (0.1+x), (0.1+y), image, rtx, rty ) )		// ���F�קK�B�I�Ƴy�����~
			outputList.append(   QPointF( rtx, rty ) );
		else
		{
			breakList.append( QPointF( x, y ) );
			outputList.append( QPointF(-1. , -1.) );
		}
	}

	// ��z�I ���Xpair
	pairList.clear();
	for( i=0; i<outputList.size()-1; i++ )
	{
		qf	=	outputList.at(i);
		if( qf.x() >= 0 && qf.y() >= 0 )
			pairList.append( qf );
		else
		{	
			qf2	=	QPointF( floor(PointList.at(i).x()) + 0.5, floor(PointList.at(i).y()) + 0.5 );
			pairList.append( qf2 );
		}

		qf	=	outputList.at(i+1);
		if( qf.x() >= 0 && qf.y() >= 0 )
			pairList.append( qf );
		else
		{	
			qf2	=	QPointF( floor(PointList.at(i+1).x()) + 0.5, floor(PointList.at(i+1).y()) + 0.5 );
			pairList.append( qf2 );
		}
	}

	// �e�X���
	draw_subpixel_point( pairList , breakList );
	filedata->setOutputList( outputList );				// ��ƥγo�Ӥ覡�s��filedata
	progress->outputSubpixelEdge( outputList );			// �γo�䰵�e����s  �n�d�N�O�_�w�g���Ƨ�s�W�h�F  �p�G�|�浹���Pthread�n�d�N�P�B���D.
	

	// �M�����
	PointList.clear();
	outputList.clear();
	pairList.clear();

#endif
	
	progress->debuger("<font color=\"Red\"> finish Subpixel Edge detector... </font>");

	//emit( filedata->output3(image) );
	emit( filedata->output_svg("temp.svg"));

#endif		// _OPENPIV_
}











//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~�o��O Corner ��function~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



/*=============================================================================================
	�غc���
/*=============================================================================================*/
#ifdef _OPENPIV_
Corner::Corner(  Settings *settingsPass, DataContainer *filedataPass  )
	:	AndreaEngine()
{
	progress	=	settingsPass->getProgress();	
	settings	=	settingsPass;
	filedata	=	filedataPass;

    mutex.lock();

	X		=	settings->get_measure_X();
	Y		=	settings->get_measure_Y();

    mutex.unlock();
}
#else
Corner::Corner() {}
#endif


/*=============================================================================================
	�Ѻc���
/*=============================================================================================*/
Corner::~Corner()
{}


/*=============================================================================================
	�esubpixel���I�W�h
/*=============================================================================================*/
void	Corner::draw_subpixel_point( double x, double y, double xx, double yy )
{
#ifdef _OPENPIV_
	QPointF		qf,	qf2;
	QString		path	=	"temp.svg";

	// �Nbmp��svg �d��
 
    QSvgGenerator	generator;
    generator.setFileName(path);

	generator.setSize( QSize(image.width(), image.height()) );
	generator.setViewBox( QRect(0, 0, image.width(), image.height()) );

    QPainter painter(&generator);
	painter.drawImage( QPoint(0,0), image  );

	painter.setPen( QPen(Qt::red, 0.1 ) );
	
	qf.setX( static_cast<qreal>(xx) );
	qf.setY( static_cast<qreal>(yy) );
	
	qf2.setX( static_cast<qreal>(x) );
	qf2.setY( static_cast<qreal>(y) );
	painter.drawLine( qf, qf2 );

    painter.begin(&generator);
    painter.end();
#endif
}


/*=============================================================================================
	�esubpixel���I�W�h
/*=============================================================================================*/
void	Corner::draw_subpixel_point( QVector<QPointF> qf )
{
#ifdef _OPENPIV_
	QString		path	=	"temp.svg";

	// �Nbmp��svg �d��
 
    QSvgGenerator	generator;
    generator.setFileName(path);

	generator.setSize( QSize(image.width(), image.height()) );
	generator.setViewBox( QRect(0, 0, image.width(), image.height()) );


    QPainter painter(&generator);
	painter.drawImage( QPoint(0,0), image  );

	// �]�w����
	painter.setPen( QPen(Qt::red, 0.1 ) );

	// �}�l�e�u 
	painter.drawLines( qf );

    painter.begin(&generator);
    painter.end();

#endif
}




/*=============================================================================================
	OPENPIV���{���i�J�I
/*=============================================================================================*/
void	Corner::dosomething()
{
#ifdef _OPENPIV_
	progress->setTabText( 2, "vector pic" );

	// ��l�ʧ@
	image	=	filedata->get_image();

// �P�_�n���n���I�p�� �٬O�h�I
#if 1
	if( X < 0 || X >= image.width() )		X	=	0;			// ²�檺���b
	if( Y < 0 || Y >= image.height() )		Y	=	0;

	// �Q�� njet �Ӱ� sub-pixel �� corner �p��
	Njet	njet(image);

	double	dx,	dy;
	
	// openCV��code
	cornerSubPix( image, 0.00000000000000001 + X, 0.0000000000000000001 + Y, dx, dy );

	//int		ii	=	njet.get_corner( X, Y, dx, dy );

	int		ii;
	if( fabs(dx) > 0.5 || fabs(dy) > 0.5 )
		ii	=	-1;
	else
		ii	=	0;


	if( ii == -1 )
		progress->debuger( QString("out of point!!").arg(dx).arg(dy) );
	else if( ii == 0 )
		progress->debuger( QString("over max iter!!").arg(dx).arg(dy) );

	//progress->debuger( QString("Subpixel Corner = ( %1 , %2 )").arg(dx+X).arg(dy+Y) , QColor(33,9,253) );
	progress->debuger( QString("Subpixel Corner (delta) = ( %1 , %2 )").arg(dx).arg(dy) , QColor(33,9,253) );
	progress->debuger( QString("finish Subpixel Corner detector...") , QColor(255,0,0) );

	draw_subpixel_point( dx, dy, 0.5+X, 0.5+Y );
#else
	int		i,		j;
	int		tmp;
	int		x,		y;
	int		count	=	0;
	double	rtx,	rty;
	double	ip_x,	ip_y;

	// �Q��njet ����ӳB�zsubpixel
	Njet	njet(image);

	QVector<QPointF>::iterator		itr;
	QVector<QPointF>				PointList	=	filedata->getPointList();
	QVector<QPointF>				outputList,	pairList;
	QVector<QPointF>				breakList;						// �z�����I�M��
	QPointF							qf,	qf2;

	outputList.clear();
	pairList.clear();
	breakList.clear();

	// �i��subpixel���p��
	for( itr = PointList.begin();        itr != PointList.end();         itr++ )
	{
		progress->setGress( (count++)%100 );

		qf	=	*itr;
		x	=	floor( qf.x() );
		y	=	floor( qf.y() );

		if( njet.get_corner( x, y, rtx, rty ) == 1 )
			outputList.append( QPointF( rtx, rty ) );
		else
		{
			breakList.append( QPointF( x, y ) );
			outputList.append( QPointF(-1. , -1.) );
		}
	}

	// ��z�I ���Xpair
	pairList.clear();
	for( i=0; i<outputList.size()-1; i++ )
	{
		qf	=	outputList.at(i);
		if( qf.x() >= 0 && qf.y() >= 0 )
		{
			pairList.append( qf );
			ip_x	=	floor(qf.x()) + 0.5;
			ip_y	=	floor(qf.y()) + 0.5;
			pairList.append( qf );
		}

	}

	// �e�X���
	draw_subpixel_point( pairList );
	filedata->setOutputList( outputList );				// ��ƥγo�Ӥ覡�s��filedata
	progress->outputSubpixelEdge( outputList );			// �γo�䰵�e����s  �n�d�N�O�_�w�g���Ƨ�s�W�h�F  �p�G�|�浹���Pthread�n�d�N�P�B���D.
	

	// �M�����
	PointList.clear();
	outputList.clear();
	pairList.clear();



#endif


	// ��ϥ�h���
	//emit( filedata->output_svg("temp.svg"));


#endif			// _OPENPIV_
}