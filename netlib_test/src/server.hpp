#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/thread.hpp>
#include <boost/network/protocol/http/server.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>


namespace http = boost::network::http;

/*<< Defines the server. >>*/
struct hello_world;
typedef http::server<hello_world> server;

/*<< Defines the request handler.  It's a class that defines two
     functions, `operator()` and `log()` >>*/
struct hello_world {
    /*<< This is the function that handles the incoming request. >>*/
    void operator() (server::request const &request,
                     server::response &response) {

		const int	buf_size	=	512;

		char	buf[buf_size];
		char	file_name_c[1000],	path_c[1000];
		string	full_filename,	file_name,	extension,	path,	full_path;
		string	total_size;
		string	str;

		// get filename, path, extension. full_path
		get_file_name_detail( para, full_filename, file_name, path, full_path );
		seperate_filename_extension( full_filename, file_name, extension );

		// open file
		ifstream	infile( full_path.c_str(), std::ios::in | std::ios::binary );
		if ( !infile )
		{
			response	=	server_type::response::stock_reply( server_type::response::not_found );
			return;
		}

		// get file lengh   ÀÉ®×ªø«×  or   file_size.
		int		file_size	=	get_file_size( infile );

		// read file
		int		begin	=	0,	end	=	0;
		read_stream_file( response, infile, range, begin, end, file_size );

		// set headers.
		response.headers.resize(6);

		response.headers[0].name	=	"Content-Length";
		response.headers[0].value	=	int_to_string( response.content.size() );		//boost::lexical_cast<std::string>(response.content.size());

		response.headers[1].name	=	"Content-Type";
		response.headers[1].value	=	http::server::mime_types::extension_to_type( extension );

		response.headers[2].name	=	"Content-Disposition";
		response.headers[2].value	=	string("inline; filename=") + full_filename;

		response.headers[3].name	=	"Accept-Ranges";
		response.headers[3].value	=	"bytes";

		response.headers[4].name	=	"Content-Range";
		response.headers[4].value	=	string("bytes ") + int_to_string(begin) + string("-") + int_to_string(end) + string("/") + int_to_string(file_size);

		response.headers[5].name	=	string("Content-Length");
		response.headers[5].value	=	int_to_string( file_size );


    }
    /*<< It's necessary to define a log function, but it's ignored in
         this example. >>*/
    void log(...) {
        // do nothing
		std::cout << "server log...\n";
    }
};


int f_server() {

	// http://cpp-netlib.org/0.11.0/reference/http_server.html?highlight=thread  see for multi thread
	//boost::thread t1(boost::bind(&http_server::run, &server));
	//boost::thread t2(boost::bind(&http_server::run, &server));
	//server.run();
	//t1.join();
	//t2.join();
	const int THREAD_MAX = 5;
	boost::thread	*thr[THREAD_MAX];
   
    try {


        /*<< Creates the request handler. >>*/
        hello_world handler;
        /*<< Creates the server. >>*/
        server::options options(handler);
        server server_(options.address("192.168.1.74").port("8000"));

		// create threads.
		//boost::thread	*thr[THREAD_MAX];
		for( int i = 0; i < THREAD_MAX; i++ )
			thr[i]	=	new boost::thread( boost::bind( &server::run, &server_) );

        /*<< Runs the server. >>*/
        server_.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
//]


#endif