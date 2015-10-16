#pragma once

// 希望這個檔案只能被HttpServer.cpp, HttpServer_priv.cpp引用.
#if !(defined(HTTP_SERVER_CPP) | defined(HTTP_SERVER_PRIV_CPP))
#error
#endif


// 一定要定義Q_MOC_RUN  不然會發生編譯錯誤 
#ifndef Q_MOC_RUN
#include <boost/function.hpp>
#include <boost/mpl/if.hpp>
#include <boost/network/protocol/http/server.hpp>
#endif 


#include <string>
#include <stdio.h>

using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define		HTTP_SERVER_OK					1
#define		HTTP_SERVER_DEFAULT				0
#define		HTTP_SERVER_ERR					-1
#define		HTTP_SERVER_ERR_PARAMETER		-2
#define		HTTP_SERVER_ERR_METHOD			-3
#define		HTTP_SERVER_ERR_CMD				-4
#define		HTTP_SERVER_ERR_HEADER			-5


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct		http_handler_struct;														// 因為netlib設計關係 必須寫在這邊.
typedef		boost::network::http::server<http_handler_struct>	http_server_type;			// 格式參考cpp netlib  



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum HTTP_GET_CMD
{
	HTTP_VALIDATE_DEVICE,
};


enum HTTP_METHOD_TYPE
{
	HTTP_GET,
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct	http_handler_struct
{
	// callback function.
	boost::function< int(http_server_type::request const&, 
						 http_server_type::response &)>		handle_request;

	// operator ----------------------------------------------------------
    void operator () ( http_server_type::request const &request,
                       http_server_type::response &response ) 
	{
		log( "http handler" );
		//log("receive\n");
		if( handle_request )
		{
			int	res		=	handle_request( request, response );
			if( res <= 0 )
				log( "res <= 0" );
		}
		else
			BOOST_ASSERT(false);
    }

	// log ----------------------------------------------------------
    void log( const char *msg )
	{
		printf( "%s\n", msg );
		//std::cout << msg;
    }
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class	HttpServerPriv
{
public:
	HttpServerPriv();
	~HttpServerPriv();

	int		handle_request( http_server_type::request const &request, 
							http_server_type::response &response );

	int		handle_get( http_server_type::request const &request, 
						http_server_type::response &response );

	int 	validate_device( http_server_type::request const &request, 
							 http_server_type::response &response, 
							 string para );


	int		trans_method( string method );
	int		trans_get_cmd( string cmd );

	map<string,string>	parse_destination( string dest );
	map<string,string>	parse_parameter( string para );

private:


};