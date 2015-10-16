#include "HttpClient.h"

#include <iostream>
#include <curl/curl.h>
#include <CpuInformation.h>
#include <DataManager.h>
#include <UncordObject.h>

#ifndef Q_MOC_RUN
#include <boost/network/protocol/http/client.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#endif

using namespace std;

UNCORD_NAME_SPACE_BEGIN







// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/**********************************************************
	constructor
***********************************************************/
HttpClient::HttpClient()
	:	NetClientBase()
{
	ip			=	FDR_SERVER_IP;
}




/**********************************************************
	destructor
***********************************************************/
HttpClient::~HttpClient()
{}



/**********************************************************
	get_token
***********************************************************/
void	HttpClient::get_token()
{
	char		buf[MAX_BUFFER]	=	"";
	CURL		*curl;
	CURLcode	res;
   
	// init curl
	curl	=	curl_easy_init();

	// set URL   
	sprintf( buf, "http://%s:%s/get_token", ip.c_str(), PORT );
	curl_easy_setopt(curl, CURLOPT_URL, buf );

	struct	curl_httppost	*formpost	=	NULL;  
	struct	curl_httppost	*lastptr	=	NULL;  

	//string	UDID	=	cpu_get_vender() + cpu_get_serial();
	char	identity[200];
	sprintf( identity, "%s%s" , cpu_get_vender().c_str(), cpu_get_serial().c_str() );

	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "Identity",   CURLFORM_COPYCONTENTS, identity, CURLFORM_END );
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "Type",   CURLFORM_COPYCONTENTS, "PC", CURLFORM_END );

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost); 

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &get_token_callback );
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout );

	res	=	curl_easy_perform(curl);
	
	if( res != CURLE_OK )
	{
		//BOOST_ASSERT(false);
		LOG_MESSAGE( 6, "get_token fail." );
	}

	curl_easy_cleanup(curl);
}





/**********************************************************
	create_user
***********************************************************/
string	HttpClient::create_user( string username, string password )
{
	char		buf[MAX_BUFFER]	=	"";
	CURL		*curl;
	CURLcode	res;
	string		email	=	username + string("@tutk.com");
	string		response;		// 留意生命週期問題  假如在callback的時候此物件已經消失,會產生bug.

	// init curl
	curl	=	curl_easy_init();

	// set URL   
	sprintf( buf, "http://%s:%s/create_user/", ip.c_str(), PORT );
	curl_easy_setopt(curl, CURLOPT_URL, buf );

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT ); 

	struct	curl_httppost	*formpost	=	NULL;  
	struct	curl_httppost	*lastptr	=	NULL;  

	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "email",		CURLFORM_COPYCONTENTS, email.c_str(), CURLFORM_END );
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "password",	CURLFORM_COPYCONTENTS, password.c_str(), CURLFORM_END );
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "username",	CURLFORM_COPYCONTENTS, username.c_str(), CURLFORM_END );
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "displayname", CURLFORM_COPYCONTENTS, username.c_str(), CURLFORM_END );

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost); 

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &create_user_callback );
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response );

	res	=	curl_easy_perform(curl);

	if( res == CURLE_OPERATION_TIMEDOUT )
		response	=	"{ \"status\":\"time out\" }";
	else if( res != CURLE_OK )
		LOG_MESSAGE( 6, "fail." );

	curl_easy_cleanup(curl);

	return	response;
}



/**********************************************************
	add_device
***********************************************************/
string	HttpClient::add_device( string auth_token, string uid )
{
	char		buf[MAX_BUFFER]	=	"";
	CURL		*curl;
	CURLcode	res;
	string		cpu_id			=	cpu_get_vender() + cpu_get_serial();
	string		device_name		=	get_machine_name();
	string		response;

	// init curl
	curl	=	curl_easy_init();

	// set URL   
	sprintf( buf, "http://%s:%s/my/device/", ip.c_str(), PORT );
	curl_easy_setopt(curl, CURLOPT_URL, buf );
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT );

	struct	curl_httppost	*formpost	=	NULL;  
	struct	curl_httppost	*lastptr	=	NULL;  

	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "auth_token",		CURLFORM_COPYCONTENTS, auth_token.c_str(),	CURLFORM_END );
#ifdef WIN32
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "device_platform",	CURLFORM_COPYCONTENTS, "win",				CURLFORM_END );
#endif
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "device_serial",	CURLFORM_COPYCONTENTS, cpu_id.c_str(),		CURLFORM_END );
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "device_uid",		CURLFORM_COPYCONTENTS, uid.c_str(),			CURLFORM_END );
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "device_name",		CURLFORM_COPYCONTENTS, device_name.c_str(), CURLFORM_END );


	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost); 

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &add_device_callback );
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response );

	res	=	curl_easy_perform(curl);
	
	if( res == CURLE_OPERATION_TIMEDOUT )
		response	=	"{ \"status\":\"time out\" }";
	else if( res != CURLE_OK )
		LOG_MESSAGE( 6, "create_user fail." );

	curl_easy_cleanup(curl);

	return	response;
}




/**********************************************************
	remove_symbol
	Quokka回傳的json多了一些斜線等等的符號
	移除
***********************************************************/
void		HttpClient::remove_symbol( string& str )
{
	// 回傳的資料會多 \\ 跟頭尾的 \"  目前先手動移除.
	std::string::iterator	itr;
	for( itr = str.begin(); itr != str.end(); ++itr )
	{
		if( *itr == '\\' )
			str.erase(itr);
	}
	str.erase(str.begin());
	str.erase(str.length()-1);
}


/**********************************************************
	login
***********************************************************/
string	HttpClient::login( string username, string password )
{
	char		buf[MAX_BUFFER]	=	"";
	CURL		*curl;
	CURLcode	res;
	string		email	=	username + string("@tutk.com");
	string		response;

	//
	curl	=	curl_easy_init();

	// set URL   
	sprintf( buf, "http://%s:%s/login/", ip.c_str(), PORT );
	curl_easy_setopt(curl, CURLOPT_URL, buf );
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT );

	struct	curl_httppost	*formpost	=	NULL;  
	struct	curl_httppost	*lastptr	=	NULL;  

	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "email",		CURLFORM_COPYCONTENTS, email.c_str(),		CURLFORM_END );
	curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "password",	CURLFORM_COPYCONTENTS, password.c_str(),	CURLFORM_END );

	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost); 

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &login_callback );
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response );

	res	=	curl_easy_perform(curl);
	
	if( res == CURLE_OPERATION_TIMEDOUT )
		response	=	"{ \"status\":\"time out\" }";
	else if( res != CURLE_OK )
		LOG_MESSAGE( 6, "fail." );

	curl_easy_cleanup(curl);

	return	response;
}




/**********************************************************
	parse_json
***********************************************************/
map<string,string>		HttpClient::parse_json( string json )
{
	map<string,string>		result_map;
	
	boost::property_tree::ptree		pt;
	stringstream					ss; 
	
	ss << json;
	boost::property_tree::read_json( ss, pt );

	boost::property_tree::ptree::iterator	itr;
	for( itr = pt.begin(); itr != pt.end(); ++itr )
		result_map[ itr->first ]	=	itr->second.get_value<string>();

	return	result_map;
}




/**********************************************************
	get_uid

	http://192.168.0.17:8080/get_uid?token=722ae861-2261-40f8-960c-95391358e13b

***********************************************************/
string 	HttpClient::get_uid( string auth_token )
{
	char		buf[MAX_BUFFER]	=	"";
	CURL		*curl;
	CURLcode	res;
	string		response;
   
	// init curl
	curl	=	curl_easy_init();

	// set URL   
	sprintf( buf, "http://%s:%s/get_uid/?auth_token=%s", ip.c_str(), PORT, auth_token.c_str() );
	curl_easy_setopt(curl, CURLOPT_URL, buf );
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT );

	curl_easy_setopt(curl, CURLOPT_HTTPGET, true ); 

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &get_uid_callback );
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response );

	res	=	curl_easy_perform(curl);
	
	if( res == CURLE_OPERATION_TIMEDOUT )
		response	=	"{ \"status\":\"time out\" }";
	else if( res != CURLE_OK )
		LOG_MESSAGE( 6, "get_token fail." );

	curl_easy_cleanup(curl);

	return	response;
}






/**********************************************************
	login_callback
	buffer: 回傳資料
	user_p: 使用者自定寫入位置 (file等等)
***********************************************************/
size_t		HttpClient::login_callback( void *buffer, size_t size, size_t nmemb, void *user_p )
{
	LOG_MESSAGE( 3, buffer );

	string	*result	=	static_cast<string*>(user_p);
	result->assign( (char*)buffer );

	return	result->length();
}



/**********************************************************
	add_device_callback
	buffer: 回傳資料
	user_p: 使用者自定寫入位置 (file等等)
***********************************************************/
size_t		HttpClient::add_device_callback( void *buffer, size_t size, size_t nmemb, void *user_p )
{
	LOG_MESSAGE( 3, buffer );

	string	*result	=	static_cast<string*>(user_p);
	result->assign( (char*)buffer );

	return	result->length();
}





/**********************************************************
	create_user_callback
	buffer: 回傳資料
	user_p: 使用者自定寫入位置 (file等等)
***********************************************************/
size_t		HttpClient::create_user_callback( void *buffer, size_t size, size_t nmemb, void *user_p )
{
	LOG_MESSAGE( 3, buffer );

	string	*result	=	static_cast<string*>(user_p);
	result->assign( (char*)buffer );

	return	result->length();
}





/**********************************************************
	buffer: 回傳資料
	user_p: 使用者自定寫入位置 (file等等)
***********************************************************/
size_t		HttpClient::get_token_callback( void *buffer, size_t size, size_t nmemb, void *user_p )
{
	BOOST_ASSERT(false);
	LOG_MESSAGE( 3, buffer );

	DataManager*	p_data_manager	=	static_cast<DataManager*>( get_instance(DATA_MANAGER) );

	string	old_token	=	p_data_manager->get_token();
	string	new_token	=	string( (char*)buffer );

	if( old_token.length() > 1 && old_token != new_token ) {
		BOOST_ASSERT(false);		// token error.
		LOG_MESSAGE( 6, "token error" );
	} else {
		p_data_manager->set_token( string((char*)buffer) );
	}

	return	new_token.length();
}


/**********************************************************
	get_uid_callback
***********************************************************/
size_t		HttpClient::get_uid_callback( void *buffer, size_t size, size_t nmemb, void *user_p )
{
	LOG_MESSAGE( 3, buffer );

	string	*result	=	static_cast<string*>(user_p);
	result->assign( (char*)buffer );

	return	result->size();
}










UNCORD_NAME_SPACE_END
