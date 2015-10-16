#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H


#include <Thread.h>


#ifndef Q_MOC_RUN
#include <boost/thread.hpp>
#endif


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	ThreadManager
***********************************************************/
class ThreadManager
{
public:
	ThreadManager();
	virtual ~ThreadManager();

	void	start( int cmd );
	
	template<typename Mission>
	void	add_thread( Mission mission, bool is_join );

private:
	Thread	thr;

};




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ template class function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	set_thr_cmd
***********************************************************/
template<typename Mission>
void	ThreadManager::add_thread( Mission mission, bool is_join = false )
{
	LOG_MESSAGE( 3, "add thread" );

	boost::thread	thr( mission );

	if( is_join )
		thr.join();
}





#endif