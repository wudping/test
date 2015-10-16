#ifndef NET_SERVER_BASE_H
#define NET_SERVER_BASE_H

#include "NetBase.h"



UNCORD_NAME_SPACE_BEGIN




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	NetServerBase
***********************************************************/
class	NetServerBase : public NetBase
{
public:
	NetServerBase();
	virtual ~NetServerBase();

	SET_DATA_MACRO( int, port );
	SET_DATA_MACRO( string, ip );
	//SET_DATA_MACRO( string, root_path );		// �]���ثeset path�|�����[�@�� �ҥH���g�b�o��.

protected:
	int			port;
	string		ip;
	//string		root_path;

private:


};


UNCORD_NAME_SPACE_END


#endif











/*
����ip��code  (���ӬO)


#include <QtCore/QCoreApplication>
#include <QtNetwork/QNetworkInterface>


bool isConnectedToNetwork()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        if ( iface.flags().testFlag(QNetworkInterface::IsUp)
             && !iface.flags().testFlag(QNetworkInterface::IsLoopBack) )
        {

#ifdef DEBUG
            // details of connection
            qDebug() << "name:" << iface.name() << endl
                    << "ip addresses:" << endl
                    << "mac:" << iface.hardwareAddress() << endl;
#endif

            // this loop is important
            for (int j=0; j<iface.addressEntries().count(); j++)
            {
#ifdef _DEBUG
                qDebug() << iface.addressEntries().at(j).ip().toString()
                        << " / " << iface.addressEntries().at(j).netmask().toString() << endl;
#endif

                // we have an interface that is up, and has an ip address
                // therefore the link is present

                // we will only enable this check on first positive,
                // all later results are incorrect
                if (result == false)
                    result = true;
            }
        }

    }

    return result;
}




*/