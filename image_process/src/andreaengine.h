#ifndef ANDREAENGINE_H
#define ANDREAENGINE_H

/***************************************************************************************************************************************************************
	�쫬 
	�Ҧ���Engine���~�ӥL
/****************************************************************************************************************************************************************/


#include <QMutex>
#include "settings.h"
#include "datacontainer.h"




class AndreaEngine
{
    public:
		// �غc
        AndreaEngine( );
		
        virtual			~AndreaEngine();

       // PivData			*operator() ( MetaData dataPass );

		virtual	void	dosomething()	=	0;			// �Ҧ��䴩�����󳣥����γo�ӨӰ���D�n���ʧ@

    protected:

    private:
        QMutex	mutex;

};

#endif // ANDREAENGINE_H
