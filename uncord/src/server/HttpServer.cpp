#include "server/HttpServer.h"

#define HTTP_SERVER_CPP
#include "HttpServer_priv.h"		// 要先定義 HTTP_SERVER_CPP 才能使用.

#ifndef Q_MOC_RUN
#include <boost/thread.hpp>
#endif

#include "UncordObject.h"


using namespace std;



UNCORD_NAME_SPACE_BEGIN


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
HttpServer::HttpServer()
	: NetServerBase()
{}




/**********************************************************
	destructor
***********************************************************/
HttpServer::~HttpServer()
{}





/**********************************************************
	init
***********************************************************/
void	HttpServer::init()
{}




/**********************************************************
	start

	目前採用多緒方式
	每個thread會再開thread去處理request.

***********************************************************/
void	HttpServer::start()
{
	LOG_MESSAGE( 3, "http server start" );

	string	str_port	=	int_to_string(port);
	boost::thread	*thr[THREAD_MAX];
	HttpServerPriv	http_server_priv;
	int		i;

	try	{
		// create server  (格式參考library.)  物件定義在 priv內.
		http_handler_struct			handler;
		http_server_type::options	options(handler);
		// 
		http_server_type			http_server( options.address(ip).port(str_port) );
		// bind handle_request
		handler.handle_request	=	boost::bind( &HttpServerPriv::handle_request, &http_server_priv, _1, _2 );

		// create threads.
		for( i = 0; i < THREAD_MAX; i++ )
			thr[i]	=	new boost::thread( boost::bind(&http_server_type::run,&http_server) );

		http_server.run();

	} catch ( std::exception &e ) {
        std::cerr << e.what() << std::endl;
		Uncord::message_box("error");
    }

	// delete
	for( i = 0; i < THREAD_MAX; i++ )
	{
		delete thr[i];
		thr[i]	=	NULL;
	}
}





UNCORD_NAME_SPACE_END
