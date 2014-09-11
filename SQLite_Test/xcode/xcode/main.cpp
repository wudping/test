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
    
    sqc.init_db( "test" );
    
    FILE *p = fopen("thisatest.txt","a+");
    fprintf( p, "this is a test.!!");
    fclose(p);
    
    
    return 0;
}

