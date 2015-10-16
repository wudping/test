#ifndef TUTK_P2PTUNNEL_H
#define TUTK_P2PTUNNEL_H

#include <UncordBase.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <time.h>
//#include <pthread.h>
#include "P2PTunnelAPIs.h"


using namespace std;







UNCORD_NAME_SPACE_BEGIN






// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/**********************************************************
	Server
***********************************************************/
class	TUTK_P2PTunnel : public UncordBase
{
public:
	TUTK_P2PTunnel();
	virtual ~TUTK_P2PTunnel();

	void *Thread_TestCloseSession(void *arg);



	int		main_tutk();


private:

};








UNCORD_NAME_SPACE_END


void __stdcall Tunnel_Status_CB(int nErrorCode, int nSID, void *pArg);
int __stdcall Tunnel_SessionInfo_CB(sP2PTunnelSessionInfo *sSessionInfo, void *pArg);



#endif