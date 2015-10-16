#include "NetWorkSpeed.h"


#ifdef USE_QT
#include <QLibrary>
#endif



UNCORD_NAME_SPACE_BEGIN




/**********************************************************
	constructor
***********************************************************/
NetWorkSpeed::NetWorkSpeed()
{
	download_value	=	-1;
	upload_value	=	-1;
}





/**********************************************************
	desturctor
***********************************************************/
NetWorkSpeed::~NetWorkSpeed()
{}



	
/**********************************************************
	start
***********************************************************/
void	NetWorkSpeed::start()
{
	QLibrary	network_speed_dll("NetWrokSpeedApi");

	dll_network_funtion_t	network_speed_start	=	(dll_network_funtion_t)network_speed_dll.resolve("network_speed_start");

	if(network_speed_start)
		network_speed_start( "Uncord", &download_value, &upload_value );
	else
		BOOST_ASSERT(false);


}


	
UNCORD_NAME_SPACE_END
