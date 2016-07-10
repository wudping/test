

#include "progress.h"



Progress::Progress() {}

Progress::~Progress() {}



/*===================================================================
	�]�w�i�ױ�����
/*===================================================================*/
void	Progress::setGress( int value )
{
#ifndef _GTEST_
	emit( signalsGress(value) );
#endif
}



/*===================================================================
	�]�w�D�e�� tab �W����r����
/*===================================================================*/
void		Progress::setTabText( int index, QString str )
{
#ifndef _GTEST_
	emit( signalTabText(index, str) );
#endif
}

/*===================================================================
	��Xedge subpixel�����
/*===================================================================*/
void		Progress::outputSubpixelEdge( QVector<QPointF> qf )
{
#ifndef _GTEST_
	emit( updateWidgeB() );
#endif
}


/*===================================================================
	��ܰT����D�e����debug_text�W��
/*===================================================================*/
void		Progress::debuger( QString str )
{
#ifndef _GTEST_
	emit( debugSignal(str) );
#endif
}


/*===================================================================
	��ܰT����D�e����debug_text�W��
/*===================================================================*/
void	Progress::debuger( QString str, QColor color  )
{
	int	r	=	color.red();
	int	g	=	color.green();
	int	b	=	color.blue();

	char	tmp[100];

	sprintf( tmp, "\"#%02x%02x%02x\"", r, g, b);

	//str		=	"<font color = " + tmp + " > " + str + "</font>";
	QString		str2	=	"<font color=";
	str2.append( tmp );
	str2	+=	" > " + str + "</font>";

#ifndef _GTEST_
	emit( debugSignal(str2) );
#endif
	//progress->debuger("<font color=\"Red\"> finish subpixel blob detector... </font>");
}

/*===================================================================
	��Ximage����ܵ���
/*===================================================================*/
void	Progress::outputImage( int index, QImage image )
{
#ifndef _GTEST_
	emit( signalImage( index, image ) );
#endif
}




