#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/network/protocol/http/server.hpp>
#include <iostream>
#include <cstdlib>

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

		char tmp[100];
		//request >> tmp;
		//std::cout << request.headers[0];

		/*std::cout << "wait for";
		for( int i = 0; i < 100; i++ )
		{
			Sleep(100);
			std::cout << ".";
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
    
    try {
        /*<< Creates the request handler. >>*/
        hello_world handler;
        /*<< Creates the server. >>*/
        server::options options(handler);
        server server_(options.address("127.0.0.1").port("8000"));
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