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
    {
        // set global pointer, so static wndproc can call this class instance, which should be a singlton, but we don't enforce that ...
        g_pWindowListener = this;

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
        // Call the original wndproc setup by our graphics window manager class instance.
        return CallWindowProc(m_origWndProc, hWnd, message, wParam, lParam);
    }
}