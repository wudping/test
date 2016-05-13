#include "ceftest_renderer.h"

#include "include/cef_v8.h"
#include "../CEF_test/ceftest_app.h"
#include <iostream>

//
CefTestRenderer::CefTestRenderer() :
    m_browser(NULL)
{
    create_skt();
}



//
void CefTestRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
    printf("on context created.\n");
    
    // Retrieve the context's window object.
    CefRefPtr<CefV8Value> window = context->GetGlobal();
    
    // Create an instance of my CefV8Handler object.
    // In this case it's this object, and content will be executed in bool ClientHandler::Execute(...)
    CefRefPtr<CefV8Handler> handler = this;
    
    // Create a function.
    CefRefPtr<CefV8Value> function1 = CefV8Value::CreateFunction("JS_btn_1", handler);
    CefRefPtr<CefV8Value> function2 = CefV8Value::CreateFunction("JS_btn_2", handler);
    CefRefPtr<CefV8Value> function3 = CefV8Value::CreateFunction("JS_btn_3", handler);
    
    // Create a new object
    CefRefPtr<CefV8Value> cpp = CefV8Value::CreateObject(NULL);
    
    // Add the object to windows JS: window.cpp
    window->SetValue("cpp", cpp, V8_PROPERTY_ATTRIBUTE_NONE);
    
    // Add the function to the object
    cpp->SetValue("JS_btn_1", function1, V8_PROPERTY_ATTRIBUTE_NONE);
    cpp->SetValue("JS_btn_2", function2, V8_PROPERTY_ATTRIBUTE_NONE);
    cpp->SetValue("JS_btn_3", function3, V8_PROPERTY_ATTRIBUTE_NONE);
    
}



//
void    CefTestRenderer::send_process_message_test()
{
    printf("Helper start send msg\n");
    
    // Create the message object.
    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create("CEF_test test msg Helper");
    
    // Retrieve the argument list object.
    CefRefPtr<CefListValue> args = msg->GetArgumentList();
    
    //CefString str = "test_aaaaaaa";
    
    //std::cout << "---" << str.ToString().c_str() << "   " << str.length() << "---\n";
    
    // Populate the argument values.
    //args->SetString( 0, str );
    
    args->SetString( 0, "testtt   aaa " );
    args->SetString( 1, "message 2. just test." );
    //args->SetInt( 0, 10 );
    //args->SetInt( 1, 99 );
    //args->SetDouble( 0, 1.9911 );
    
    //std::cout << "......." << args->GetString(0).length() << ".....\n";
    
    // Send the process message to the render process.
    // Use PID_BROWSER instead when sending a message to the browser process.
    m_browser->SendProcessMessage( PID_BROWSER, msg );
    
    printf("Helper end sed msg\n");
}




//
bool CefTestRenderer::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
    printf("Execute\n");
    
    if (name == "JS_btn_1")
    {
        printf("JS_btn_1\n");
        send_process_message_test();
        
        if (arguments.size() == 1 && arguments[0]->IsString())
        {
            CefString text = arguments[0]->GetStringValue();
            
            if( m_browser.get() == NULL )
            {
                printf("NULL ptr\n");
                return true;
            }
            
            CefRefPtr<CefFrame> frame = m_browser->GetMainFrame();
            
            std::string jscall = "ChangeText('";
            
            jscall += text;
            jscall += "');";
            
            frame->ExecuteJavaScript(jscall, frame->GetURL(), 0);
            
            /*
             * If you want your method to return a value, just use retval, like this:
             * retval = CefV8Value::CreateString("Hello World!");
             * you can use any CefV8Value, what means you can return arrays, objects or whatever you can create with CefV8Value::Create* methods
             */
            
            char buf[256] = "btn1 clicked.";
            send_msg(buf);
            
            return true;
        }
    }
    else if( name == "JS_btn_2" )
    {
        printf("JS_btn_2\n");
        
        if (arguments.size() == 1 && arguments[0]->IsString())
        {
            CefString text = arguments[0]->GetStringValue();
            
            if( m_browser.get() == NULL )
            {
                printf("NULL ptr\n");
                return true;
            }
            
            CefRefPtr<CefFrame> frame = m_browser->GetMainFrame();
            
            std::string jscall = "ChangeText_2('";
            
            jscall += text;
            jscall += "');";
            
            frame->ExecuteJavaScript(jscall, frame->GetURL(), 0);
            
            char buf[256] = "btn 2 clicked.";
            send_msg( buf );
            
            return true;
        }
    }
    else if( name == "JS_btn_3" )
    {
        printf("JS_btn_3\n");
        
        if (arguments.size() == 1 && arguments[0]->IsString())
        {
            CefString text = arguments[0]->GetStringValue();
            
            if( m_browser.get() == NULL )
            {
                printf("NULL ptr\n");
                return true;
            }
            
            printf("str = %s", text.ToString().c_str() );
            
            char buf[256];
            sprintf( buf, "btn3 with msg = %s", text.ToString().c_str() );
            send_msg( buf );
            
            return true;
        }
    }
    
    return false;
}


//
bool CefTestRenderer::OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
    // Check the message name.
    const std::string& message_name = message->GetName();
    
    printf("receive msg name = %s\n", message_name.c_str() );

    
    return  true;
}


// CefApp methods.
CefRefPtr<CefRenderProcessHandler> CefTestRenderer::GetRenderProcessHandler()
{
    return this;
}


