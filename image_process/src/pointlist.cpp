#include "pointlist.h"


/***************************************************************************************************
	�غc
/****************************************************************************************************/
PointList::PointList(QWidget* parent) 
	: QTableWidget(parent)
{
    setupConnections();
}


/***************************************************************************************************
	�Ѻc
/****************************************************************************************************/
PointList::~PointList()
{}



/***************************************************************************************************
	�]�w���U����connect  �ثe�L�@��
/****************************************************************************************************/
void PointList::setupConnections()
{}





/*========================================================================================
	�M����ܸ��
/*========================================================================================*/
void	PointList::clearData()
{
	this->clearContents();
	this->clear();
	setColumnCount(0);		// �N row  col �k�s
	setRowCount(0);
}





/*========================================================================================
	���������point
/*========================================================================================*/
void	PointList::removePointList( int row, int col )
{
	if( row >= rowCount() )
		return;

	//row + col;
	this->removeRow( row );

	if( rowCount() == 0 )
		clearData();					// �M�Ÿ��

	// ��s
	this->resizeColumnsToContents();
}


/*========================================================================================
	�������� 
	��s��ܦblist�W��
/*========================================================================================*/
void	PointList::updatePointList( const PIXEL pixel )
{
	QTableWidgetItem		*pointItem;
	QTableWidgetItem		*rgbItem;

	int		row		=	rowCount();
	int		col		=	2;
	int		x		=	pixel.x;
	int		y		=	pixel.y;

	QRgb	rgb		=	pixel.rgb;
	int		r		=	qRed( rgb );
	int		g		=	qGreen( rgb );
	int		b		=	qBlue( rgb );

	// �W�[��e
	this->setColumnCount( col );
	this->setRowCount( row + 1 );

	// ���ͭn��ܪ���r
	pointItem	=	new	QTableWidgetItem( QString("( %1 , %2 )").arg(x).arg(y) );
	//rgbItem		=	new	QTableWidgetItem( QString("( %1 , %2 , %3 )").arg(r).arg(g).arg(b) );
	rgbItem		=	new QTableWidgetItem( QString("size = %1").arg(pixel.square_size) );

	// ���ƨ�point list
	this->setItem( row , 0, pointItem );
	this->setItem( row , 1, rgbItem );




	//QComboBox		*ttt	=	new	QComboBox;
	//this->setItem( row, 2, ttt );



	// ��s�e��
	this->resizeRowsToContents();
	this->resizeColumnsToContents();
}
