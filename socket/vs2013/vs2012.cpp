// vs2012.cpp : �w�q�D���x���ε{�����i�J�I�C
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
    DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL ����
 
    //�� WSAStartup �}�l Winsocket-DLL
    r = WSAStartup(DLLVSERION, &wsaData);
 
    //�ŧi socket ��}��T(���P���q�T,�����P����}��T,�ҥH�|�����P����Ƶ��c�s��o�Ǧ�}��T)
    SOCKADDR_IN addr;
    int addrlen = sizeof(addr);
 
    //�إ� socket
    SOCKET sListen; //listening for an incoming connection
    SOCKET sConnect; //operating if a connection was found
	//
	SOCKET sc[10];
	int sc_count = 0;
 
    //AF_INET�G��ܫإߪ� socket �ݩ� internet family
    //SOCK_STREAM�G��ܫإߪ� socket �O connection-oriented socket
    sConnect = socket(AF_INET, SOCK_STREAM, NULL);
 
    //�]�w��}��T�����
    addr.sin_addr.s_addr = inet_addr("192.168.2.66");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
 
    //�]�w Listen
    sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);//SOMAXCONN: listening without any limit
 
    //���ݳs�u
    SOCKADDR_IN clinetAddr;
	SOCKADDR_IN c1,c2;
    while(true)
    {
        cout << "waiting..." << endl;
 
        /*if(sConnect = accept(sListen, (SOCKADDR*)&clinetAddr, &addrlen))
        {
            //cout << "a connection was found" << endl;
            //printf("server: got connection from %s\n", inet_ntoa(addr.sin_addr));
 
            //�ǰe�T���� client ��
            //char *sendbuf = "sending data test";
            //send(sConnect, sendbuf, (int)strlen(sendbuf), 0);


			//���� client �ݪ��T��
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


int _tmain(int argc, _TCHAR* argv[])
{
	f();

	system("PAUSE");
	return 0;
}

