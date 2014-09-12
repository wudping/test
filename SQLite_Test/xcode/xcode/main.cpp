//
//  main.cpp
//  xcode
//
//  Created by hidog on 2014/9/10.
//  Copyright (c) 2014年 hidog. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <SQLiteControl.h>


int main(int argc, const char * argv[])
{

    SQLiteControl   sqc;
    
    sqc.create_db( "test" );
    sqc.create_table();
    
    ExampleData     data;
    data.Name       =   "Peter";
    data.Age        =   31;
    data.Height     =   452.441;

    sqc.write_data( data );
    
    sqc.read_data( 1, 5 );
    
    return 0;
}
/*
 測試請衝突 要怎麼處理
 */

