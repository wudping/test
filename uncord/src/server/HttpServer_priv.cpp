#define HTTP_SERVER_PRIV_CPP
#include "HttpServer_priv.h"



#include <cstring>
#include <fstream>
#include <sstream>

#include <mime_type.hpp>
#include "DataManager.h"
#include "UncordObject.h"


/**********************************************************
	constructor
***********************************************************/
HttpServerPriv::HttpServerPriv()
{}





/**********************************************************
	destructor
***********************************************************/
HttpServerPriv::~HttpServerPriv()
{}







/**********************************************************
	trans_cmd
***********************************************************/
int		HttpServerPriv::trans_get_cmd( string cmd )
{
	if( cmd == "validate_device" )
		return	HTTP_VALIDATE_DEVICE;
	else
		return	HTTP_SERVER_ERR_CMD;
}





/**********************************************************
	trans_method
***********************************************************/
int		HttpServerPriv::trans_method( string method )
{
	if( method == "GET" )
		return	HTTP_GET;
	else
		return	HTTP_SERVER_ERR_METHOD;
}





/**********************************************************
	parse_destination
***********************************************************/
map<string,string>	HttpServerPriv::parse_destination( string dest )
{
	char	cmd_c[100],		para_c[100];
	sscanf( dest.c_str() , "/%[^?]?%[^/ \\ \n]", cmd_c, para_c );

	map<string,string>	result_map;

	result_map["cmd"]	=	cmd_c;
	result_map["para"]	=	para_c;

	return	result_map;
}



/**********************************************************
	handle_get
***********************************************************/
int		HttpServerPriv::handle_get( http_server_type::request const &request, 
									http_server_type::response &response )
{
	map<string,string>	dest_map	=	parse_destination( request.destination );
	map<string,string>::iterator	cmd_itr,	para_itr;

	cmd_itr		=	dest_map.find("cmd");
	para_itr	=	dest_map.find("para");

	int		res		=	HTTP_SERVER_DEFAULT;

	if( cmd_itr == dest_map.end() || para_itr == dest_map.end() )
		return	HTTP_SERVER_ERR_PARAMETER;

	switch( trans_get_cmd(cmd_itr->second) )
	{
		case HTTP_VALIDATE_DEVICE:
			res		=	validate_device( request, response, para_itr->second );
			break;
		default:
			BOOST_ASSERT(false);
	}

	return	res;
}





/**********************************************************
	handle_request
***********************************************************/
map<string,string>	HttpServerPriv::parse_parameter( string para )
{
	map<string,string>	result_map;

	//if( para.back() == '/' || para.back() == '\\' )	// 理論上在之前的parse就已經把最後面的斜線拿掉了.
		//para.pop_back();

	int		pos;
	char	key_c[1000],	value_c[1000];
	string	sub_str;

	
	//
	while( !para.empty() )
	{
		pos		=	para.find_last_of('&');

		if( pos == string::npos )
		{
			sub_str		=	para;
			para.erase( 0, para.length() );
		}
		else
		{
			sub_str		=	para.substr( pos+1, string::npos );
			para.erase( pos, para.length() );
		}

		sscanf( sub_str.c_str(), "%[^=]=%[^\n]", key_c, value_c );
		result_map[ key_c ]		=	value_c;	
	}

	return	result_map;
}





/**********************************************************
	handle_request
***********************************************************/
int		HttpServerPriv::handle_request( http_server_type::request const& request, 
										http_server_type::response & response )
{
	LOG_MESSAGE( 3, "handle request" );

	string	method	=	request.method;
	string	dest	=	request.destination;	

	vector<string>	range;

	int		res	=	HTTP_SERVER_DEFAULT;

	switch( trans_method(method) )
	{
		case HTTP_GET:
			res		=	handle_get( request, response );
			break;
		default:
			BOOST_ASSERT(false);
	}

	if( res <= 0 )
		LOG_MESSAGE( 6, "res error" );

	return	res;
}



/**********************************************************
	validate_device
***********************************************************/
int 	HttpServerPriv::validate_device( http_server_type::request const &request, 
										 http_server_type::response &response, 
										 string para )
{
	string	accept_value;
	// 檢查header.
	for ( http_server_type::request::vector_type::iterator itr = request.headers.begin(); itr != request.headers.end(); ++itr ) 
	{
		if( itr->name == "Accept" )
		{
			accept_value	=	itr->value;
			break;
		}
	}
	if( accept_value != "application/uncord.v1" )
	{
		response	=	http_server_type::response::stock_reply( http_server_type::response::unauthorized, STR_MACRO(HTTP_SERVER_ERR_HEADER) );
		return	HTTP_SERVER_ERR_HEADER;
	}

	// 轉換參數
	map<string, string>		para_map	=	parse_parameter( para );
	map<string,string>::iterator	device_uuid_itr		=	para_map.find("device_uuid"),
									device_name_itr		=	para_map.find("device_name"),
									password_itr		=	para_map.find("password");
	if( device_uuid_itr == para_map.end() || device_name_itr == para_map.end() || password_itr == para_map.end() )
		return	HTTP_SERVER_ERR_PARAMETER;

	// 追加到device列表
	Uncord::DataManager*	p_data_manager	=	static_cast<Uncord::DataManager*>( Uncord::get_instance(Uncord::DATA_MANAGER) );

	// 檢查密碼
	if( !p_data_manager->check_QR_pw(password_itr->second) )
	{		
		response	=	http_server_type::response::stock_reply( http_server_type::response::unauthorized, STR_MACRO(HTTP_SERVER_ERR_PARAMETER) );
		return	HTTP_SERVER_ERR_PARAMETER;
	}

	// 加入裝置
	p_data_manager->add_device_info( device_uuid_itr->second, device_name_itr->second, password_itr->second );
	response	=	http_server_type::response::stock_reply( http_server_type::response::created, STR_MACRO(HTTP_SERVER_OK) );

	// 通知主畫面更新
	Uncord::UncordObject*	p_uncord_obj	=	static_cast<Uncord::UncordObject*>( Uncord::get_instance(Uncord::UNCORD_OBJ) );
	p_uncord_obj->refresh_device_list();

	return	HTTP_SERVER_OK;
}


