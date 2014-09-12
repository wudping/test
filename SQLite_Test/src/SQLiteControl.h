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
#include <list>
#include <sqlite3.h>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ define ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define ERROR_MSG(msg) \
    { \
        std::cerr << "error in " << __FILE__ << " , " << __LINE__ << " msg: " << msg << std::endl; \
        assert(false); \
    }

#define LOG_LEVEL 4

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ typedef ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

typedef std::list<ExampleData> list_example_data_t;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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
    
    void    create_db( std::string _db_name );
    void    create_table();
    void	write_data( const ExampleData& data );
	void	clear_db();
	int		get_db_count();
	int		get_min_ID();

	ExampleData		get_data( sqlite3_stmt *stmt );

    list_example_data_t     read_data_range( int start, int end );
    
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void    log_message( char const *msg );
void    log_message( int level , char const *msg );



#endif
