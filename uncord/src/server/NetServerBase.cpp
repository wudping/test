#include "server/NetServerBase.h"

using namespace std;

UNCORD_NAME_SPACE_BEGIN



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/**********************************************************
	constructor
***********************************************************/
NetServerBase::NetServerBase()
	:	NetBase()
{
	port	=	8888;
	ip		=	"127.0.0.1";

	//root_path	=	"./";
}






/**********************************************************
	destructor
***********************************************************/
NetServerBase::~NetServerBase()
{}









UNCORD_NAME_SPACE_END
