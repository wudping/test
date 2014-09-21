#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <boost/network/protocol/http/client.hpp>
#include <iostream>

using namespace boost::network;
using namespace boost::network::http;
//namespace http = boost::network::http;


int
f_client() {
    
    try {
		int		a;
		std::cout << "input > ";
		std::cin >> a;

        /*<< Creates the client. >>*/
        boost::network::http::client client;
        /*<< Creates a request using a URI supplied on the command
             line. >>*/
        boost::network::http::client::request request("http://127.0.0.1:8000");

		char tmp[100];
		sprintf( tmp, "%d", a );
		request << header( tmp, "close");

        /*<< Gets a response from the HTTP server. >>*/
        boost::network::http::client::response response = client.get(request);
        /*<< Prints the response body to the console. >>*/
        std::cout << body(response) << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
//]


#endif