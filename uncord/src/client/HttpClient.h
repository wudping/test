/*
	note: libcurl�|�bvisual studio 2012���U�o��crash
	��]����,���G�O���bug....
	���n�bdebug mode�U���`
	��T�ݰ_�Ӹ�ssl����...
*/

#ifndef FDR_NET_HTTP_H
#define FDR_NET_HTTP_H

#include "NetClientBase.h"
#include <map>


using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define FDR_SERVER_IP	"cmpdemo2.tutk.com"
#define PORT			"9521"
#define TIMEOUT			60



UNCORD_NAME_SPACE_BEGIN



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	HttpClient
***********************************************************/
class	HttpClient : public NetClientBase
{
public:
	HttpClient();
	virtual ~HttpClient();

	void	get_token();
	string	get_uid( string auth_token );

	string	create_user( string username, string password );
	string	login( string username, string password );
	string	add_device( string auth_token, string uid );

	//size_t	get_token_callback( void *buffer, size_t size, size_t nmemb, void *user_p );

	static	map<string,string>		parse_json( string json );

private:
	void		remove_symbol( string& str );

	static size_t	get_token_callback( void *buffer, size_t size, size_t nmemb, void *user_p );
	static size_t	get_uid_callback( void *buffer, size_t size, size_t nmemb, void *user_p );
	static size_t	create_user_callback( void *buffer, size_t size, size_t nmemb, void *user_p );
	static size_t	login_callback( void *buffer, size_t size, size_t nmemb, void *user_p );
	static size_t	add_device_callback( void *buffer, size_t size, size_t nmemb, void *user_p );

	string		ip;
};


UNCORD_NAME_SPACE_END




#endif