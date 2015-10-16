#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "UncordBase.h"
#include <string>
#include <deque>
#include <map>

#ifndef Q_MOC_RUN
#include <boost/random/mersenne_twister.hpp>
#endif



using namespace std;


UNCORD_NAME_SPACE_BEGIN



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct	DeviceInfo
{
	string	device_uuid;
	string	device_name;
	string	password;
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	ClientBase
***********************************************************/
class DataManager : public UncordBase
{
public:
	DataManager();
	virtual ~DataManager();

	GET_DATA_MACRO( string, UID )
	SET_DATA_MACRO( string, UID )
	GET_DATA_MACRO( string, token )
	SET_DATA_MACRO( string, token )

	GET_DATA_MACRO( string, username )
	GET_DATA_MACRO( string, password )

	SET_DATA_MACRO( string, auth_token )
	GET_DATA_MACRO( string, auth_token )

	//void		add_device_info( string str );
	void		add_device_info( string device_uuid, string device_name, string password );
	string		generate_QR_pw();
	bool		check_QR_pw( string pw );
	void		set_device_name( int index, string name );

	deque<DeviceInfo>	get_device_info();

	// 臨時使用的function 之後會移除
	void	init_account();
	void	generate_account();
	string 	parse_form_data( string name, string data );

	//void	set_temp_data( string key, string value );
	//string	get_temp_data( string key );

private:
	string	UID;
	string	token;
	string	QR_pw;		// 出現在掃描QR code頁面的 password.

	deque<DeviceInfo>			device_info;

	boost::random::mt19937		rd_seed;    
	//boost::random::uniform_int_distribution<>	*rd_gen( 0, 35 );
	
	// 臨時使用的變數 之後視情況移除
	string	username;
	string	password;
	string	auth_token;

	//map<string,string>	temp_map;		// 暫存資料的map. 主要是libcurl要用callback的方式接收回傳資料,方便切回主thread.
};



UNCORD_NAME_SPACE_END


#endif