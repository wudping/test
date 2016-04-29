#ifndef CEFTEST_RENDERER_H
#define CEFTEST_RENDERER_H


#include "include/cef_app.h"



class CefTestRenderer : public CefApp, public CefRenderProcessHandler, public CefV8Handler
{
public:
    
    CefTestRenderer();
    
    
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

    
private:
    
    CefRefPtr<CefBrowser> m_browser;
    
    
    IMPLEMENT_REFCOUNTING(CefTestRenderer);
    DISALLOW_COPY_AND_ASSIGN(CefTestRenderer);

};




#endif
