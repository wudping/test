#include "UncordObject.h"

#include <QMessageBox>
#include <QHostInfo>

#include <ThreadManager.h>
#include <common.h>

#include "NetWorkSpeed.h"
#include "TUTK_P2PTunnel.h"
#include "server/SFtpServer.h"
#include "server/AsyncHttpServer.h"
#include "server/HttpServer.h"

#include "DataManager.h"


#ifndef Q_MOC_RUN
#include <boost/bind.hpp>
#endif


using namespace std;




UNCORD_NAME_SPACE_BEGIN



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ extern global point ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern "C" {
	extern QObject		*p_main_window;		// 為了方便取用main window   可以思考有沒有其他做法
	extern QObject		*p_setting_widget;
}	// end extern "C"





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
UncordObject::UncordObject()
	:	p_thread_manager(NULL)
{
	p_network_speed			=	NULL;
	p_tutk_tunnel			=	NULL;
	p_async_http_server		=	NULL;
	p_sftp_server			=	NULL;
	p_http_server			=	NULL;
}






/**********************************************************
	destructor
***********************************************************/
UncordObject::~UncordObject()
{
	p_thread_manager	=	NULL;	// don't delete.

	SAFE_DELETE(p_network_speed)
	SAFE_DELETE(p_tutk_tunnel)
	SAFE_DELETE(p_async_http_server)
	SAFE_DELETE(p_sftp_server)
	SAFE_DELETE(p_http_server)
}




/**********************************************************
	init
***********************************************************/
void	UncordObject::init()
{
	LOG_MESSAGE( 3, "init client." );

	if( p_thread_manager == NULL )	
		BOOST_ASSERT(false);

	init_network_speed();
	init_tutk_tunnel();
	init_async_http_server();
	init_http_server();
	init_sftp_server();
}	


/**********************************************************
	init_http_server
***********************************************************/
void	UncordObject::init_http_server()
{
	if( p_http_server == NULL )
	{
		int		port	=	port_map[STR_MACRO(p_http_server)];

		p_http_server		=	new HttpServer();
		p_http_server->set_port(port);

		p_thread_manager->add_thread( boost::bind(&HttpServer::start,p_http_server) );
	}
	else
		BOOST_ASSERT(false);
}



/**********************************************************
	set_root
***********************************************************/
void	UncordObject::set_root( string root_path )
{
	p_sftp_server->set_root_path( root_path );
	p_async_http_server->set_root( root_path );
}




/**********************************************************
	init_sftp_server
***********************************************************/
void	UncordObject::init_sftp_server()
{
	if( p_sftp_server == NULL )
	{
		int		port	=	port_map[STR_MACRO(p_sftp_server)];

		p_sftp_server		=	new SFtpServer();
		p_sftp_server->set_port(port);

		p_thread_manager->add_thread( boost::bind(&SFtpServer::start,p_sftp_server) );
	}
	else
		BOOST_ASSERT(false);
}


/**********************************************************
	set_port
***********************************************************/
void	UncordObject::set_port( int port, int obj_type )
{
	switch( obj_type )
	{
		case ASYNC_HTTP_SERVER:
			port_map[STR_MACRO(p_async_http_server)]	=	port;
			break;
		case SFTP_SERVER:
			port_map[STR_MACRO(p_sftp_server)]			=	port;
			break;
		case HTTP_SERVER:
			port_map[STR_MACRO(p_http_server)]			=	port;
			break;
		default:
			BOOST_ASSERT(false);
	}
}




/**********************************************************
	init_async_http_server
***********************************************************/
void	UncordObject::init_async_http_server()
{
	if( p_async_http_server == NULL )
	{
		int		port	=	port_map[STR_MACRO(p_async_http_server)];

		p_async_http_server		=	new AsyncHttpServer();
		p_async_http_server->set_port(port);

		p_thread_manager->add_thread( boost::bind(&AsyncHttpServer::start,p_async_http_server) );
	}
	else
		BOOST_ASSERT(false);
}



/**********************************************************
	init_tutk_tunnel
***********************************************************/
void	UncordObject::init_tutk_tunnel()
{
	if( p_tutk_tunnel == NULL )
		p_tutk_tunnel		=	new TUTK_P2PTunnel();
	else
		BOOST_ASSERT(false);
}



/**********************************************************
	start_tutk_tunnel
***********************************************************/
void	UncordObject::start_tutk_tunnel()
{
	p_thread_manager->add_thread( boost::bind(&TUTK_P2PTunnel::main_tutk,p_tutk_tunnel) );
}




/**********************************************************
	get_download_value
***********************************************************/
float	UncordObject::get_download_value()
{
	if( p_network_speed != NULL )
		return	p_network_speed->get_download_value();
	else
	{
		BOOST_ASSERT(false);
		return	-1;
	}
}




/**********************************************************
	get_upload_value
***********************************************************/
float	UncordObject::get_upload_value()
{
	if( p_network_speed != NULL )
		return	p_network_speed->get_upload_value();
	else
	{
		BOOST_ASSERT(false);
		return	-1;
	}
}


/**********************************************************
	init_network_speed
***********************************************************/
void	UncordObject::init_network_speed()
{
	if( p_network_speed == NULL )
	{
		p_network_speed	=	new	NetWorkSpeed();
		p_thread_manager->add_thread( boost::bind(&NetWorkSpeed::start,p_network_speed) );
	}
	else
		BOOST_ASSERT(false);
}



/**********************************************************
	get_UID
***********************************************************/
void	UncordObject::get_UID()
{
	if( p_thread_manager != NULL )
		p_thread_manager->start( thread::NET_HTTP_GET_UID );
	else
		BOOST_ASSERT(false);
}


/**********************************************************
	inform_get_uid
***********************************************************/
void	UncordObject::inform_get_uid()
{
	emit get_uid_sig();
}



/**********************************************************
	refresh_device_list
***********************************************************/
void	UncordObject::refresh_device_list()
{
	//printf("test");
	emit( refresh_device_list_sig() );
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	get_qt_obj_instance
***********************************************************/
QObject*		get_qt_obj_instance( int type )
{
	QObject		*ptr	=	NULL;

	switch(type)
	{
		case SETTING_WIDGET:
			ptr		=	p_setting_widget;
			break;
		case MAIN_WINDOW:
			ptr		=	p_main_window;
			break;
		default:
			BOOST_ASSERT(false);
	}

	return	ptr;
}



/**********************************************************
	message_box
***********************************************************/
void	message_box( string str )
{
	QMessageBox::about(NULL, "msg", QString(str.c_str()) );
}



/**********************************************************
	get_machine_name
***********************************************************/
string		get_machine_name()
{
    QString local_host_name		=	QHostInfo::localHostName();
	return	local_host_name.toStdString();
}



UNCORD_NAME_SPACE_END
