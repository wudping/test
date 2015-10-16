// see https://code.google.com/p/cpp-netlib/source/browse/libs/network/example/http/fileserver.cpp?r=685810c885e6d0c03b39b61c0ac6201632eb323c

#pragma once

#if !(defined(ASYNC_HTTP_SERVER_CPP)|defined(ASYNC_HTTP_SERVER_PRIV_CPP))		// ���F�O��include���ɮת��u��async http server.
#error
#endif

#ifndef Q_MOC_RUN
#include <boost/network/include/http/server.hpp>
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
#endif

// �ϥ�QT���u��Ӱ�parse
#include <QString>
#include <QUrl>

#ifdef UNIX				// note: �ëD��unix only, �ݭn�T�{.
#include <sys/mman.h>
#include "mmap_windows.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <map>
#include <vector>

#include "mime_type.hpp"
#include "UncordBase.h"

using namespace std;

//namespace http = boost::network::http;
//namespace utils = boost::network::utils;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct & typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// �o�䥲����cpp netlib���榡�Ӽg
struct		async_http_handler_struct;
typedef		boost::network::http::async_server<async_http_handler_struct>		file_server;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ASYNC_OK			1
#define ASYNC_DEFAULT		0
#define ASYNC_ERR			-1
#define ASYNC_ERR_METHOD	-2

#define FILE_POS_DEFAULT	-1


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum	ASYNC_METHOD
{
	ASYNC_HTTP_HEAD,
	ASYNC_HTTP_GET,
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t				get_file_size( string filename );
__int64				get_file_size( int fh );

void				seperate_cmd_para( string destination, string& cmd, string& para );
void				seperate_filename_path( string para, string &filename, string &path );
bool				is_file_exist( string full_filename );
vector<string>		get_range( file_server::request const & request );
int					parse_method( string method );
int					handle_request( file_server::request const &request, 
									file_server::connection_ptr connection,
									char *root_path );
int					handle_get( file_server::request const &request, 
								file_server::connection_ptr connection,
								char *root_path );
void				url_decode( string& str );
map<string,string>	parse_parameter( string para );



// --------------------------------------------------------------------------------------------------------------------------
// async_http_handler_struct
struct	async_http_handler_struct 
{
	char	*root_path;		// �o��ĥΦ@�θ�ƪ��覡�Ӧs��.  (�h�����i���k)

	// --------------------------------------------------------------------------------------------------------------------------
	// constructor
	explicit async_http_handler_struct() 
	{
		root_path	=	new char[1000];
		strcpy( root_path, "C:\\" );		// default path.
	}
	// destructor
	~async_http_handler_struct() { delete root_path; root_path = NULL; }

	// --------------------------------------------------------------------------------------------------------------------------
	void operator() (	file_server::request const &request, 
						file_server::connection_ptr connection ) 
	{
		int		res		=	handle_request( request, connection, root_path );
		if( res <= 0 )
			LOG_MESSAGE( 6, "res <= 0" );
	}
};

