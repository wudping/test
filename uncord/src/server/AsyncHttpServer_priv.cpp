#define ASYNC_HTTP_SERVER_PRIV_CPP
#include "AsyncHttpServer_priv.h"


#include <string>
#include <fstream>
#include <stdio.h>

#ifdef WIN32		// 不確定是否全部都是windows only.
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif


//#include <qDebug>

using namespace std;









// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 實際上處理檔案傳輸的部分.
struct connection_handler : boost::enable_shared_from_this<connection_handler> 
{
	// --------------------------------------------------------------------------------------------------------------------------
	// constructor
	explicit connection_handler() {}

	// --------------------------------------------------------------------------------------------------------------------------
	// 配合cpp netlib的設計  先這樣寫  (留意boost::enable_shared_from_this<connection_handler>)  
	// 之前修改的時候發生crash
	void operator() (	std::string const &destination, 
						file_server::connection_ptr connection, 
						__int64 begin, __int64 end, char *root_path ) 
	{
		string		para,	cmd;

		seperate_cmd_para( destination, cmd, para );

		// 目前此server只提供get_file	
		if( cmd != "get_file" )	
		{
			connection->set_status(file_server::connection::bad_request);
			connection->write("bad request");
			return;
		}

		// decode url
		url_decode( para );
			
		// parse parameter
		map<string,string>	para_map	=	parse_parameter( para );
		map<string,string>::iterator	file_path_itr	=	para_map.find("file_path"),
										file_name_itr	=	para_map.find("file_name");
		if( file_path_itr == para_map.end() || file_name_itr == para_map.end() )
		{
			connection->set_status(file_server::connection::bad_request);
			connection->write("bad request");
			return;
		}

		string	filename	=	file_name_itr->second,	
				path		=	file_path_itr->second;

		// 測試轉中文
		QUrl orig( filename.c_str() );
		QUrl orig2( path.c_str() );
		QByteArray data, data2;
		data.append(orig.toString());
		data2.append( orig2.toString() );
		QUrl url = QUrl::fromEncoded(data);
		QUrl url2 = QUrl::fromEncoded(data2);
		QString qfilename	=	url.fileName();
		QString qpath = url2.path();
		//qDebug() << url; 

		// 測試轉編碼  以後需要多測試跟修改.
		string pre_root_tmp(root_path);		
		wstring	pre_root(pre_root_tmp.begin(), pre_root_tmp.end());		// 根目錄. momo那邊無法丟過來
		//string	full_filename	=	pre_root + string(path) + string("/") + string(filename);
		wstring	full_filename	=	pre_root + qpath.toStdWString() + wstring(L"/") + qfilename.toStdWString();

		// 開啟檔案
		int		fd	=	_wopen( full_filename.c_str(), _O_BINARY | _O_RDONLY );
		if( fd == -1 )
		{
			not_found(path, connection);
			return;
		}

		// 傳輸檔案
		__int64		file_size	=	get_file_size(fd);
		//size_t	file_size		=	get_file_size(full_filename);
		if( end == FILE_POS_DEFAULT )	
			end		=	file_size;
		send_headers( connection , begin, end, file_size, filename );
		send_file( fd, begin, end, file_size, connection );
		 
		//_close(fd);		// 檢查這邊close的時機是否正確
	}

	// --------------------------------------------------------------------------------------------------------------------------
	void not_found( string const &path, 
					file_server::connection_ptr connection ) 
	{
		static file_server::response_header headers[] = {
				{"Connection","close"}
				,{"Content-Type", "text/plain"}
			};

		connection->set_status(file_server::connection::not_found);
		connection->set_headers(boost::make_iterator_range(headers, headers + 2));
		connection->write("File Not Found!");
	}

	// --------------------------------------------------------------------------------------------------------------------------
	void send_headers( file_server::connection_ptr connection , __int64 begin, __int64 end, __int64 file_size, string filename_extension ) 
	{
		string	file_name,	extension;
		Uncord::seperate_filename_extension( filename_extension, file_name, extension );

		file_server::response_header common_headers[] = {
				 { "Content-Length",		boost::lexical_cast<std::string>(file_size) }
				,{ "Content-Disposition",	string("inline; filename=") + filename_extension }		// attachment
				,{ "Content-Type",			mime_types::extension_to_type( extension ) }
				,{ "Accept-Ranges",			"bytes" }
				//,{ "Content-Range",			string("bytes ") + boost::lexical_cast<std::string>(begin) + string("-") + boost::lexical_cast<std::string>(end) + string("/") + boost::lexical_cast<std::string>(file_size) }
				,{ "Content-Range",			string("bytes ") + Uncord::int_to_string(begin) + string("-") + Uncord::int_to_string(end) + string("/") + Uncord::int_to_string(file_size) }
			};

		//std::vector<server::response_header> headers(common_headers, common_headers+5);
		//headers[0].value = boost::lexical_cast<std::string>(file_size);
		//file_headers.insert(std::make_pair(real_filename, headers));
		//file_headers.insert(std::make_pair(filename, headers));

		connection->set_status( file_server::connection::partial_content );
		connection->set_headers( boost::make_iterator_range(common_headers, common_headers + 5) );
	}


	// --------------------------------------------------------------------------------------------------------------------------
	void send_file( int fd, __int64 begin, __int64 end, __int64 file_size, file_server::connection_ptr connection )
	{
		//std::ifstream	infile( full_filename, std::ios::binary | std::ios::in );
		//infile.seekg( begin, std::ios::beg );
		_lseeki64( fd, begin, SEEK_SET );

		if( begin > end )
		{
			_close(fd);		// 檢查這邊close的時機是否正確
			return;
		}

		const int	buf_size	=	5000;

		char	buf[buf_size];
		//infile.read( buf, buf_size );
		if( _read( fd, buf, buf_size ) <= 0 )
		{
			_close(fd);
			return;
		}
		
		//if( infile.gcount() <= 0 )
			//return;

		// 這邊要小心 之前不照cpp netlib的做法 會發生crash.
		connection->write	(	boost::asio::const_buffers_1( static_cast<char const *>(buf), buf_size )  ,		// 參數1
								boost::bind( &connection_handler::handle_chunk,									// 整串是參數2
										connection_handler::shared_from_this(),	// 似乎只能用shared from this的樣子....
										fd, begin + buf_size, end, file_size, connection, _1 ) 
							);
	}


	// --------------------------------------------------------------------------------------------------------------------------
	void handle_chunk( int fd, __int64 begin, __int64 end, __int64 file_size, file_server::connection_ptr connection, boost::system::error_code const &ec ) 
	{
		if (!ec)
			send_file( fd, begin, end, file_size, connection );
		else
			_close(fd);		// 檢查這邊close的時機是否正確
	}
};















/**********************************************************
	get_file_size
***********************************************************/
__int64		get_file_size( int fd )
{
	__int64		size	=	0;
	_lseeki64( fd, 0, SEEK_END );
	size	=	_telli64(fd);
	_lseeki64( fd, 0, SEEK_SET );		
	return	size;
}



/**********************************************************
	get_file_size
***********************************************************/
size_t		get_file_size( string filename )
{
	ifstream	infile( filename );

	infile.seekg ( 0, infile.end );
	size_t	size	=	infile.tellg();

	return	size;
}



/**********************************************************
	seperate_cmd_para
***********************************************************/
void	seperate_cmd_para( string destination, string& cmd, string& para )
{
	char	cmd_c[1000],	para_c[1000];
	sscanf( destination.c_str() , "/%[^?]?%[^\n]", cmd_c, para_c );		// note: path會有斜線.

	cmd		=	string(cmd_c);
	para	=	string(para_c);
}


/**********************************************************
	seperate_filename_path
***********************************************************/
void	seperate_filename_path( string para, string &filename, string &path )
{
	char	file_name_c[1000],	path_c[1000];
	//sscanf( para.c_str() , "file_name=%[^&]&file_path=%s[^\n]", file_name_c, path_c );
	sscanf( para.c_str() , "file_path=%[^&]&file_name=%[^\n]", path_c, file_name_c );
	//string full_filename	=	string("D:\\" ) + string(path_c) + string(file_name_c);

	filename	=	string(file_name_c);
	path		=	string(path_c);
}


/**********************************************************
	is_file_exist
***********************************************************/
bool	is_file_exist( string full_filename )
{
	ifstream	infile( full_filename );

	if( infile.is_open() )
		return	true;
	else
		return	false;
}


/**********************************************************
	get_range
***********************************************************/
vector<string>	get_range( file_server::request const & request )
{
	vector<string>	range;
	int i;
	for( i = 0; i < request.headers.size(); i++ )
	{
		if( request.headers[i].name == "Range" )
		{
			//range = request.headers[i].value;
			printf( "%s\n", request.headers[i].value.c_str() );
			range.push_back( request.headers[i].value );
			break;
		}
	}

	return	range;
}



/**********************************************************
	parse_method
***********************************************************/
int		parse_method( string method )
{
	if( method == "HEAD" )
		return	ASYNC_HTTP_HEAD;
	else if( method == "GET" )
		return	ASYNC_HTTP_GET;
	else
		return	ASYNC_ERR_METHOD;
}




/**********************************************************
	handle_get
***********************************************************/
int		handle_get( file_server::request const &request, 
					file_server::connection_ptr connection,
					char *root_path )
{
	int		i;

	// analysis range
	std::vector<std::string>	range	=	get_range( request );
	__int64		begin	=	FILE_POS_DEFAULT, 
				end		=	FILE_POS_DEFAULT;
	if( range.size() == 0 )		// head no range.
		begin	=	0;
	else if( range.size() == 1 )
	{
		//connection->set_status(async_server_type::connection::partial_content);
		if( range[0].back() == '-' )
			sscanf( range[0].c_str(), "bytes=%I64d-" , &begin );	
		else
			sscanf( range[0].c_str(), "bytes=%I64d-%I64d" , &begin, &end );	
	}
	else
		BOOST_ASSERT(false);

	// note: 這邊之前改掉後會發生crash  要研究shared ptr以後再修改.
	boost::shared_ptr<connection_handler>	handler( new connection_handler() );
	(*handler)( request.destination, connection, begin, end, root_path );

	return	ASYNC_OK;
}



/**********************************************************
	handle_request
***********************************************************/
int		handle_request( file_server::request const &request, 
						file_server::connection_ptr connection,
						char *root_path )
{
	LOG_MESSAGE( 3, "receive request." );

#if 0		// player無法檢察header
	string	accept_value;
	// 檢查header.
	for ( file_server::request::vector_type::iterator itr = request.headers.begin(); itr != request.headers.end(); ++itr ) 
	{
		if( itr->name == "Accept" )
		{
			accept_value	=	itr->value;
			break;
		}
	}
	if( accept_value != "application/uncord.v1" )
	{
		connection->set_status( file_server::connection::bad_request );
		connection->write( STR_MACRO(ASYNC_ERR) );
		//response	=	http_server_type::response::stock_reply( http_server_type::response::unauthorized, STR_MACRO(HTTP_SERVER_ERR_HEADER) );
		return	ASYNC_ERR;
	}
#endif

	int		res	=	ASYNC_DEFAULT;

	switch( parse_method(request.method) )
	{
		case ASYNC_HTTP_HEAD:
			BOOST_ASSERT(false);
			//boost::shared_ptr<connection_handler> h(new connection_handler());
			//(*h)(request.destination, connection, false, 0, 0, root_path);
			break;
		case ASYNC_HTTP_GET:
			res		=	handle_get( request, connection, root_path );
			break;
		default:
			BOOST_ASSERT(false);
			/*static file_server::response_header error_headers[] = {
					{ "Connection", "close" }
				};	
			connection->set_status(file_server::connection::not_supported);
			connection->set_headers(boost::make_iterator_range(error_headers, error_headers+1));
			connection->write("Method not supported.");*/
	}

	return	res;
}


/**********************************************************
	parse_parameter
***********************************************************/
map<string,string>	parse_parameter( string para )
{
	map<string,string>	result_map;

	//if( para.back() == '/' || para.back() == '\\' )	// 理論上在之前的parse就已經把最後面的斜線拿掉了.
		//para.pop_back();

	int		pos;
	char	key_c[1000],	value_c[1000];
	string	sub_str;
	
	//
	while( !para.empty() )
	{
		pos		=	para.find_last_of('&');

		if( pos == string::npos )
		{
			sub_str		=	para;
			para.erase( 0, para.length() );
		}
		else
		{
			sub_str		=	para.substr( pos+1, string::npos );
			para.erase( pos, para.length() );
		}

		sscanf( sub_str.c_str(), "%[^=]=%[^\n]", key_c, value_c );
		result_map[ key_c ]		=	value_c;	
	}

	return	result_map;
}




/**********************************************************
	url_decode
	also see http://cpp-netlib.org/0.11.0/examples/http/simple_wget.html  cpp netlib的做法 
***********************************************************/
void	url_decode( std::string& str )
{
	QString		qstr( str.c_str() );
	QUrl		qurl( qstr );

	//printf( "%s\n", qurl.path().toStdString().c_str() );

	str		=	qurl.path().toStdString().c_str() ;
	//std::string		output( qurl.path().toStdString().c_str() );
	//return	outupt;
}