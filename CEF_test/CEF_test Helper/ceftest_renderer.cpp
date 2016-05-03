#include "ceftest_renderer.h"

#include "include/cef_v8.h"
#include "../CEF_test/ceftest_app.h"
#include <iostream>

//
CefTestRenderer::CefTestRenderer() :
    m_browser(NULL)
{}



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
    CefRefPtr<CefV8Value> function = CefV8Value::CreateFunction("ChangeTextInJS", handler);
    
    // Create a new object
    CefRefPtr<CefV8Value> cpp = CefV8Value::CreateObject(NULL);
    
    // Add the object to windows JS: window.cpp
    window->SetValue("cpp", cpp, V8_PROPERTY_ATTRIBUTE_NONE);
    
    // Add the function to the object
    cpp->SetValue("ChangeTextInJS", function, V8_PROPERTY_ATTRIBUTE_NONE);

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
    
    if (name == "ChangeTextInJS")
    {
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


