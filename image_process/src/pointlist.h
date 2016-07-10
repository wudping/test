/*================================================================================================================================

	�D���� ���U�����list  
	�ثe�O���h��point list

/*================================================================================================================================*/

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QObject>
#include <QVector>
#include <QTableWidget>
#include <QHeaderView>

#include "datacontainer.h"
#include "../tools.h"

class PointList : public QTableWidget
{
    Q_OBJECT

    private:

    public:
        PointList( QWidget* parent = 0 );
        ~PointList();
	
		void				updatePointList( const PIXEL pixel );							// �������� ��s��ܦblist�W��
		void				clearData();													// �M����ܸ��
		void				removePointList( int row, int col );							// ���������pointlist
		void				setupConnections();

    signals:
        void 				fileClicked(QString fileName, int rowSelected);
        void 				fileClicked(int rowSelected);
        void 				itemEntered(QTableWidgetItem* item);
};

#endif // IMAGELIST_H
