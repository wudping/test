//
//  SQLiteControl.h
//  SQLite_Test
//
//  Created by hidog on 2014/9/10.
//  Copyright (c) 2014年 hidog. All rights reserved.
//

#ifndef SQLITE_CONTROL_H
#define SQLITE_CONTROL_H

#include <string>
#include <sqlite3.h>


#define ERROR_MSG(msg) \
    { \
        std::cerr << "error in " << __FILE__ << " , " << __LINE__ << " msg: " << msg << std::endl; \
        assert(false); \
    }



/*****************************************************
    examaple data
******************************************************/
struct  ExampleData
{
    int             ID;
    std::string     Name;
    int             Age;
    double          Height;
};




/*****************************************************
    SQLite Control
******************************************************/
class   SQLiteControl
{
private:
    std::string     db_name;
    sqlite3         *db;
    
public:
    SQLiteControl();
    ~SQLiteControl();
    
    void            create_db( std::string _db_name );
    void            create_table();
    void            write_data( const ExampleData& data );
    ExampleData     read_data( int start, int end );
    
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void    log_message( char const *msg );



#endif
