// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include "include/cef_app.h"
#include "include/cef_client.h"

#include <cstring>

// Implement application-level callbacks for the browser process.
class CefTestApp : public CefApp, public CefBrowserProcessHandler, public CefClient
{

public:
    CefTestApp();

    // CefApp methods:
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

    // CefBrowserProcessHandler methods:
    virtual void OnContextInitialized() OVERRIDE;

    //
    std::string html_path();

private:
    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(CefTestApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
