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
	//SET_DATA_MACRO( string, root_path );		// 因為目前set path會有附加作用 所以不寫在這邊.

protected:
	int			port;
	string		ip;
	//string		root_path;

private:


};


UNCORD_NAME_SPACE_END


#endif











/*
偵測ip的code  (應該是)


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