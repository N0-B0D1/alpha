#ifndef ALPHA_HID_WINDOW_LISTENER_H
#define ALPHA_HID_WINDOW_LISTENER_H

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

#include "HID/HIDTypes.h"

namespace alpha
{
    class HIDPlatformTranslator;
    struct MousePosition;

    /**
     * HIDWindowListener is a platform specific implementation that reads user input on the window.
     */
    class HIDWindowListener
    {
    public:
        explicit HIDWindowListener(std::function<void(HID, const HIDAction &, bool)> dispatchHIDActionKey, std::function<void(HID, const HIDAction &, long, float)> dispatchHIDActionAxis);
        virtual ~HIDWindowListener();

        void Update();

        /** Windows message loop handler for human input. */
        LRESULT CALLBACK InputWndProc(HWND, UINT, WPARAM, LPARAM);

    private:
        // non-copyable
        HIDWindowListener(const HIDWindowListener&);
        HIDWindowListener & operator=(const HIDWindowListener&);

        /** Helper function to handle WM_INPUT message. */
        void WMInputHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        /** Convert raw keyboard key codes as needed. */
        unsigned short ConvertKeyCode(RAWKEYBOARD kbRaw);

        /** Register raw input devices, so we can query their state in our WndProc */
        void RegisterRawHIDs();

        /** Helper for dispatching HID Action key up/down events */
        std::function<void(HID, const HIDAction &, bool)> m_fDispatchHIDActionKey;
        /** Helper for dispatching HID Action axis range events */
        std::function<void(HID, const HIDAction &, long, float)> m_fDispatchHIDActionAxis;

        /** Handle to original WndProc, allowing it to run, and be restored when this instance destructs */
        WNDPROC m_origWndProc;
        /** Track mouse inside/outside of window */
        bool m_mouseTracking;
        /** Track the current mouse absolute and relative position */
        MousePosition m_mousePosition;
        /** Track the position of the mouse, since the last update, campare against current to determine if event should be published */
        MousePosition m_lastMousePosition;
        /** Platform translator handles translation from platform code to engine code */
        HIDPlatformTranslator * m_pPlatformTranslator;
    };
}

#endif // ALPHA_HID_WINDOW_LISTENER_H