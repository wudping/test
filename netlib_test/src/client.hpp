#include <boost/network/protocol/http/client.hpp>
#include <iostream>


//namespace http = boost::network::http;


int
f_client() {
    
    try {
        /*<< Creates the client. >>*/
        boost::network::http::client client;
        /*<< Creates a request using a URI supplied on the command
             line. >>*/
        boost::network::http::client::request request("127.0.0.1");
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
