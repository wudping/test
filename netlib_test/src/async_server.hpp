#pragma once

#include <boost/thread.hpp>
#include <boost/network/protocol/http/server.hpp>

#include <fstream>
#include <stdio.h>
using namespace std;

struct handler_type;
typedef boost::network::http::async_server<handler_type> http_server;

struct handler_type {
    void operator()(
        http_server::request const & request,
        http_server::connection_ptr connection
    ) {
        // do something here

		//connection->write("Hello, world!\n");
		//std::string sample = "I have a string!";
		//connection->write(sample);

		int		i;

		string	range;

		for( i = 0; i < request.headers.size(); i++ )
		{
			if( request.headers[i].name == "Range" )
			{
				range = request.headers[i].value ;
				break;			}
		}
		int		begin;
		sscanf( range.c_str(), "bytes=%d-" , &begin );	
		printf("%d\n", begin);

		ifstream	infile( "D:\\Kanon.mpg", ios::binary );


		int		filesize;

		infile.seekg ( 0, infile.end );
		filesize	=	infile.tellg();
		infile.seekg( begin, infile.beg );




		char	buf[5120];
		while( infile.read( buf, sizeof(buf) ).gcount() > 0 )
		//infile.read( buf, sizeof(buf) );
			connection->write(buf);
			//response.content.append( buf, infile.gcount() );

		server::response_header headers[4];// = { {"Connection","close"} ,{"Content-Type", "text/plain"} };

		headers[0].name	=	"Content-Length";
		headers[0].value	=	boost::lexical_cast<string>(filesize);   	//boost::lexical_cast<std::string>(response.content.size());

		//headers[1].name	=	"Content-Type";
		//headers[1].value	=	http::http_server::mime_types::extension_to_type( "mpg" );

		headers[1].name	=	"Content-Disposition";
		headers[1].value	=	string("inline; filename=") + "Kanon.mpg";

		headers[2].name	=	"Accept-Ranges";
		headers[2].value	=	"bytes";

		//return	boost::lexical_cast<string>(input);

		headers[3].name	=	"Content-Range";
		headers[3].value	=	string("bytes ") +  boost::lexical_cast<string>(begin)  + string("-") +  boost::lexical_cast<string>(filesize) + string("/") +   boost::lexical_cast<string>(filesize); 



		connection->set_headers(boost::make_iterator_range(headers, headers + 4));
		connection->set_status( http_server::connection::ok );
 

    }
};




void	async_test()
{

	handler_type handler;
	http_server::options options(handler);
	boost::network::utils::thread_pool thread_pool(2);
	http_server server(options.address("192.168.1.2").port("8000"));

	boost::thread t1(boost::bind(&http_server::run, &server));
	boost::thread t2(boost::bind(&http_server::run, &server));
	boost::thread t3(boost::bind(&http_server::run, &server));
	boost::thread t4(boost::bind(&http_server::run, &server));
	boost::thread t5(boost::bind(&http_server::run, &server));
	boost::thread t6(boost::bind(&http_server::run, &server));
	boost::thread t7(boost::bind(&http_server::run, &server));
	boost::thread t8(boost::bind(&http_server::run, &server));

	server.run();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
	t8.join();

}