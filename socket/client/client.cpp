// client.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib")
 
#include <WinSock2.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <cassert>

#include <ws2tcpip.h>
#include <windows.h>



using namespace std;



void	g( int thr_id )
{
	string confirm;
    //char message[200];
 
    //開始 Winsock-DLL
    int r;
    WSAData wsaData;
    WORD DLLVersion;
    DLLVersion = MAKEWORD(2,1);
    r = WSAStartup(DLLVersion, &wsaData);
 
    //宣告給 socket 使用的 sockadder_in 結構
    SOCKADDR_IN addr;
 
    int addlen = sizeof(addr);
 
    //設定 socket
    SOCKET sConnect;
 
    //AF_INET: internet-family
    //SOCKET_STREAM: connection-oriented socket
    sConnect = socket(AF_INET, SOCK_STREAM, NULL);
 
    //設定 addr 資料
    //addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_addr.s_addr = inet_addr("192.168.2.66");

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
 
    //cout << "connect to server?[Y] or [N]" << endl;
    //cin >> confirm;

	confirm		=	"Y";
 
    if(confirm == "N")
    {
        exit(1);
    }else{
        if(confirm == "Y")
        {
            connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));


			//char *sendbuf = "sending data test";
			char	sendbuf[200];


			
			/*sprintf( sendbuf, "hello how are you? ~ %d\n", thr_id );

			if( send(sConnect, sendbuf, (int)strlen(sendbuf), 0) < 0 )
			{
				cout << "send " << sendbuf << endl;
				cout << thr_id << endl;
			}*/

			recv( sConnect, sendbuf, (int)strlen(sendbuf), 0 ) ;
			cout << sendbuf << endl;
 
            //接收 server 端的訊息
            //ZeroMemory(message, 200);
            //r = recv(sConnect, message, sizeof(message), 0);
            //cout << message << endl;
             
            //getchar();
            //getchar();
        }
    }

}


void	f(int thr_id )
{
	while(true)
		g(thr_id);
}



int TCP_client()
{
	WORD sockVersion = MAKEWORD(2,2);
	WSADATA data; 
	if(WSAStartup(sockVersion, &data) != 0)
	    return 0;
	
	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sclient == INVALID_SOCKET)
	{
	    printf("invalid socket !");
	    return 0;
	}
	
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(12345);
	serAddr.sin_addr.S_un.S_addr = inet_addr("220.135.38.106"); 
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
	    printf("connect error !");
	    closesocket(sclient);
	    return 0;
	}
	char * sendData = "hi i am client!\n";
	send(sclient, sendData, strlen(sendData), 0);
	
	char recData[255];
	int ret = recv(sclient, recData, 255, 0);
	if(ret > 0)
	{
	    recData[ret] = 0x00;
	    printf(recData);
	}
	closesocket(sclient);
	WSACleanup();
	return 0;
}


int UDP_send()
{
	WORD socketVersion = MAKEWORD(2,2);
	WSADATA wsaData; 
	if(WSAStartup(socketVersion, &wsaData) != 0)
	    return 0;
	SOCKET skt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(13578);
	sin.sin_addr.S_un.S_addr = inet_addr("192.168.2.145");
	int len = sizeof(sin);
	
	char * sendData = "send message to you.";
	sendto(skt, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);
	
	char recvData[255];     
	int ret = recvfrom(skt, recvData, 255, 0, (sockaddr *)&sin, &len);
	if(ret > 0)
	{
	    recvData[ret] = 0x00;
	    printf(recvData);
	}
	
	closesocket(skt);
	WSACleanup();
	return 0;
}


#define MAX_LENGTH 1024
int UDP_recv()
{
	WORD socketVersion = MAKEWORD(2,2);
	WSADATA wsaData; 
	if(WSAStartup(socketVersion, &wsaData) != 0)
	    return 0;
	SOCKET skt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	
	sockaddr_in sin;
	//bzero( (char*)&sin, sizeof(sin) );
	sin.sin_family = AF_INET;
	sin.sin_port = htons(13578);
	sin.sin_addr.s_addr = htonl(INADDR_ANY);

	// node: need handle bind fail error.
	int ret = bind( skt, (struct sockaddr *)&sin, sizeof(sin) );
	assert( ret > 0 );
    
	sockaddr_in client_addr;
	int length, nbytes;
	while (true) 
	{
		char buf[1000] = "";
		nbytes = recvfrom( skt, buf, MAX_LENGTH, 0, (sockaddr*)&client_addr, (socklen_t *)&length );

		if( nbytes < 0 )
		{
			printf( "." );
			Sleep(1000);
			continue;
		}

        printf("Received data form %s : %d\n", inet_ntoa(client_addr.sin_addr), htons(client_addr.sin_port));
        printf("%s\n", buf);

		char buf2[1000] = "send back.";
        sendto( skt, buf2, strlen(buf2), 0, (sockaddr*)&client_addr, length );

    }
	
	closesocket(skt);
	WSACleanup();
	return 0;
}










int _tmain(int argc, _TCHAR* argv[])
{
	//TCP_client();
	UDP_recv();
	//UDP_send();
	//g(0);

	/*boost::thread	thr[1];

	for( int i = 0; i < 1; i++ )
		thr[i]		=	boost::thread( &f, i );*/

	system("PAUSE");
	return 0;
}

