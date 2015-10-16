#ifndef THREAD_H
#define THREAD_H

#include "UncordBase.h"
#include <QtCore/QThread>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	ThreadManager
***********************************************************/
class Thread : public QThread
{
public:
	Thread();
	~Thread();

	virtual void	run();

	SET_DATA_MACRO( int, cmd )

private:
	int		cmd;

	// functions
	void		net_http_get_token();
	void		net_http_get_uid(); 

	string		net_http_login();
	bool		net_http_add_device( string auth_token, string uid );

};





#endif