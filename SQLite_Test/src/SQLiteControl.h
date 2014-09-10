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


#define ERROR_MSG(msg) \
    { \
        std::cerr << "error in " << __FILE__ << " , " << __LINE__ << " msg: " << msg << std::endl; \
    }




/*****************************************************
    SQLite Control
******************************************************/
class   SQLiteControl
{
private:
    std::string     db_name;
    
public:
    SQLiteControl();
    ~SQLiteControl();
    
    void    init_db( std::string _db_name );
    
};



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void    error_msg( char* msg );



#endif
