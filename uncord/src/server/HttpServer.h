#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H




#include "server/NetServerBase.h"
#include <string>



using namespace std;




UNCORD_NAME_SPACE_BEGIN





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	HttpServer
***********************************************************/
class HttpServer : public NetServerBase
{
public:
	HttpServer();
	virtual ~HttpServer();

	//
	void	init();
	void	start();

private:

	const static int	THREAD_MAX	=	10;

};



UNCORD_NAME_SPACE_END


#endif