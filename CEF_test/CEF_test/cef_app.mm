
#import "ceftest_app.h"

#import <Cocoa/Cocoa.h>

#import "include/cef_browser.h"
#import "include/wrapper/cef_helpers.h"


//
void CefTestApp::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
    CEF_REQUIRE_UI_THREAD();
    
    NSView* view = (NSView*)browser->GetHost()->GetWindowHandle();
    NSWindow* window = [view window];
    std::string titleStr(title);
    NSString* str = [NSString stringWithUTF8String:titleStr.c_str()];
    [window setTitle:str];
}



