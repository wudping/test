/***********************************************************************************************************
	最底層的物件.
************************************************************************************************************/
#ifndef UNCORD_BASE_H
#define UNCORD_BASE_H

#include <string>
#include <stdio.h>

#ifndef Q_MOC_RUN
#include <boost/function.hpp>
#endif

using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define UNCORD_NAME_SPACE_BEGIN		namespace Uncord {
#define UNCORD_NAME_SPACE_END		}

#define MAX_BUFFER 3000

#ifdef _DEBUG
#define LOG_LEVEL	1
#else
#define LOG_LEVEL	1
#endif

#define LOG_MESSAGE( level, msg ) \
	{ \
		if( (level) >= LOG_LEVEL ) \
			printf( "%s:%d %s : %s\n", __FILE__, __LINE__, __FUNCTION__ , (msg) ); \
	}

#define ALERT_MESSAGE( msg ) \
	{ \
		printf( "%s:%d %s : %s\n", __FILE__, __LINE__, __FUNCTION__ , (msg) ); \
		BOOST_ASSERT(false); \
	}
		
#define SAFE_DELETE_PTR( ptr ) \
	delete (ptr); \
	(ptr) = NULL;

#define SAFE_DELETE_PTR_ARRAY( ptr ) \
	delete [] (ptr); \
	(ptr) = NULL;

#define STR_MACRO(str) #str

#define GET_DATA_MACRO( type, name ) \
	type get_##name() { \
		return (name); \
	}

#define SET_DATA_MACRO( type, name ) \
	void set_##name( type a ) { \
		name = (a); \
	}

#define SAFE_DELETE(ptr) \
	{ delete (ptr); ptr = NULL; }





UNCORD_NAME_SPACE_BEGIN


	
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	INSTANCE_TYPE
{   
	// 
	UNCORD_OBJ,
	DATA_MANAGER,
	// widget
	SETTING_WIDGET,
	MAIN_WINDOW,
};




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef		boost::function< int( int cmd, std::string data, std::string &result ) >		boost_cmd_function;
typedef		int cmd_function_type( int, std::string, std::string& );






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	ClientBase
***********************************************************/
class UncordBase
{
public:
	UncordBase();
	virtual ~UncordBase();

private:
	
};





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void		print_build_time();
void*		get_instance( int type );
void		seperate_filename_extension( const string& source, string& filename, string& extension );
string		int_to_string( int input );
string		int_to_string( __int64 input );
void		uncord_sleep( int msec );
string		generate_uuid();

UNCORD_NAME_SPACE_END


#endif