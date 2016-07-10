#ifndef PIVTHREAD_H
#define PIVTHREAD_H

#include <fstream>
#include <assert.h>

#include <QThread>
#include <QSemaphore>
#include <QVector>
#include <QMutex>
#include <QList>

#include "settings.h"
#include "datacontainer.h"

// blob detector
#include "../BLOB/blob.h"				// �ۦ漶�g��sub pixel blob detector
#include "../BLOB/watershed.h"			// �ۦ漶�g��watershed

// measure
#include "../MEASURE/FFT.h"				// �Q�Χֳt�ť߸��ഫ�Ӱ�junction detector
#include "../MEASURE/Steer.h"			// �Q��Steer filter�Ӱ�junction detector
#include "../MEASURE/subpixel.h"		// �i��U��subpixel���ʧ@ Edge Corner����
#include "../MEASURE/channel.h"			// �����m�q�D
#include "../MEASURE/colorcode.h"		// ����˴�

#include "../MEASURE/emboss.h"

#include "progress.h"


class PivThread : public QThread
{
    Q_OBJECT

    public:
        explicit PivThread( int _id, QSemaphore *freePass, QSemaphore *usedPass, QMutex *mutexPass/*, QList<int> listPass*/, QObject *parent = 0);
        virtual ~PivThread();

        void setSettings(Settings *settingsPass);
        void setFileData(DataContainer *filedataPass);

        int process();
        void stopProcess();

		void				setProgress( Progress *passGress );			// �]�wProgress
		emboss::EMBOSS_DIRECTION	int2direction( int a );						// �N����নdirection
		emboss::EMBOSS_DARK_LIGHT	int2dark( int a );							// �N����নdark_light

		int		id;		// ��thread��id


    protected:
        void	initializeProcessor();
		void	run();

		void	run2();

	signals:
		
		void	finished(int);

    private:


        QSemaphore *free;
        QSemaphore *used;
        QMutex *mutex;
        QList<int> list;

        Settings		*settings;
        bool			settingsSet;
        int				_processor;
        DataContainer	*filedata;
        bool			filedataSet;

        bool filter;
        bool analysisCreated;

        bool abort;

        //Processors
		AndreaEngine		*engine;						// �B��֤� �M�w�n����˪��ʧ@
		Progress			*progress;						// �t�d�^���i�ץ�

};

#endif // PIVTHREAD_H
