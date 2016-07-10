/************************************************************************************************************************************
	
	����˴���b�o��

/*************************************************************************************************************************************/

#ifndef _COLOR_CODE_H
#define _COLOR_CODE_H

#include "stdafx.h"
#include "../BLOB/blob.h"
#include "../BLOB/watershed.h"
#include "channel.h"



#ifdef _OPENPIV_
#include "../src/andreaengine.h"
#include "../src/settings.h"
#include "../src/progress.h"
#include "../src/datacontainer.h"
#endif

#ifdef _OPENPIV_
class	ColorCode : public AndreaEngine
#else
class	ColorCode
#endif
{

	private:
		QImage			image;
		QImage			label;								// ��watershed label �N��Ф��}�ӤF
		QImage			graytone;							// ��gray tone


#ifdef _OPENPIV_
		DataContainer	*filedata;
		Settings		*settings;
		QMutex			mutex;
		Progress		*progress;							// �t�d�^���i�ץΪ�
		
	signals:
		void			debugSignal( QString );				// �����T��
#endif

	protected:
	public:

#ifdef _OPENPIV_
		ColorCode( Settings *settingsPass, DataContainer *filedataPass );		// �غc
		void		dosomething();												// ���Q�γo��funtion�Ӱ��Ʊ�. ���N�쥻��main
#else
		ColorCode();															// �غc �L�ǤJ��
#endif

		virtual		~ColorCode(); 												// �Ѻc


};




#endif