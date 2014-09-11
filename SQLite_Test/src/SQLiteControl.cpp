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
    if( db_name.empty() )
    {
        db_name     =   _db_name;
        db_name.append( ".db" );
        // create db file
        sqlite3_open( db_name.c_str(), &db );
    }
    else
    {
        ERROR_MSG("");
        assert(false);
    }
}













