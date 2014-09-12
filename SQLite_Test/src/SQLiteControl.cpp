//
//  SQLiteControl.cpp
//  SQLite_Test
//
//  Created by hidog on 2014/9/10.
//  Copyright (c) 2014年 hidog. All rights reserved.
//

#include "SQLiteControl.h"

#include <sqlite3.h>
#include <cassert>
#include <iostream>
#include <sstream>






/*****************************************************
    Constructor
******************************************************/
SQLiteControl::SQLiteControl()
    :   db(NULL)
{}



/*****************************************************
    Destructor
 ******************************************************/
SQLiteControl::~SQLiteControl()
{}




/*****************************************************
    clear_db
******************************************************/
void	SQLiteControl::clear_db()
{
	char	*errmsg		=	NULL;

	std::stringstream	ss;

	ss << "delete from test_table;";

	int		res		=	sqlite3_exec( db, ss.str().c_str(), NULL, NULL, &errmsg );

	if( res != SQLITE_OK ) {
 		ERROR_MSG( errmsg );
	} else {
		log_message( 5, "clear all success." );
	}
}



/*****************************************************
    init_db
******************************************************/
void    SQLiteControl::create_db( std::string _db_name )
{
    if( db_name.empty() && db == NULL )
    {
        db_name     =   _db_name;
        // create db file
        db_name.append(".db");
        int res     =   sqlite3_open( db_name.c_str(), &db );
        if( res == SQLITE_OK )
            log_message("create database success.");
    }
    else
    {
        ERROR_MSG("");
        assert(false);
    }
}



/*****************************************************
    init_db
******************************************************/
void    SQLiteControl::create_table()
{
    if( db == NULL )
    {
        ERROR_MSG("no db file");
        return;
    }
    
    std::stringstream   ss;
    ss << "CREATE TABLE IF NOT EXISTS test_table"
       << "(ID INTEGER PRIMARY KEY AUTOINCREMENT,"
       << "Name TEXT,"
       << "Age INTEGER,"
       << "Height REAL);" ;
    
    log_message( 1, ss.str().c_str() );
    
    int     res;
    char    *errmsg =   NULL;
    
    res     =   sqlite3_exec( db, ss.str().c_str(), NULL, NULL, &errmsg );

    if( res != SQLITE_OK ) {
        ERROR_MSG(errmsg);
    } else {
        log_message("create success.");
    }
}


/*****************************************************
    write_data
 
 INSERT INTO TABLE_NAME (column1, column2, column3,...columnN)]
 VALUES (value1, value2, value3,...valueN);
******************************************************/
void    SQLiteControl::write_data( const ExampleData& data )
{
    if( db == NULL )
    {
        ERROR_MSG("no db file");
        return;
    }
    
    std::stringstream   ss;
    ss << "INSERT INTO test_table "
       << "(Name,Age,Height) "
       << "VALUES ("
       << "'" << data.Name << "'" << ","
       << data.Age << ","
       << data.Height << ");" ;
    
    log_message( 1, ss.str().c_str() );
    
    int     res;
    char    *err    =   NULL;
    
    res     =   sqlite3_exec( db, ss.str().c_str(), NULL, NULL, &err );
    
    if( res != SQLITE_OK ) {
        ERROR_MSG( err );
    } else {
        log_message( 1, "write data success!" );
    }    
}



/*****************************************************
    get_data
******************************************************/
ExampleData		SQLiteControl::get_data( sqlite3_stmt *stmt )
{
    ExampleData     data;

    data.ID     =   sqlite3_column_int( stmt, 0 );
    data.Name   =   (char*)sqlite3_column_text( stmt, 1 );
    data.Age    =   sqlite3_column_int( stmt, 2 );
    data.Height =   sqlite3_column_double( stmt, 3 );
    
    //std::cout << "ID = " << data.ID << " Name = " << data.Name << " Age = " << data.Age
    //          << " Height = " << data.Height << std::endl;

	return	data;
}




/*****************************************************
    get_min_ID
******************************************************/
int		SQLiteControl::get_min_ID()
{
	if( db == NULL )
	{
		ERROR_MSG("no db");
		return	0;
	}

	int		res;
	int		result	=	0;

	std::stringstream	ss;
	ss << "select min(ID) from test_table;";

	sqlite3_stmt	*stmt	=	NULL;

	res		=	sqlite3_prepare( db, ss.str().c_str(), -1, &stmt, NULL );

	if( res != SQLITE_OK )
	{
		ERROR_MSG( "get db_count error" );
		return	0;
	}

	res		=	sqlite3_step( stmt );

	if( res == SQLITE_ROW ) {
		result	=	sqlite3_column_int( stmt, 0 );
	} else {
		ERROR_MSG("get db count fail.");
		result	=	0;
	}

	return	result;
}



/*****************************************************
    db_count
******************************************************/
int		SQLiteControl::get_db_count()
{
	if( db == NULL )
	{
		ERROR_MSG("no db");
		return	0;
	}

	int		res;
	int		result	=	0;

	std::stringstream	ss;
	ss << "select count(*) from test_table;";

	sqlite3_stmt	*stmt	=	NULL;

	res		=	sqlite3_prepare( db, ss.str().c_str(), -1, &stmt, NULL );

	if( res != SQLITE_OK )
	{
		ERROR_MSG( "get db_count error" );
		return	0;
	}

	res		=	sqlite3_step( stmt );

	if( res == SQLITE_ROW ) {
		result	=	sqlite3_column_int( stmt, 0 );
	} else {
		ERROR_MSG("get db count fail.");
		result	=	0;
	}

	return	result;
}



/*****************************************************
    read_data_range
******************************************************/
list_example_data_t     SQLiteControl::read_data_range( int start, int end )
{
    if( db == NULL )
    {
        ERROR_MSG("db is not exists");
        return  list_example_data_t();
    }
    
    std::stringstream   ss;
    ss << "select * from test_table where ID >= " << start << " and ID <= " << end << ";";
    
    log_message( 1, ss.str().c_str() );
    
    int		res;
    
	sqlite3_stmt	*stmt   =   NULL;
    ExampleData     data;

    list_example_data_t		result;

    res     =   sqlite3_prepare( db, ss.str().c_str(), -1, &stmt, 0 );
    
    if( res != SQLITE_OK )
    {
        ERROR_MSG("db query fail.");
        return  list_example_data_t();
    }
    
	// start get data.
    while(true)
    {
        res     =   sqlite3_step( stmt );

        if( res == SQLITE_ROW ) {
			data	=	get_data( stmt ); 
			result.push_back(data);            
        } else if( res == SQLITE_BUSY ) {
			sqlite3_sleep(10);
        } else if( res == SQLITE_DONE ) {
            break;
        } else {
            ERROR_MSG("db query fail.");
        }
    }
                                
    return  result;
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void    log_message( char const *msg )
{
    std::cout << msg << std::endl;
}


void    log_message( int level, char const *msg )
{
	if( level > LOG_LEVEL )
		std::cout << msg << std::endl;
}

