#include "UncordBase.h"


#include <iostream>
#include <string>
#include <sstream>

// �@�w�n�w�qQ_MOC_RUN  ���M�|�o�ͽsĶ���~ 
#ifndef Q_MOC_RUN
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
// uuid
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/name_generator.hpp>
#endif 


using namespace std;




UNCORD_NAME_SPACE_BEGIN

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ global variable ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// for get instance.
extern "C" {
	class	Uncord;				Uncord		*p_uncord_obj		=	NULL;				
	class	DataManager;		DataManager	*p_data_manager		=	NULL;
}	// end extern "C"




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************
	constructor
***********************************************************/
UncordBase::UncordBase()
{}




/**********************************************************
	destructor
***********************************************************/
UncordBase::~UncordBase()
{}










// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	print_build_time
***********************************************************/
void	print_build_time()
{
	printf( "build in %s %s\n", __DATE__, __TIME__ );
}



/**********************************************************
	get instance  
	�t�d�B�z function�I�s���쪺����
***********************************************************/
void*	get_instance( int type )
{
	void*	ptr		=	NULL;

	switch(type)
	{
		case UNCORD_OBJ :
			ptr		=	(void*)p_uncord_obj;
			break;
		case DATA_MANAGER :
			ptr		=	(void*)p_data_manager;
			break;
		default:
			BOOST_ASSERT(false);
	}

	return	ptr;
}




/**********************************************************
	seperate_filename_extension  
***********************************************************/
void	seperate_filename_extension( const string& source, string& filename, string& extension )
{
	size_t	i	=	source.find_last_of(".");

	if( i == source.npos )		// �L���ɦW
	{
		filename	=	source;
		extension	=	"";
		return;
	}

	filename	=	source.substr( 0, i );
	extension	=	source.substr( i+1, source.length() );
}



/**********************************************************
	int_to_string  
***********************************************************/
string		int_to_string( int input )
{
	/*stringstream	ss;
	ss << input;
	string	result	=	ss.str();	// ���T�w�O�_���ͩR�P�������D  �O�I�_���h��@�h �t�׷|�ܺC 
	return	result;*/
	return	boost::lexical_cast<string>(input);
}



/**********************************************************
	int_to_string  
	x64
***********************************************************/
string		int_to_string( __int64 input )
{
	/*stringstream	ss;
	ss << input;
	string	result	=	ss.str();	// ���T�w�O�_���ͩR�P�������D  �O�I�_���h��@�h �t�׷|�ܺC 
	return	result;*/
	return	boost::lexical_cast<string>(input);
}




/**********************************************************
	uncord_sleep  
***********************************************************/
void		uncord_sleep( int msec )
{
	boost::this_thread::sleep(boost::posix_time::milliseconds(msec));
}




/**********************************************************
	generate_uuid
***********************************************************/
string		generate_uuid()
{
	boost::uuids::uuid	uuid	=	boost::uuids::random_generator()();

	std::stringstream	ss;
	ss << uuid;

	return	ss.str();
}




UNCORD_NAME_SPACE_END
