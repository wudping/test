#ifndef CEFTEST_RENDERER_H
#define CEFTEST_RENDERER_H


#include "include/cef_app.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>



class CefTestRenderer : public CefApp, public CefRenderProcessHandler, public CefV8Handler
{
public:
    
    CefTestRenderer();
    
    void create_skt()
    {
        int portno;
        struct sockaddr_in serv_addr;
        struct hostent *server;
        
        portno = atoi("9288");
        skt = socket(AF_INET, SOCK_STREAM, 0);
        if (skt < 0)
            printf("ERROR opening socket");
        server = gethostbyname("127.0.0.1");
        if (server == NULL)
            printf("ERROR, no such host\n");
        
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(skt,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
            printf("ERROR connecting");
        
        char buf[256] = "connect sucess";
        write( skt, buf, 256 );
    }
    
    void send_msg( char *str )
    {
        char buf[256] = {0};
        memcpy( buf, str, 255 );
        
        write( skt, buf, 256 );
    }
    
    
    // Virutal on CefV8Handler
    bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;
    
    // Virutal on CefV8ContextHandler
    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
    
    // CefApp methods.
    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE;

    void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE
    {
        m_browser = browser;
    }

    void    send_process_message_test();

    bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE;

private:

    int skt;

    CefRefPtr<CefBrowser> m_browser;
    
    
    IMPLEMENT_REFCOUNTING(CefTestRenderer);
    DISALLOW_COPY_AND_ASSIGN(CefTestRenderer);

};




#endif
