// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "ceftest_app.h"

#include <string>
#include <iostream>

//#include "cefsimple/simple_handler.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

#ifdef _WIN32
#error 
#else
#include <unistd.h>
#endif

#ifdef MACOS
#include "CoreFoundation/CoreFoundation.h"
#endif



//
/*CefTestHandler::CefTestHandler() :
    m_Browser(NULL)
{}*/



//
bool CefTestApp::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
    if (name == "ChangeTextInJS")
    {
        if (arguments.size() == 1 && arguments[0]->IsString())
        {
            CefString text = arguments[0]->GetStringValue();
            
            CefRefPtr<CefFrame> frame = m_Browser->GetMainFrame();
            
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
void CefTestApp::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    if (!m_Browser.get())
    {
        // We need to keep the main child window, but not popup windows
        m_Browser = browser;
        //m_BrowserHwnd = browser->GetWindowHandle();
    }
}


//
void CefTestApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
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
CefTestApp::CefTestApp()
{}


//
std::string CefTestApp::html_path()
{
    //char buf[80];
    //getcwd( buf, 80 );
    //printf( "path = %s\n", buf );
    
    std::string str;
    
#ifdef MACOS
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation( resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
        std::cout << "error!!";
    }
    CFRelease(resourcesURL);
    
    //chdir(path);
    std::cout << "Current Path: " << path << std::endl;
    str = "file://";
    str.append(path);
    str.append( "/html/index.html");
#endif
    
    return str;
}



//
void CefTestApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();

    // Information used when creating the native window.
    CefWindowInfo window_info;

#if defined(OS_WIN)
    // On Windows we need to specify certain flags that will be passed to
    // CreateWindowEx().
    window_info.SetAsPopup(NULL, "cefsimple");
#endif

    // SimpleHandler implements browser-level callbacks.
    //CefRefPtr<CefTestHandler> handler(new CefTestHandler());

    // Specify CEF browser settings here.
    CefBrowserSettings browser_settings;

    std::string url;

    // Check if a "--url=" value was provided via the command-line. If so, use
    // that instead of the default URL.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
    

    
    url = command_line->GetSwitchValue("url");
    if (url.empty())
        url = html_path();
        //url = "file:///Users/hidog/code/test/CEF_test/html/index.html";
    
    // Create the first browser window.
    CefBrowserHost::CreateBrowser(window_info, this, url, browser_settings, NULL);
}







