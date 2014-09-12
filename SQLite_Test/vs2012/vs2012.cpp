// vs2012.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <SQLiteControl.h>
#include <ctime>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <iostream>


ExampleData		rand_example_data()
{
	ExampleData		data;

	std::stringstream	ss;

	ss << "John_" << rand() % 1000;

	data.Age		=	rand() % 100;
	data.Height		=	1.0 * rand() / rand();
	data.Name		=	ss.str();

	return		data;
}




int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));


	SQLiteControl	sqc;

	sqc.create_db( "test" );
	sqc.create_table();
	sqc.clear_db();

	ExampleData		data;

	for( int i = 0; i < 100; i++ )
	{
		data	=	rand_example_data();
		sqc.write_data( data );
	}

	std::cout << sqc.get_db_count() << std::endl;

	int		start	=	sqc.get_min_ID();

	list_example_data_t		list_data	=	sqc.read_data_range( start, start + 50 );
	std::list<ExampleData>::iterator	itr;

	for( itr = list_data.begin(); itr != list_data.end(); ++itr )
	{
		std::cout << itr->ID << " " << itr->Name << " " << itr->Age << " " << itr->Height << std::endl;
	}


	system("PAUSE");
	return 0;
}
