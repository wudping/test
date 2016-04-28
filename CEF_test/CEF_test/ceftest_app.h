// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_life_span_handler.h"

#include <cstring>


//
/*class CefTestHandler : public CefClient, public CefV8Handler, public CefRenderProcessHandler, public CefLifeSpanHandler, public CefLoadHandler, public CefDisplayHandler
{
public:
    
    CefTestHandler();
    


    // Virutal on CefV8Handler
    bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;
    
    // Virutal on CefV8ContextHandler
    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;
    
    //
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

    
private:
    
    // The child browser window
    CefRefPtr<CefBrowser> m_Browser;
    
    IMPLEMENT_REFCOUNTING(CefTestHandler);
};*/







// Implement application-level callbacks for the browser process.
class CefTestApp : public CefApp, public CefBrowserProcessHandler, public CefClient, public CefV8Handler, public CefRenderProcessHandler, public CefLifeSpanHandler, public CefLoadHandler, public CefDisplayHandler
{

public:
    CefTestApp();

    // CefClient methods:
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE
    {
        return this;
    }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
    {
        return this;
    }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE
    {
        return this;
    }

    // CefApp methods:
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

    // CefBrowserProcessHandler methods:
    virtual void OnContextInitialized() OVERRIDE;

    //
    std::string html_path();

    // Virutal on CefV8Handler
    bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception) OVERRIDE;

    // Virutal on CefV8ContextHandler
    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;

    //
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;



private:

    // The child browser window
    CefRefPtr<CefBrowser> m_Browser;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(CefTestApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
