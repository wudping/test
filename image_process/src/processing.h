#ifndef PROCESSING_H
#define PROCESSING_H

#include <QStringList>
#include <QPoint>
#include <QObject>

#include <QSemaphore>
#include <QThread>
#include <QMutex>

#include "datacontainer.h"
#include "settings.h"
#include "batchwindow.h"
#include "output.h"
#include "pivthread.h"
#include "progress.h"

//#include "../BLOB/gaussfilter.h"

class Processing : public QObject 
{
    Q_OBJECT

    public:
        explicit	Processing(Settings *settingsPass, DataContainer *filedataPass, QObject *parent = 0);
        virtual		~Processing();

    public slots:
        void	processBatch();

		void	progress_add( int value );				// �]�w�i�ױ�
		void	setTabText( int index, QString str );	// �]�w�D�e����tab��r
		void	finish( int id );						// ����process   �ثe�u��single thread


    signals:
        void	currentProcessed();
        void	batchProcessed();

		void	debugSignal( QString );					// �����T��

		void	imageThrow(QImage);						// ��X�Ϥ�
		void	signalsGress(int);						// ����i�ױ�
		void	signalTabText( int, QString );			// �ק�D�e����tab��r����


    protected slots:
        void	launchBatchWindow();
        void	emitBatchProcessed();
        void	stopBatch();

    protected:
        void	processBatchSerial();
        void	processBatchParallel();
        void	initializeThreads();
        void	deleteThreads();


    private:
        Settings				*settings;
        DataContainer			*filedata;
        BatchWindow				*batchWindow;

        QList<PivThread*>		threadList;
        bool					threadsCreated;

		//GaussFilter_2D			gauss_filter;			// Andrea  Gauss Filter
		Progress				*progress;				// �i�ױ�����

		bool					isProcess;				// �����O�_���b����process  �O���ܭn���

};

#endif // PROCESSING_H
