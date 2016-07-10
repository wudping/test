#include <QStringList>
#include <QObject>
#include <iostream>
#include <QFileDialog>

#include "settings.h"
#include "datacontainer.h"

using namespace std;



/*=================================================================================
	�غc���
/*=================================================================================*/
DataContainer::DataContainer(Settings *settingsPass, QObject *parent) : QObject(parent)
{
    settings			=	settingsPass;

//	GFilgerImageCreated		=	false;
	pixelList.clear();
}


/*=================================================================================
	�Ѻc���
/*=================================================================================*/
DataContainer::~DataContainer()
{}




/*=================================================================================
	Ū��BMP
	Andrea�s�W
/*=================================================================================*/
void	DataContainer::loadBMP( QImage imagePass )
{
	image	=	imagePass;

	// ��l��map
	map.resize( image.width(), image.height() );
	map.clear();
}



/*=================================================================================
	�N�B�z�᪺�v�����mainwindow����ܵ����W��
	Andrea�s�W
	��X�����2
/*=================================================================================*/
void	DataContainer::setOutputList( QVector<QPointF> qf )
{
	outputList	=	qf;
}


/*=================================================================================
	�N��ƥ�svg���覡��X  (�ɮש�b�w��  �q�w�и̭�Ū���i��)
/*=================================================================================*/
void	DataContainer::output_svg( QString name )
{
#ifndef _GTEST_
	emit( svgChanged(name) );
#endif
}

/*=================================================================================
	�^��BMP
	Andrea�s�W
/*=================================================================================*/
QImage	DataContainer::get_image()
{
	if( !image.isNull() )
		return	image;

	else
		return	QImage( 100, 100, QImage::Format_RGB888 );		// 100*100���ťչ�

}

/*=================================================================================
	�M��pointlist
/*=================================================================================*/
void	DataContainer::clearPixelList()
{
	outputList.clear();
	pixelList.clear();
	map.clear();
}

/*=================================================================================
	�^��outputList
/*=================================================================================*/
QVector<QPointF>	DataContainer::getOutputList()
{
	return	outputList;
}




/*=================================================================================
	�^��Pixel List
/*=================================================================================*/
QVector<PIXEL>	DataContainer::getPixelList()
{
	return	pixelList;
}





/*=================================================================================
	�W�[�I��list�M��
/*=================================================================================*/
bool	DataContainer::addPointList( PIXEL pixel )
{
	int		x	=	pixel.x;
	int		y	=	pixel.y;

	if( x<0 || x>=map.size1() || y<0 || y>map.size2() )
		return	false;
	
	// ���\�s�W�I ��L���O����
	else if( pixelList.size() < 100 && map(x,y) != 1 )
	{
		map(x,y)	=	1;
		pixelList.append( pixel );
		return	true;
	}

	else if( map(x,y) == 1 )
	{
		//debugSignal("DataContainer::addPointList this point has been added!!");
		return	false;
	}

	else
	{
		//debugSignal("DataContainer::addPointList full list!!");
		return	false;
	}
}



/*=================================================================================
	�����I���쪺����
/*=================================================================================*/
void	DataContainer::removePointList( int row, int col )
{
	if( row >= pixelList.size() )
		return;

	pixelList.remove( row );
}


/*=================================================================================
	���v�˴���  �^��square����T  
	(�D�e���I�쪺�ɭ�)
/*=================================================================================*/
PIXEL	DataContainer::get_square( int row, int col )
{
	return	pixelList.at(row);
	//pixelList.remove( row );
}