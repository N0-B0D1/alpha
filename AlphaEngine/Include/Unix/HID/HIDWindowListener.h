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

#include <GLFW/glfw3.h>

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
        HIDWindowListener(std::function<void(HID, const HIDAction &, bool)> dispatchHIDActionKey, std::function<void(HID, const HIDAction &, long, float)> dispatchHIDActionAxis);
        virtual ~HIDWindowListener();

        bool Update();

        void GLFWMouseKeyCallback(GLFWwindow * window, int button, int action, int mods);
        void GLFWMouseScrollCallback(GLFWwindow * window, double xoffset, double yoffset);
        void GLFWMousePositionCallback(GLFWwindow * window, double xpos, double ypos);

    private:
        // non-copyable
        HIDWindowListener(const HIDWindowListener&);
        HIDWindowListener & operator=(const HIDWindowListener&);

        /** Helper for dispatching HID Action key up/down events */
        std::function<void(HID, const HIDAction &, bool)> m_fDispatchHIDActionKey;
        /** Helper for dispatching HID Action axis range events */
        std::function<void(HID, const HIDAction &, long, float)> m_fDispatchHIDActionAxis;

        /** Track the current mouse absolute and relative position */
        MousePosition m_mousePosition;
        /** Track the position of the mouse, since the last update, campare against current to determine if event should be published */
        MousePosition m_lastMousePosition;
        /** Platform translator handles translation from platform code to engine code */
        HIDPlatformTranslator * m_pPlatformTranslator;
    };
}

#endif // ALPHA_HID_WINDOW_LISTENER_H
