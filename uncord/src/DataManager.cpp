#include "DataManager.h"


#ifndef Q_MOC_RUN
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/network/protocol/http/client.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#endif

#include "client/HttpClient.h"

#include <ctime>
#include <fstream>
#include <iostream>


using namespace std;




UNCORD_NAME_SPACE_BEGIN






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
DataManager::DataManager()
	: UncordBase()
{
	boost::random::mt19937::result_type random_seed = static_cast<boost::random::mt19937::result_type>(time(0));
	//rd_seed.seed(random_seed);
	rd_seed.seed(static_cast<boost::random::mt19937::result_type>(random_seed));
}




/**********************************************************
	destructor
***********************************************************/
DataManager::~DataManager()
{}



/**********************************************************
	generate_QR_pw
***********************************************************/
void	DataManager::set_device_name( int index, string name )
{
	if( index >= device_info.size() )
	{
		BOOST_ASSERT(false);
		return;
	}

	device_info[index].device_name	=	name;
}



/**********************************************************
	generate_QR_pw
***********************************************************/
string		DataManager::generate_QR_pw()
{          
	boost::random::uniform_int_distribution<>	rd_gen( 0, 35 );
	int		i,	r;

	QR_pw.resize(6);

	for( i = 0; i < 6; i++ )
	{
		r	=	rd_gen(rd_seed);

		if( r >= 0 && r <= 9 )
			r	=	r + '0';
		else
			r	=	r - 10 + 'a';

		QR_pw[i]	=	static_cast<char>(r);
	}

	return	QR_pw;
}


/**********************************************************
	check_QR_pw
***********************************************************/
bool	DataManager::check_QR_pw( string pw )
{
	return	QR_pw == pw;
}


/**********************************************************
	set_temp_data
***********************************************************/
/*void	DataManager::set_temp_data( string key, string value )
{
	temp_map[key]	=	value;
}*/


/**********************************************************
	get_temp_data
***********************************************************/
/*string	DataManager::get_temp_data( string key )
{
	map<string,string>::iterator	itr		=	temp_map.find(key);

	if( itr == temp_map.end() )
		return	string();
	else
		return	itr->second;
}*/


/**********************************************************
	add_device_info
***********************************************************/
void	DataManager::add_device_info( string device_uuid, string device_name, string password )
{
	DeviceInfo	di;		// device_info is class member.
	di.device_uuid		=	device_uuid;
	di.device_name		=	device_name;
	di.password		=	password;

	device_info.push_back(di);
}



/**********************************************************
	init_account
***********************************************************/
void	DataManager::generate_account()
{
	boost::random::uniform_int_distribution<>	rd_gen( 0, 35 );
	
	char	username_c[15]	=	"guest",		// ´ú¸Õª©¥»
			password_c[15]	=	{0};	
	
	int		i,	r;
	
	for( i = 5; i < 12; i++ )
	{
		r	=	rd_gen(rd_seed);
		if( r >= 0 && r <= 9 )
			r	=	r + '0';
		else
			r	=	r - 10 + 'a';
	
		username_c[i]	=	(char)r;
	}
	
	for( i = 0; i < 12; i++ )
	{
		r	=	rd_gen(rd_seed);
		if( r >= 0 && r <= 9 )
			r	=	r + '0';
		else
			r	=	r - 10 + 'a';
	
		password_c[i]	=	(char)r;
	}
	
	username	=	username_c;
	password	=	password_c;	
}





/**********************************************************
	parse_form_data
***********************************************************/
string 	DataManager::parse_form_data( string name, string data )
{
	//7MA4YWxkTrZu0gW
	//---------------

	stringstream	formParams;
	formParams	<< "----WebKitFormBoundaryp---------------" << "\n"
				<< "Content-Disposition: form-data; name=\"" << name << "\"\n\n" << data << "\n";

	return	formParams.str();
}



/**********************************************************
	init_account
***********************************************************/
void	DataManager::init_account()
{
	ifstream	infile( "account.txt" );

	if( infile.is_open() )
	{
		LOG_MESSAGE( 3, "load from data");

		infile >> username >> password ;
		infile.close();
	}
	else
	{
		LOG_MESSAGE( 3, "send create user");
		string		resopnse;
		string		status	=	"";
		HttpClient	http_client;

		do{
			generate_account();

			resopnse	=	http_client.create_user( username, password );
			//
			boost::property_tree::ptree		pt;
			stringstream	ss; 
			ss << resopnse;
			boost::property_tree::read_json(ss, pt);

			status	=	pt.get<std::string>("status");

		} while( status != "OK" );

		ofstream	outfile("account.txt");
		outfile << username << "\n" << password;
		outfile.close();		
	}
}


/**********************************************************
	get_device_info
***********************************************************/
deque<DeviceInfo>	DataManager::get_device_info()
{
	return	device_info;
}




UNCORD_NAME_SPACE_END
