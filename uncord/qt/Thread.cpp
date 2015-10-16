#include <Thread.h>

#include <client/HttpClient.h>
#include "UncordObject.h"
#include <DataManager.h>

#include <stdio.h>
#include <common.h>
#include <qfile.h>
#include <iostream>

using namespace std;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
Thread::Thread()
{}


/**********************************************************
	destructor
***********************************************************/
Thread::~Thread()
{}


/**********************************************************
	run
	目前測試結果...
	在run結束之前,再次呼叫start也是無效的.
	(以同一個thread來講)
	要留意這邊的控制.
***********************************************************/
void	Thread::run()
{
	LOG_MESSAGE( 3, "start run" );

	switch( cmd )
	{
		case thread::NET_HTTP_GET_UID:
			net_http_get_uid();
			break;
		default:
			BOOST_ASSERT(false);
	}
}



/**********************************************************
	net_http_get_token
***********************************************************/
void	Thread::net_http_get_token()
{
	Uncord::HttpClient	http_client;
	http_client.get_token();
}


/**********************************************************
	net_http_create_user
***********************************************************/
string	Thread::net_http_login()
{
	LOG_MESSAGE( 3, "login" );

	Uncord::DataManager*	p_data_manager	=	static_cast<Uncord::DataManager*>( Uncord::get_instance(Uncord::DATA_MANAGER) );

	string	auth_token;
	
	string	username	=	p_data_manager->get_username();
	string	password	=	p_data_manager->get_password();

	Uncord::HttpClient	http_client;
	// login
	string	response	=	http_client.login( username, password );

	// 取得資料
	map<string,string>	login_json	=	Uncord::HttpClient::parse_json( response );
	
	if( login_json["status"] != "OK" )
	{
		LOG_MESSAGE( 6, "status != ok" );
		auth_token	=	"no token";
	}
	else
		auth_token	=	login_json["auth_token"];

	return	auth_token;
}






/**********************************************************
	net_http_get_uid
***********************************************************/
bool	Thread::net_http_add_device( string auth_token, string uid )
{
	LOG_MESSAGE( 3, "add device" );

	Uncord::DataManager	*p_data_manager		=	static_cast<Uncord::DataManager*>( Uncord::get_instance(Uncord::DATA_MANAGER) );

	Uncord::HttpClient	http_client;
	string	response	=	http_client.add_device( auth_token, uid );

	map<string,string>	result_map	=	Uncord::HttpClient::parse_json( response );

	if( result_map["status"] == "OK" )
		return	true;
	else
		return	false;
}




/**********************************************************
	net_http_get_uid
***********************************************************/
void	Thread::net_http_get_uid()
{
	Uncord::UncordObject*	p_uncord_obj	=	static_cast<Uncord::UncordObject*>( Uncord::get_instance(Uncord::UNCORD_OBJ) );
	Uncord::DataManager*	p_data_manager	=	static_cast<Uncord::DataManager*>( Uncord::get_instance(Uncord::DATA_MANAGER) );
	string	uid;

	// login
	string	auth_token	=	net_http_login();
	p_data_manager->set_auth_token( auth_token );		// write token data.

	// get uid
	Uncord::HttpClient	http_client;
	string response		=	http_client.get_uid( auth_token );

	map<string,string>	result_map	=	Uncord::HttpClient::parse_json( response );
	if( result_map["status"] != "OK" )
	{
		LOG_MESSAGE( 6, "status != ok" );
		uid		=	"no data";
	}
	else
		uid		=	result_map["uid"];

	// add device
	if( net_http_add_device( auth_token, uid ) )		// need check result.
	{
		p_data_manager->set_UID(uid);
		p_uncord_obj->inform_get_uid();
	}
	else
	{
		uid		=	"add device fail.";
		p_data_manager->set_UID(uid);
		p_uncord_obj->inform_get_uid();
		LOG_MESSAGE( 6, "add device fail." );
	}
}

