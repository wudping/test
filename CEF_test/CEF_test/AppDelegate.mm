//
//  AppDelegate.m
//  CEF_test
//
//  Created by hidog on 2016/4/20.
//  Copyright © 2016年 hidog. All rights reserved.
//

#import "AppDelegate.h"


#import "include/cef_app.h"
#import "include/cef_application_mac.h"
#import "include/cef_base.h"
#import "include/cef_browser.h"
#import "include/cef_client.h"



@interface AppDelegate ()

- (IBAction)saveAction:(id)sender;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
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

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}



@end
