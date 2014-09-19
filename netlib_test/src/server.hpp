#ifndef SERVER_HPP
#define SERVER_HPP


#include <boost/network/protocol/http/server.hpp>
#include <iostream>


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
        std::ostringstream data;
        data << "Hello, " << ip << "!";
        response = server::response::stock_reply(
            server::response::ok, data.str());
    }
    /*<< It's necessary to define a log function, but it's ignored in
         this example. >>*/
    void log(...) {
        // do nothing
    }
};






int f(int argc, char * argv[]) {
    
    /*if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " address port" << std::endl;
        return 1;
    }*/

    try {
        /*<< Creates the request handler. >>*/
        hello_world handler;
        /*<< Creates the server. >>*/
        //server server_(argv[1], argv[2], handler);
        server server_( "0.0.0.0", "8000", handler);
        /*<< Runs the server. >>*/
        server_.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    system("PAUSE");
    return 0;
}

#endif