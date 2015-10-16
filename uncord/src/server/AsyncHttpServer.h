#ifndef ASYNC_HTTP_SERVER_H
#define ASYNC_HTTP_SERVER_H


#include "server/NetServerBase.h"


UNCORD_NAME_SPACE_BEGIN



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum ASYNC_HTTP_GET_CMD
{
	ASYNC_HTTP_GET_FILE,
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	AsyncHttpServer
***********************************************************/
class AsyncHttpServer : public NetServerBase
{
public:
	AsyncHttpServer();
	virtual ~AsyncHttpServer();

	//
	void	init();
	void	start();
	void	set_root( string str );

	char	*root_path;

private:
	const static int	THREAD_MAX	=	20;

};






UNCORD_NAME_SPACE_END


#endif