// vs2012.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
//#include "../src/server.hpp"
#include "../src/async_server.hpp"


int _tmain(int argc, _TCHAR* argv[])
{
	file_cache	cache("D:\\");
	file_server handler(cache);
	server::options options(handler);
	server instance(
	options.thread_pool(boost::make_shared<utils::thread_pool>(4)).address("192.168.1.74").port("8000"));

	instance.run();

	return 0;
}

