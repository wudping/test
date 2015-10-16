#ifndef NETWORK_SPEED_H
#define NETWORK_SPEED_H

#include "NetBase.h"


using namespace std;



UNCORD_NAME_SPACE_BEGIN



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef void (*dll_network_funtion_t)( char*, float*, float* );




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	HttpServer
***********************************************************/
class NetWorkSpeed : Uncord::NetBase
{
public:
	NetWorkSpeed();
	~NetWorkSpeed();

	void	start();

	float	get_download_value() { return download_value; }
	float	get_upload_value() { return upload_value; }

private:
	float	download_value;
	float	upload_value;

};






UNCORD_NAME_SPACE_END


#endif