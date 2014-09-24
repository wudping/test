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
        server::string_type ip = source(request);
        unsigned int port = request.source_port;
        std::ostringstream data;

		//std::vector<request_header_narrow>::iterator header_iterator = headers.begin();

		/*
		std::vector<  boost::network::http::request_header_narrow> hv;
		hv = headers(request);

		std::cout << hv[0].name << std::endl;
		int a = atoi(hv[0].name.c_str());

		if( a == 2 )
		{
			std::cout << "wait for";
			for( int i = 0; i < 100; i++ )
			{
				Sleep(100);
				std::cout << ".";
			}
		}*/


        data << "Hello, " << ip << ':' << port << '!';
        response = server::response::stock_reply(
            server::response::ok, data.str());
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

   
    try {


        /*<< Creates the request handler. >>*/
        hello_world handler;
        /*<< Creates the server. >>*/
        server::options options(handler);
        server server_(options.address("192.168.0.6").port("8000"));

		//boost::thread t1(boost::bind(&server::run, &server_));
		//boost::thread t2(boost::bind(&server::run, &server_));


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