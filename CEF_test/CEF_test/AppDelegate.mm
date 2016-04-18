
#import "AppDelegate.h"

#import "include/cef_app.h"
#import "include/cef_application_mac.h"
#import "include/cef_base.h"
#import "include/cef_browser.h"
#import "include/cef_client.h"
#import "include/cef_command_line.h"
#import "include/cef_frame.h"
#import "include/cef_runnable.h"
#import "include/cef_web_plugin.h"
#import "include/cef_web_urlrequest.h"

#import "ClientHandler.h"

#include <iostream>

@implementation AppDelegate

@synthesize window;

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    CefSettings appSettings;
    CefBrowserSettings browserSettings;
    
    CefRefPtr<CefApp> cefApplication;
    CefRefPtr<CefClient> client(new ClientHandler());
    
    CefWindowInfo info;
    info.SetAsChild([window contentView], 0, 0, [[window contentView] frame].size.width, [[window contentView] frame].size.height);
    
    CefInitialize(appSettings, cefApplication);
    
    std::string path = [[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSUTF8StringEncoding];
    path = "file://" + path + "/html/index.html";
    
    CefBrowser::CreateBrowser(info, client, path, browserSettings);
    
    CefRunMessageLoop();
}

@end
