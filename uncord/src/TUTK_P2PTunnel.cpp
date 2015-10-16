#include "TUTK_P2PTunnel.h"


#include <DataManager.h>


//#include <WinBase.h>		// win8­n­×§ï 


char gUID[21];

typedef struct st_AuthData
{
	char szUsername[64];
	char szPassword[64];
} sAuthData;






using namespace std;

UNCORD_NAME_SPACE_BEGIN














// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/**********************************************************
	constructor
***********************************************************/
TUTK_P2PTunnel::TUTK_P2PTunnel()
	:	UncordBase()
{}






/**********************************************************
	destructor
***********************************************************/
TUTK_P2PTunnel::~TUTK_P2PTunnel()
{}






/**********************************************************
	Thread_TestCloseSession
***********************************************************/
void *TUTK_P2PTunnel::Thread_TestCloseSession(void *arg)
{
	//pthread_detach(pthread_self());
	
	int SID = *((int *)arg);
	free(arg);
	srand(time(NULL));
    int nDelayTime = rand() % 60;
    printf("nDelayTime to close[%d]\n", nDelayTime);

	//Sleep(nDelayTime);
	//Sleep(nDelayTime);
	_sleep(nDelayTime);


	printf("P2PTunnelServer_Disconnect SID[%d] ret[%d]\n", SID, P2PTunnelServer_Disconnect(SID));
	
	return 0;
}






/**********************************************************
	main_tutk
***********************************************************/
int TUTK_P2PTunnel::main_tutk()
{
	DataManager*	p_data_manager	=	(DataManager*)get_instance(DATA_MANAGER);

	if( p_data_manager->get_UID().length() <= 1 )
	{
		BOOST_ASSERT(false);
		// not get uid yet...
	}

	//strcpy(gUID, "L4DBLJFUSZZ6P5YD111A");
	strcpy(gUID, p_data_manager->get_UID().c_str() );

	
	char *s = "My arg Pass to call back function";
	P2PTunnelServer_GetStatus(  Tunnel_Status_CB, (void *)s);
	printf("Tunnel Version[%X]\n", P2PTunnel_Version());
	int ret = P2PTunnelServerInitialize(20);
	if(ret < 0)
	{
		printf("P2PTunnelServerInitialize error[%d]!\n", ret);
		return -1;
	}
	ret = P2PTunnelServer_Start(gUID);
	if(ret < 0)
	{
		printf("P2PTunnelServer_Start error[%d]!\n", ret);
		return -1;
	}
	
	/* If you don't want to use authentication mechanism, you can give NULL argument
	ret = P2PTunnelServer_GetSessionInfo(TunnelSessionInfoCB, NULL);
	*/
	ret = P2PTunnelServer_GetSessionInfo( Tunnel_SessionInfo_CB, (void *)s);
	
	printf("Call P2PTunnelServer_GetSessionInfo ret[%d]\n", ret);
	
	while(1)
	{
		//sleep(3600);
		//Sleep(3600);
		_sleep(3600);

	}
	
	return 0;
}












UNCORD_NAME_SPACE_END




	
/**********************************************************
	TunnelStatusCB
***********************************************************/
void __stdcall Tunnel_Status_CB(int nErrorCode, int nSID, void *pArg)
{
	if(nErrorCode == TUNNEL_ER_DISCONNECTED)
	{
		printf("SID[%d] TUNNEL_ER_DISCONNECTED Log file here!\n", nSID);
		if(pArg != NULL)
			printf("MyArg = %s\n", (char *)pArg);
	}
}


/**********************************************************
	TunnelSessionInfoCB
***********************************************************/
int __stdcall Tunnel_SessionInfo_CB(sP2PTunnelSessionInfo *sSessionInfo, void *pArg)
{
	printf("TunnelSessionInfoCB trigger\n");
	if(pArg != NULL) printf("pArg = %s\n", (char *)pArg);
	printf("[Client Session Info]\n");
	printf("  Connection Mode = %d, NAT type = %d\n", sSessionInfo->nMode, sSessionInfo->nNatType);
	printf("  P2PTunnel Version = %X, SID = %d\n", (unsigned int)sSessionInfo->nVersion, sSessionInfo->nSID);
	printf("  IP Address = %s:%d\n", sSessionInfo->szRemoteIP, sSessionInfo->nRemotePort);
	
	if(sSessionInfo->nAuthDataLen == 0 || sSessionInfo->pAuthData == NULL)
		return -777;
	else if(sSessionInfo->nAuthDataLen > 0)
	{
		sAuthData *pAuthData = (sAuthData *)sSessionInfo->pAuthData;
		printf("  Auth data length = %d, username = %s, passwd = %s\n", sSessionInfo->nAuthDataLen, pAuthData->szUsername, pAuthData->szPassword);
		if(strcmp(pAuthData->szUsername, "Tutk.com") != 0 || strcmp(pAuthData->szPassword, "P2P Platform") != 0)
			return -888;
	}
	
	#if 0 // test random time to close session
	int *SID = (int *)malloc(sizeof(int));
	if(SID != NULL)
	{
		*SID = sSessionInfo->nSID;
		pthread_t threadID;
		pthread_create(&threadID, NULL, &Thread_TestCloseSession, (void *)SID);
	}
	#endif
	
	return 0;
}
