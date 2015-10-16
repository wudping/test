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

	// �{�ɨϥΪ�function ����|����
	void	init_account();
	void	generate_account();
	string 	parse_form_data( string name, string data );

	//void	set_temp_data( string key, string value );
	//string	get_temp_data( string key );

private:
	string	UID;
	string	token;
	string	QR_pw;		// �X�{�b���yQR code������ password.

	deque<DeviceInfo>			device_info;

	boost::random::mt19937		rd_seed;    
	//boost::random::uniform_int_distribution<>	*rd_gen( 0, 35 );
	
	// �{�ɨϥΪ��ܼ� ��������p����
	string	username;
	string	password;
	string	auth_token;

	//map<string,string>	temp_map;		// �Ȧs��ƪ�map. �D�n�Olibcurl�n��callback���覡�����^�Ǹ��,��K���^�Dthread.
};



UNCORD_NAME_SPACE_END


#endif