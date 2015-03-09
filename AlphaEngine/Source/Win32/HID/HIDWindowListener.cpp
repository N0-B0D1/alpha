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
#include "HID/HIDPlatformTranslator.h"
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

    HIDWindowListener::HIDWindowListener(EventDataPublisher<EventData_HIDKeyAction> & pubHIDKeyAction)
        : m_origWndProc(nullptr)
        , m_mouseTracking(false)
        , m_pPlatformTranslator(nullptr)
        , m_pubHIDKeyAction(pubHIDKeyAction)
    {
        // set global pointer, so static wndproc can call this class instance, which should be a singlton, but we don't enforce that ...
        g_pWindowListener = this;
        RegisterRawHIDs();

        // hi-jack the current window message function, store old one so we can still call it.
        m_origWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWL_WNDPROC, (LONG)StaticInputWndProc);

        // set platform context, to translate to engine input codes
        m_pPlatformTranslator = new HIDPlatformTranslator();

        m_lastMousePosition = m_mousePosition;
    }

    HIDWindowListener::~HIDWindowListener()
    {
        // un-hi-jack WndProc
        SetWindowLongPtr(g_hWnd, GWL_WNDPROC, (LONG)m_origWndProc);
        g_pWindowListener = nullptr;
    }

    void HIDWindowListener::Update()
    {
        // compare current and last mouse positions
        // if changed, then send an action range event
        if (m_lastMousePosition.xRelativePos != m_mousePosition.xRelativePos || m_lastMousePosition.xAbsolutePos != m_mousePosition.xAbsolutePos)
        {
            // send x-axis action event
            HIDAction * pAction = m_pPlatformTranslator->TranslateMouseCode(MC_XAXIS);
            DispatchHIDActionAxisEvent(HID_MOUSE, *pAction, m_mousePosition.xRelativePos, m_mousePosition.xAbsolutePos);
        }
        if (m_lastMousePosition.yRelativePos != m_mousePosition.yRelativePos || m_lastMousePosition.yAbsolutePos != m_mousePosition.yAbsolutePos)
        {
            // send y-axis action event
            HIDAction * pAction = m_pPlatformTranslator->TranslateMouseCode(MC_YAXIS);
            DispatchHIDActionAxisEvent(HID_MOUSE, *pAction, m_mousePosition.yRelativePos, m_mousePosition.yAbsolutePos);
        }

        // update last mouse postition
        m_lastMousePosition = m_mousePosition;
    }

    LRESULT CALLBACK HIDWindowListener::InputWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        //int count;

        switch (message)
        {
        case WM_INPUT:
            this->WMInputHandler(hWnd, message, wParam, lParam);
            break;

        case WM_MOUSEMOVE:
            if (!m_mouseTracking)
            {
                m_mouseTracking = true;
                while (ShowCursor(false) >= 0);
            }

            // set the mouse positions absolute values
            m_mousePosition.xAbsolutePos = LOWORD(lParam);
            m_mousePosition.yAbsolutePos = HIWORD(lParam);

            break;

        case WM_NCMOUSELEAVE:
            m_mouseTracking = false;
            while (ShowCursor(true) < 0);
            break;
        }

        // Call the original wndproc setup by our graphics window manager class instance.
        return CallWindowProc(m_origWndProc, hWnd, message, wParam, lParam);
    }

    void HIDWindowListener::WMInputHandler(HWND /*hWnd*/, UINT /*message*/, WPARAM /*wParam*/, LPARAM lParam)
    {
        UINT size;
        //HRESULT hr;

        //TCHAR szTempOutput[256];
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));

        LPBYTE lpByte = new BYTE[size];
        if (lpByte == NULL)
        {
            return;
        }

        if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpByte, &size, sizeof(RAWINPUTHEADER)) != size)
        {
            LOG_WARN("WindowListener > Raw input did not return the correct size.");
        }

        RAWINPUT * raw = (RAWINPUT *)lpByte;

        if (raw->header.dwType == RIM_TYPEKEYBOARD)
        {
            // read keyboard input
            bool keyUp = raw->data.keyboard.Flags & RI_KEY_BREAK;
            unsigned short keyCode = raw->data.keyboard.VKey;
            HIDAction * pAction = m_pPlatformTranslator->TranslateKeyboardCode(keyCode);

            if (pAction != nullptr)
            {
                DispatchHIDActionKeyEvent(HID_KEYBOARD, *pAction, !keyUp);
            }
            else
            {
                LOG_WARN("Non-mapped input: ", keyCode);
            }
        }
        else if (raw->header.dwType == RIM_TYPEMOUSE)
        {
            // read mouse button input
            // for now only read left, middle, and right buttons
            unsigned short btnIndex = MC_LEFT; // skip x-axis and y-axis codes
            unsigned int rawIndex = RI_MOUSE_BUTTON_1_DOWN;
            while (rawIndex <= RI_MOUSE_BUTTON_5_UP)
            {
                HIDAction * pAction = m_pPlatformTranslator->TranslateMouseCode(btnIndex);

                // check for button pushed
                unsigned long btnCode = (raw->data.mouse.ulButtons & rawIndex);
                bool btnState = (btnCode == 0 ? false : true);
                if (btnState)
                {
                    // button was pushed
                    //m_pContextManager->MouseButtonDown(static_cast<MouseButtonCode>(btnIndex));
                    DispatchHIDActionKeyEvent(HID_MOUSE, *pAction, true);
                }

                // increment to next raw button id
                rawIndex = rawIndex << 1;

                // check for button released
                btnCode = (raw->data.mouse.ulButtons & rawIndex);
                btnState = (btnCode == 0 ? false : true);
                if (btnState)
                {
                    // this button was released
                    //m_pContextManager->MouseButtonUp(static_cast<MouseButtonCode>(btnIndex));
                    DispatchHIDActionKeyEvent(HID_MOUSE, *pAction, false);
                }

                // increment to next raw button id
                rawIndex = rawIndex << 1;
                ++btnIndex;
            }
        }

        // always store the mouse's most recent relative position
        m_mousePosition.xRelativePos = raw->data.mouse.lLastX;
        m_mousePosition.yRelativePos = raw->data.mouse.lLastY;
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

    void HIDWindowListener::DispatchHIDActionKeyEvent(HID device, const HIDAction & action, bool pressed)
    {
        std::shared_ptr<EventData_HIDKeyAction> event = std::make_shared<EventData_HIDKeyAction>(device, action, pressed);
        m_pubHIDKeyAction.Publish(event);
    }

    void HIDWindowListener::DispatchHIDActionAxisEvent(HID device, const HIDAction & action, long relative, float absolute)
    {
        std::shared_ptr<EventData_HIDKeyAction> event = std::make_shared<EventData_HIDKeyAction>(device, action, false, relative, absolute);
        m_pubHIDKeyAction.Publish(event);
    }
}
