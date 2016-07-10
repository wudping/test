/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	�U�ظ�Ʃ�b�o��

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <fstream>
#include <iostream>

#include <QStringList>
#include <QObject>
#include <QImage>
#include <QVector>
#include <boost/numeric/ublas/matrix.hpp>

#include "settings.h"
#include "../tools.h"


//! Container class for MetaData objects.
class DataContainer : public QObject
{
#ifndef _GTEST_
    Q_OBJECT
#endif

	private:
		bMatrixInt			map;													// �ΨӰO�����I�O�_���Q�[��list�̭�							
        Settings			*settings;

		QVector<PIXEL>		pixelList;												// pixel��T�s�b�o��
		QVector<QPointF>	outputList;												// ��X��T

		QImage				image;													// Andreaw�s�W Ū�J�����ɸ��.

    public:
        DataContainer(Settings *settingsPass, QObject *parent = 0);
        virtual ~DataContainer();

		// �N���ɸ��Ū�X
		QImage				get_image();
		QString				filename;												// Ū�������ɦW��

		QVector<PIXEL>		getPixelList();											// �^�� PIXEL List
		QVector<QPointF>	getOutputList();
		
		void				output_svg( QString name );								// �N��ƥ�svg���覡��X  (�ɮש�b�w��  �q�w�и̭�Ū���i��)
		void				setOutputList( QVector<QPointF> qf );					// �ΦbwidgeB �ǤJ���
		bool				addPointList( PIXEL pixel );							// �W�[�I��list�M��
		void				clearPixelList();										// �M��pointlist
		void				removePointList( int row, int col );					// �����I�쪺����
		PIXEL				get_square( int row, int col );							// ���v�˴���  �^��square����T  (�D�e���I�쪺�ɭ�)

#ifndef _GTEST_
    public slots:
#endif
		void				loadBMP( QImage imagePass );							// Ū��BMP  Andrea

#ifndef _GTEST_
    signals:
		void				debugSignal( QString );									// �����T��
		void				svgChanged( QString );
#endif



};

#endif // DATACONTAINER_H
