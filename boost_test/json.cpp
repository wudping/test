#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <string>

#define MARK_MACRO \
	std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << "\n\n";




// 
void	boost_json_example_1()
{
	MARK_MACRO

	char str[1000]	=	"{\"id\":\"AX13441\", \"name\":\"John\", \"age\":33, \"nick_name\":\"meiiiii\", \"height\":178.14}";

	std::stringstream	ss(str);
	boost::property_tree::ptree		pt;

	read_json( ss, pt );

	std::cout << "id : " << pt.get<std::string>("id").c_str() << "\n"
			  << "name : " << pt.get<std::string>("name").c_str() << "\n"
			  << "age : " << pt.get<int>("age") << "\n"
			  << "nick_name : " << pt.get<std::string>("nick_name").c_str() << "\n"
			  << "height : " << pt.get<double>("height") << "\n"
			  << "\n\n";

}
















//
void	boost_json_example()
{
	boost_json_example_1();

	std::cout << "\n\n\n";
}