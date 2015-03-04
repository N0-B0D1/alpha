/**
Copyright 2014-2015 Jason R. Wendlandt

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <Windows.h>

#include "HID/HIDWindowListener.h"
#include "Graphics/GraphicsWindow.h"
#include "Toolbox/Logger.h"


namespace alpha
{
    static HIDWindowListener * g_pWindowListener = nullptr;
    static LRESULT CALLBACK StaticInputWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (g_pWindowListener != nullptr)
        {
            return g_pWindowListener->InputWndProc(hWnd, message, wParam, lParam);
        }
        return S_OK;
    }

    HIDWindowListener::HIDWindowListener()
        : m_origWndProc(nullptr)
        , m_mouseTracking(false)
    {
        // set global pointer, so static wndproc can call this class instance, which should be a singlton, but we don't enforce that ...
        g_pWindowListener = this;
        RegisterRawHIDs();

        // hi-jack the current window message function, store old one so we can still call it.
        m_origWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWL_WNDPROC, (LONG)StaticInputWndProc);
    }

    HIDWindowListener::~HIDWindowListener()
    {
        // un-hi-jack WndProc
        SetWindowLongPtr(g_hWnd, GWL_WNDPROC, (LONG)m_origWndProc);
    }

    LRESULT CALLBACK HIDWindowListener::InputWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        //int count;

        switch (message)
        {
        case WM_MOUSEMOVE:
            if (!m_mouseTracking)
            {
                m_mouseTracking = true;
                while (ShowCursor(false) >= 0);
            }
            break;

        case WM_NCMOUSELEAVE:
            m_mouseTracking = false;
            while (ShowCursor(true) < 0);
            break;
        }

        // Call the original wndproc setup by our graphics window manager class instance.
        return CallWindowProc(m_origWndProc, hWnd, message, wParam, lParam);
    }

    void HIDWindowListener::RegisterRawHIDs()
    {
        RAWINPUTDEVICE rid[2];

        // mouse support
        rid[0].usUsagePage = 0x01;
        rid[0].usUsage = 0x02;
        rid[0].dwFlags = 0; // RIDEV_NOLEGACY;
        rid[0].hwndTarget = 0;

        // keyboard support
        rid[1].usUsagePage = 0x01;
        rid[1].usUsage = 0x06;
        rid[1].dwFlags = 0; // RIDEV_NOLEGACY;
        rid[1].hwndTarget = 0;

        if (RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE)
        {
            LOG_ERR("HIDWindowListener > Failed to register raw HIDs.");
        }
    }
}