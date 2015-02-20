//
//  socket_test.cpp
//  socket
//
//  Created by hidog on 2015/2/20.
//  Copyright (c) 2015年 hidog. All rights reserved.
//

#include "socket_test.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string.h>


void UDP_socket_server()
{
    int skt;
    
    skt = socket( PF_INET, SOCK_STREAM, 0 );
    struct sockaddr_in dest;
    
    memset( &dest, 0, sizeof(dest) );
    dest.sin_family = AF_INET;
    dest.sin_port = htons(9999);
    
    bind( skt, (struct sockaddr*)&dest, sizeof(dest) );
    
    listen( skt,20 );
    
    while(true)
    {
        
    }

}