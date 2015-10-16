#include "server/SFtpServer.h"

#include "UncordObject.h"

#ifndef Q_MOC_RUN
#include <boost/assert.hpp>
#endif

#ifdef USE_QT
#include <QLibrary>
#endif



UNCORD_NAME_SPACE_BEGIN


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
SFtpServer::SFtpServer()
	: NetServerBase()
{}




/**********************************************************
	destructor
***********************************************************/
SFtpServer::~SFtpServer()
{}




/**********************************************************
	start
***********************************************************/
void	SFtpServer::start()
{
	QLibrary	sftp_server_dll("sftp_lite_api");

	dll_sftp_funtion_t	sftp_server_start	=	(dll_sftp_funtion_t)sftp_server_dll.resolve("sftp_server_start");

	control		=	1;
	strcpy( root_path, "C:\\" );	// default root path.

	try{
		char	ip_c[1000];
		strcpy( ip_c, ip.c_str() );

		if(sftp_server_start)
			sftp_server_start( ip_c, port, SFTP_USERNAME, SFTP_PASSWORD, root_path , &control );
		else
			BOOST_ASSERT(false);
	} catch( exception &e ) {
		Uncord::message_box("error");
	}

}


/**********************************************************
	set_root
***********************************************************/
void	SFtpServer::set_root_path( string str )
{
	strcpy( root_path, str.c_str() );
	control		=	0;
	int	count	=	0;

	//
	while(control == 0 )
	{
		uncord_sleep(300);
		LOG_MESSAGE( 1, "wait" );
		count++;
		if( count > 30 )
		{
			LOG_MESSAGE( 6, "loop error" );
			BOOST_ASSERT(false);
			break;
		}
	}
}




/**********************************************************
	stop
	目前沒這功能orz|||
***********************************************************/
void	SFtpServer::stop()
{
	BOOST_ASSERT(false);
}




UNCORD_NAME_SPACE_END
