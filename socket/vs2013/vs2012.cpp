// vs2012.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"


#pragma comment(lib, "Ws2_32.lib")
 
#include <WinSock2.h>
#include <iostream>
#include <stdlib.h>
 

using namespace std;



//
// Constants
//
#define MoveMemory RtlMoveMemory
#define CopyMemory RtlCopyMemory
#define FillMemory RtlFillMemory
#define ZeroMemory RtlZeroMemory



int f()
{
    char message[200];


    int r;
    WSAData wsaData;
    WORD DLLVSERION;
    DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL 版本
 
    //用 WSAStartup 開始 Winsocket-DLL
    r = WSAStartup(DLLVSERION, &wsaData);
 
    //宣告 socket 位址資訊(不同的通訊,有不同的位址資訊,所以會有不同的資料結構存放這些位址資訊)
    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
 
    //建立 socket
    SOCKET sListen; //listening for an incoming connection
    SOCKET sConnect; //operating if a connection was found
	//
	SOCKET sc[10];
	int sc_count = 0;
 
    //AF_INET：表示建立的 socket 屬於 internet family
    //SOCK_STREAM：表示建立的 socket 是 connection-oriented socket
    sConnect = socket(AF_INET, SOCK_STREAM, NULL);
 
    //設定位址資訊的資料
    addr.sin_addr.s_addr = inet_addr("192.168.2.66");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
 
    //設定 Listen
    sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);//SOMAXCONN: listening without any limit
 
    //等待連線
    SOCKADDR_IN clinetAddr;
	SOCKADDR_IN c1,c2;
    while(true)
    {
        cout << "waiting..." << endl;
 
        /*if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
        {
            //cout << "a connection was found" << endl;
            //printf("server: got connection from %s\n", inet_ntoa(addr.sin_addr));
 
            //傳送訊息給 client 端
            //char *sendbuf = "sending data test";
            //send(sConnect, sendbuf, (int)strlen(sendbuf), 0);


			//接收 client 端的訊息
            ZeroMemory(message, 200);
            r = recv(sConnect, message, sizeof(message), 0);
            cout << message << endl;
             
        }*/

		sc[0]	=	accept(sListen, (SOCKADDR*)&c1, &addrlen);
        cout << "get sc0..." << endl;

		sc[1]	=	accept(sListen, (SOCKADDR*)&c2, &addrlen);
        cout << "get sc1..." << endl;

        ZeroMemory(message, 200);
		r = recv( sc[0], message, sizeof(message), 0);
        cout << "get msg " << message << endl;

		r = send( sc[1], message, sizeof(message), 0);
        cout << "send msg " << message << endl;


    }
 
}



int UDP_send()
{
	WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2,2);
    if( WSAStartup( sockVersion, &wsaData) != 0 )
        return 0;

    SOCKET skt = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); 
    if(skt == INVALID_SOCKET)
    {
        printf("err = %d socket error !", WSAGetLastError() );
        return 0;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(13578);
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    if( bind( skt, (sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        printf("err = %d bind error !", WSAGetLastError() );
        closesocket(skt);
        return 0;
    }
    
    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr);
    for( int i = 0; i < 10; i++ )
    {
        char recvData[255];  
        int ret = recvfrom( skt, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);
        if (ret > 0)
        {
            recvData[ret] = 0x00;
            printf( "recv：%s \n", inet_ntoa(remoteAddr.sin_addr));
            printf( "%s", recvData );            
        }

        char sendData[100];
		sprintf( sendData, "I get your data. send back. time = %d", i);
        sendto( skt, sendData, strlen(sendData), 0, (sockaddr *)&remoteAddr, nAddrLen);    
    }
    closesocket(skt); 
    WSACleanup();
    return 0;
}



int TCP_server()
{
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA wsaData;
	if(WSAStartup(sockVersion, &wsaData) != 0)
	    return 0;
	
	//
	SOCKET skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(skt == INVALID_SOCKET)
	{
	    printf("socket error !");
	    return 0;
	}
	
	//
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(12345);
	sin.sin_addr.S_un.S_addr = INADDR_ANY; 
	if(bind(skt, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	    printf("bind error !");
	
	//
	if(listen(skt, SOMAXCONN) == SOCKET_ERROR)
	{
	    printf("listen error !");
	    return 0;
	}
	
	//
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255]; 
	while (true)
	{
	    printf("wait connect...\n");
	    sClient = accept(skt, (SOCKADDR *)&remoteAddr, &nAddrlen);
	    if(sClient == INVALID_SOCKET)
	    {
	        printf("accept error !");
	        continue;
	    }
	    printf("get connect：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
	    
	    //接收数据
	    int ret = recv(sClient, revData, 255, 0);        
	    if(ret > 0)
	    {
	        revData[ret] = 0x00;
	        printf(revData);
	    }
	
	    //发送数据
	    char * sendData = "hi i am server！\n";
	    send(sClient, sendData, strlen(sendData), 0);
	    closesocket(sClient);
	}
	
	closesocket(skt);
	WSACleanup();
	return 0;
}



int TCP_server_nonblocking()
{
	WSADATA WSAData = { 0 };
	WORD sockVersion = MAKEWORD(2,2);
    if (WSAStartup(sockVersion, &WSAData) != 0)
    {
        // Tell the user that we could not find a usable WinSock DLL.
        if (LOBYTE(WSAData.wVersion) != LOBYTE(sockVersion) ||
            HIBYTE(WSAData.wVersion) != HIBYTE(sockVersion))
            printf("Incorrect winsock version\n");

        WSACleanup();
        return 0;
    }

	int nFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nFd == INVALID_SOCKET)
	{
		printf("Error creating socket, ec: %d\n", WSAGetLastError());
		return false;
	}

	unsigned long nNonBlocking = 1;
	if (ioctlsocket(nFd, FIONBIO, &nNonBlocking) == SOCKET_ERROR)
	{
		printf("Unable to set nonblocking mode, ec:%d\n", WSAGetLastError());
		closesocket(nFd);
		return false;
	}

	sockaddr_in oAddr;

	oAddr.sin_family = AF_INET;
	oAddr.sin_addr.s_addr = INADDR_ANY;
	oAddr.sin_port = htons((u_short)SERVER_PORT);

	if (bind(nFd,(sockaddr*)&oAddr, sizeof(oAddr)) != 0)
	{
		printf("Error binding socket, ec: %d\n", WSAGetLastError());
		closesocket(nFd);
		return false;
	}

	if (listen(nFd, MAXCONN) != 0)
	{
		printf("Error listening, ec: %d\n", WSAGetLastError());
		closesocket(nFd);
		return false;
	}

	int nCliFd = 0;
	fd_set oRSet;
	int nReady, nCliLen, nError;

	while (true)
	{
		FD_ZERO(&oRSet);
		FD_SET(nFd, &oRSet);
		nReady = select(FD_SETSIZE, &oRSet, NULL, NULL, NULL);

		if (FD_ISSET(nFd, &oRSet))
		{
			nCliLen = sizeof(oCliAddr);
			nCliFd = accept(m_nFd, (struct sockaddr*)&oCliAddr, &nCliLen);

			if (nCliFd < 0)
			{
				nError = WSAGetLastError();
				if (nError == WSAEWOULDBLOCK)
					continue;
				else
				{
					printf("Accept error, ec:%d\n", nError);
					return false;
				}
			}
		}
		::Sleep(100);
}
}



int _tmain(int argc, _TCHAR* argv[])
{
	TCP_server();
	//UDP_send();
	//f();

	system("PAUSE");
	return 0;
}

