// client.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib")
 
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <cstdlib>

#include <boost/thread.hpp>


using namespace std;



void	g( int thr_id )
{
	string confirm;
    char message[200];
 
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
    addr.sin_addr.s_addr = inet_addr("192.168.0.12");

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


int _tmain(int argc, _TCHAR* argv[])
{
	g(0);

	/*boost::thread	thr[1];

	for( int i = 0; i < 1; i++ )
		thr[i]		=	boost::thread( &f, i );*/

	system("PAUSE");
	return 0;
}

