#include "server/AsyncHttpServer.h"

#define ASYNC_HTTP_SERVER_CPP
#include "AsyncHttpServer_priv.h"		// 避免其他檔案include priv


#ifndef Q_MOC_RUN
#include <boost/thread.hpp>
#endif

#include <fstream>



using namespace std;




UNCORD_NAME_SPACE_BEGIN


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
AsyncHttpServer::AsyncHttpServer()
	: NetServerBase()
{
	root_path	=	NULL;
}




/**********************************************************
	destructor
***********************************************************/
AsyncHttpServer::~AsyncHttpServer()
{}




/**********************************************************
	init
***********************************************************/
void	AsyncHttpServer::init()
{}




/**********************************************************
	set_root
***********************************************************/
void	AsyncHttpServer::set_root( string str )
{
	strcpy( root_path, str.c_str() );
}





/**********************************************************
	start

	目前採用多緒方式
	每個thread會再開thread去處理request.

***********************************************************/
void	AsyncHttpServer::start()
{
	stringstream	ss;
	ss << port;
	string	str_port	=	ss.str();

	try	{
		async_http_handler_struct handler;
		// 這邊之後再來rename
		file_server::options options(handler);

		file_server instance(
				options.thread_pool(boost::make_shared<boost::network::utils::thread_pool>(THREAD_MAX)).address(ip).port(str_port)	
			);

		root_path	=	handler.root_path;
		instance.run();
	} catch ( exception &e ) {
		printf("error");
	}
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





UNCORD_NAME_SPACE_END
