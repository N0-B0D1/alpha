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

namespace alpha
{
    /**
     * HIDWindowListener is a platform specific implementation that reads user input on the window.
     */
    class HIDWindowListener
    {
    public:
        HIDWindowListener();
        virtual ~HIDWindowListener();

        /** Windows message loop handler for human input. */
        LRESULT CALLBACK InputWndProc(HWND, UINT, WPARAM, LPARAM);

    private:
        WNDPROC m_origWndProc;
    };
}

#endif // ALPHA_HID_WINDOW_LISTENER_H