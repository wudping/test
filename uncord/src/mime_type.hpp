#ifndef MIME_TYPE_H
#define MIME_TYPE_H

#include <string>

//#pragma once


//namespace http {
//namespace server {
namespace mime_types {






/**********************************************************
	struct   mapping
	此結構不想給外部使用,包在匿名namespace底下
***********************************************************/
namespace{
struct	mapping
{
  const char*	extension;
  const char*	mime_type;
} mappings[] =
{
  { "gif",		"image/gif" },
  { "htm",		"text/html" },
  { "html",		"text/html" },
  { "jpg",		"image/jpeg" },
  { "png",		"image/png" },
  { "js",		"text/javascript" },
  { "css",		"text/css" },
  { "mp4",		"vedio/mp4" },
  { "mkv",		"vedio/mkv" }, 
  { "avi",		"vedio/avi" },
  { 0, 0 } // Marks end of list.
};

/**********************************************************
	extension_to_type  
***********************************************************/
std::string		extension_to_type( const std::string& extension )
{
	for ( mapping* m = mappings; m->extension; ++m )
	{
		if (m->extension == extension)
			return m->mime_type;
	}
	return "text/plain";
}
}	// end Anonymous namespace






} // namespace mime_types
//} // namespace server
//} // namespace http


#endif