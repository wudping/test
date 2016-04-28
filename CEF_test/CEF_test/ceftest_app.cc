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
static CefTestApp *p_app = NULL;
CefTestApp* get_app_instance()
{
    return p_app;
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
CefTestApp::CefTestApp()
{
    if( p_app != NULL )
        printf("error\n");
    p_app   =   this;
}


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







