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
    init_db
******************************************************/
void    SQLiteControl::init_db( std::string _db_name )
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
void    SQLiteControl::init_db_table()
{
    if( db == NULL )
        ERROR_MSG("no db file");
    
    std::stringstream   ss;
    ss << "create table if not exists test_table"
       << "(ID integer primary key autoincrement, Name text, Age integer);";
    
    int     res;
    char    *errmsg =   NULL;
    
    res     =   sqlite3_exec( db, ss.str().c_str(), NULL, NULL, &errmsg );

    if( res != SQLITE_OK )
    {
        ERROR_MSG("create table fail.");
        assert(false);
    }
    else
    {
        log_message("create success.");
    }
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void    log_message( char *msg )
{
    std::cout << msg << std::endl;
}


