/***********************************************************************************************************
	此物建定義為會使用QT的套件,作為其他code的銜接之用.
************************************************************************************************************/
#ifndef UNCORD_OBJECT_H
#define UNCORD_OBJECT_H

#include <QObject>

#include "UncordBase.h"


using namespace std;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ outer class declared ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class	ThreadManager;



UNCORD_NAME_SPACE_BEGIN


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	OBJ_TYPE
{
	SFTP_SERVER,
	ASYNC_HTTP_SERVER,
	HTTP_SERVER,
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class declared ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class	NetWorkSpeed;
class	TUTK_P2PTunnel;
class	AsyncHttpServer;		
class	SFtpServer;
class	HttpServer;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	Server
***********************************************************/
class	UncordObject : public QObject
{
	Q_OBJECT

public slots:
    //void setValue(int value);

signals:
    void	get_uid_sig();
	void	refresh_device_list_sig();


public:
	UncordObject();
	virtual ~UncordObject();

	void	init();
	void	get_UID();
	void	inform_get_uid();
	void	set_port( int port, int obj_type );
	void	set_root( string root_path );

	void	set_thread_manager( ThreadManager* p_thr_m ) { p_thread_manager = p_thr_m; }
	void	refresh_device_list();

	// network speed
	float	get_download_value();
	float	get_upload_value();

	// tutk tunnel
	void	start_tutk_tunnel();

private:

	// functions.
	void	init_network_speed();
	void	init_tutk_tunnel();
	void	init_async_http_server();
	void	init_http_server();
	void	init_sftp_server();

	// obj
	//DataManager		data_manager;

	// QT obj
	ThreadManager		*p_thread_manager;

	// Uncord obj
	NetWorkSpeed		*p_network_speed;
	TUTK_P2PTunnel		*p_tutk_tunnel;
	AsyncHttpServer		*p_async_http_server;
	SFtpServer			*p_sftp_server;
	HttpServer			*p_http_server;

	map<string,int>		port_map;


};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
QObject*		get_qt_obj_instance( int type );
void			message_box( string str );
string			get_machine_name();


UNCORD_NAME_SPACE_END




#endif