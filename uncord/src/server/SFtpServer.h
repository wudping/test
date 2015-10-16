#ifndef SFTP_SERVER_H
#define SFTP_SERVER_H


#include "server/NetServerBase.h"
#include <QString>
#include <string>

using namespace std;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SFTP_USERNAME	"Uncord"
#define SFTP_PASSWORD	"hahahaha"


UNCORD_NAME_SPACE_BEGIN


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef void (*dll_sftp_funtion_t)( char*, int, char*, char*, char*, int * );



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	SFtpServer
***********************************************************/
class SFtpServer : public NetServerBase
{
public:
	SFtpServer();
	virtual ~SFtpServer();

	void	start();
	void	stop();

	void	set_root_path( string str );

private:

	int		control;
	char	root_path[1000];		// 目前採用共用記憶體的做法 所以必須這樣寫 未來再改成有彈性的寫法....

};






UNCORD_NAME_SPACE_END


#endif