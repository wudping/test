// sftp_test.cpp : 定義主控台應用程式的進入點。
//



#include "stdafx.h"
#include "tchar.h"

// Import the type library.
#using "D:\\code\\test\\sftp_test\\Sample.dll"

using namespace Nuane_Samples;


int _tmain(int argc, _TCHAR* argv[])
{

	ManagedClass ^test_ftp_server = gcnew ManagedClass;

	test_ftp_server->sftp_server();


    return 0;
}





