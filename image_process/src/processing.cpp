#include <QObject>
#include <QPoint>
#include <QStringList>

#include <QSemaphore>
#include <QThread>
#include <QMutex>
#include <iostream>
#include <fstream>

#include "settings.h"
#include "processing.h"
#include "pivthread.h"
#include "batchwindow.h"


#include "progress.h"

// �t�d�B�zthread�P�B
QSemaphore	freePairs(10);
QSemaphore	usedPairs;
QMutex		mutex;


/*********************************************************************************************************************
	�غc���
/**********************************************************************************************************************/
Processing::Processing(Settings *settingsPass, DataContainer *filedataPass, QObject *parent) 
	: QObject(parent)
{
    filedata		=	filedataPass;
    settings		=	settingsPass;

	// �妸window
    batchWindow		=	new BatchWindow;
    batchWindow->setSettings(settings);

    connect(batchWindow, SIGNAL(startProcess()), this, SLOT(processBatch()));
    connect(batchWindow, SIGNAL(stopProcess()), this, SLOT(stopBatch()));

    threadsCreated	=	false;

	progress		=	settingsPass->getProgress();
	connect( progress, SIGNAL(signalsGress(int)), this, SLOT(progress_add(int)) );
	connect( progress, SIGNAL(signalTabText(int,QString)), this, SLOT(setTabText(int,QString)) );


	isProcess		=	false;

	// Andrea �h����o��. �]���쥻����m�h�÷|���
	//initializeThreads();
}


/*********************************************************************************************************************
	�Ѻc���
/**********************************************************************************************************************/
Processing::~Processing()
{
    delete batchWindow;
    deleteThreads();
	delete	progress;
}


/*********************************************************************************************************************
	�R��thread
/**********************************************************************************************************************/
void Processing::deleteThreads()
{
	progress->debuger("Processing::deleteThreads...");

    if (threadsCreated)
    {
        while (threadList.size() > 0)
		{
            delete threadList.takeFirst();
		}
		threadList.clear();
        threadsCreated = false;
    }
	isProcess	=	false;
}


/*********************************************************************************************************************
	�妸window
/**********************************************************************************************************************/
void Processing::launchBatchWindow()
{
    batchWindow->refresh();
    batchWindow->show();
}



/*********************************************************************************************************************
	����妸�ʧ@
/**********************************************************************************************************************/
void Processing::stopBatch()
{
    for (int i = 0; i < threadList.size(); i++)
    {
        threadList.value(i)->stopProcess();
    }
}



/*********************************************************************************************************************
	�e�X�妸�T��
/**********************************************************************************************************************/
void Processing::emitBatchProcessed()
{
    //std::cout << "processed\n";
    emit(batchProcessed());
}


/*********************************************************************************************************************
	���}thread
	�ثe�L�@��
/**********************************************************************************************************************/
void Processing::processBatchSerial()
{}




/*====================================================================================
	Andrea
	mainwindows�i�Jprocess���i�J�I
/*====================================================================================*/
void	Processing::processBatch()
{
	//QString		str	=	QString("Processing::processBatch");
	//emit(debugSignal(str) );

	progress->debuger( "Processing::processBatch..." );

	// ���b�����Lprocess,�צ�
	if( isProcess )
	{
		progress->debuger("now is process!!  please wait...", QColor(0,255,0) );
		return;
	}


   // batchWindow->setProgressRange(filedata->size());
	//batchWindow->setProgressRange(1);

    if (settings->batchThreading()) 
		processBatchParallel();
    else 
		processBatchSerial();

}


/*====================================================================================
	Andrea
	�]�w Gauss Filter �� scale w
	���ӻݭn�b�o��[�Wmulti thread���P�_
	�����h�p��O�_�C��thread��finished.
/*====================================================================================*/
void	Processing::finish( int id )
{
	progress->debuger( QString("%1 - process finished...").arg(id)   , QColor(0,50,0) );
	deleteThreads();
}




/*====================================================================================
	Andrea
	����process���i�J�I (�w�]�����)
/*====================================================================================*/
void Processing::processBatchParallel()
{
	progress->debuger( QString("Processing::processBatchParallel...") );

	// �g�b�o��  �]�ĤG�����ɭԳy�����...
    initializeThreads();
    for (int i = 0; i < threadList.size(); i++)
	{
		progress->debuger( QString("%1 - process...").arg(i) );
        threadList.value(i)->process();
    }
}





/*====================================================================================
	�]�w�i�ױ�
/*====================================================================================*/
void Processing::progress_add(int value)
{
	emit( signalsGress(value) );
}



/*====================================================================================
	�]�w�D�e��tab����r
/*====================================================================================*/
void Processing::setTabText(int index, QString str)
{
	emit( signalTabText(index, str) );
}



/*====================================================================================
	process ��l��thread
/*====================================================================================*/
void Processing::initializeThreads()
{
	isProcess			=	true;
	threadsCreated		=	true;

    int		datasize	=	1;


    int i;
    int N = 1;//QThread::idealThreadCount();			// ����A�令�ʺA


	QString	str	=	"Processing::initializeThreads";
	str.append('0'+N);
	emit(debugSignal(str) );


	// ���N�ª���ƲM��
	threadList.clear();

    for (i = 0; i < N; i++)
    {
		//pivthread	=	new PivThread(&freePairs,&usedPairs,&mutex ,  splitList(i,N,datasize) ) ;
		
        threadList.append(new PivThread( i, &freePairs,&usedPairs,&mutex)  );
//		threadList.append( pivthread );
		threadList.value(i)->setProgress(progress);			// �Nprogress��i�h  �t�d�^���i��
        threadList.value(i)->setSettings(settings);
        threadList.value(i)->setFileData(filedata);
		connect( threadList.value(i), SIGNAL(finished(int)), this, SLOT(finish(int)) );
		//connect(   pivthread, finished(), this, stopBatch() );
    }

}