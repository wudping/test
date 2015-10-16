#include <ThreadManager.h>


#ifndef Q_MOC_RUN
#include <boost/thread.hpp>
#endif





/**********************************************************
	constructor
***********************************************************/
ThreadManager::ThreadManager()
{}


/**********************************************************
	destructor
***********************************************************/
ThreadManager::~ThreadManager()
{}





/**********************************************************
	set_thr_cmd
***********************************************************/
void	ThreadManager::start( int cmd )
{
	if( !thr.isRunning() )
	{
		thr.set_cmd( cmd );
		thr.start();
	}
	else
	{
		LOG_MESSAGE( 4, "thread has been running." );
	}
}



