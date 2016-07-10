#ifndef PROGRESS_H
#define PROGRESS_H

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	���F����L����i�H����i�ױ�
	�ҥH�g�@�Ӫ����i�h  �γo�Ӫ���ӱ���i�ױ�
	���ӻݭn���ܦA�[�W��L�\��= ="
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include <stdio.h>
#include <QObject>
#include <QString>
#include <QColor>
#include <QVector>
#include <QImage>
#include <QPointF>

class Progress : public QObject
{
#ifndef _GTEST_
    Q_OBJECT
#endif

    public:
        explicit	Progress();
        virtual		~Progress();

		void		setGress( int value );									// �]�w�i�ױ�
		void		setTabText( int index, QString str );					// �]�w�D�e�� tab �W����r����
		void		debuger( QString );										// �Ldebug�T��
		void		debuger( QString , QColor color  );						// �Ldebug�T�� �åB���C��
		void		outputSubpixelEdge( QVector<QPointF> qf);				// ��Xedge subpixel�����
		void		outputImage( int index, QImage image );					// ��Ximage����ܵ���

#ifndef _GTEST_
	public slots:
#endif
	
#ifndef _GTEST_
    signals:
		void		signalsGress(int);
		void		debugSignal( QString );
		void		signalTabText(int,QString);
		void		updateWidgeB();											// ��X�T���� widge B
		void		signalImage( int, QImage );								// ��Ximage����ܵ���
#endif

    private:


};

#endif // PROGRESS_H
