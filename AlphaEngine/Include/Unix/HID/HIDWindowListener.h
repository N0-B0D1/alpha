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

#include "Events/EventDataPublisher.h"
#include "Events/EventData_HIDKeyAction.h"

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
        HIDWindowListener(EventDataPublisher<EventData_HIDKeyAction> & pubHIDKeyAction);
        virtual ~HIDWindowListener();

        void Update();

        void GLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

    private:
        // non-copyable
        HIDWindowListener(const HIDWindowListener&);
        HIDWindowListener & operator=(const HIDWindowListener&);

        /** Helper for dispatching HID Action key up/down events */
        void DispatchHIDActionKeyEvent(HID device, const HIDAction & action, bool pressed);
        /** Helper for dispatching HID Action axis range events */
        void DispatchHIDActionAxisEvent(HID device, const HIDAction & action, long relative, float absolute);

        /** Track the current mouse absolute and relative position */
        MousePosition m_mousePosition;
        /** Track the position of the mouse, since the last update, campare against current to determine if event should be published */
        MousePosition m_lastMousePosition;
        /** Platform translator handles translation from platform code to engine code */
        HIDPlatformTranslator * m_pPlatformTranslator;
        /** Handle to the HID Key Action publisher */
        EventDataPublisher<EventData_HIDKeyAction> & m_pubHIDKeyAction;
    };
}

#endif // ALPHA_HID_WINDOW_LISTENER_H
