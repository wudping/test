// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "include/cef_app.h"

#include "ceftest_renderer.h"
#include "ceftest_other.h"




//
int RunMain(int argc, char* argv[])
{
    CefMainArgs main_args(argc, argv);
        
    // Parse command-line arguments.
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromArgv(argc, argv);
        
    // Create a ClientApp of the correct type.
    CefRefPtr<CefApp> app;

    
    if( !command_line->HasSwitch("type"))
        printf("browser process\n");
    
    const std::string& process_type = command_line->GetSwitchValue("type");
    
    if ( process_type == "renderer" )
    {
        app = new CefTestRenderer();
        printf("renderer process\n");
    }
    else
    {
        //app = new CefTestOther();
        printf("other process\n");
    }
    
    
    // Execute the secondary process.
    return CefExecuteProcess(main_args, app, NULL);
}
    







// Entry point function for sub-processes.
int main(int argc, char* argv[])
{
    printf("enter helper.\n");
    
    // Provide CEF with command-line arguments.
    //CefMainArgs main_args(argc, argv);

    /*if( !command_line->HasSwitch("type"))
        printf("browserprocess\n");
    const std::string& process_type = command_line->GetSwitchValue("type");
    if ( process_type == "renderer" )
        printf("renderer\n");
    else
        printf("other\n");*/
    
    
    // Execute the sub-process.
    return RunMain( argc, argv );
}
