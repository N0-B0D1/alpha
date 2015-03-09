#ifndef ALPHA_HID_CONTEXT_MANAGER_H
#define ALPHA_HID_CONTEXT_MANAGER_H

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

#include <functional>
#include <string>

#include "HID/HIDTypes.h"

namespace alpha
{
    class HIDContext;

    class HIDContextManager
    {
    public:
        HIDContextManager();
        virtual ~HIDContextManager();

        void KeyboardButtonUp(const HIDAction * const action);
        void KeyboardButtonDown(const HIDAction * const action);

        void MouseButtonDown(const HIDAction * const action);
        void MouseButtonUp(const HIDAction * const action);
        void MouseMoved(const HIDAction * const action, long relative, float absolute);

        /** Bind delegate to action */
        void BindAction(std::string action, std::function<void ()> delegate);
        /** Bind delegate to action state */
        void BindState(std::string state, std::function<void (bool)> delegate);
        /** Bind delegate to action axis range */
        void BindRange(std::string range, std::function<void(long, float)> delegate);

    private:
        HIDContext * m_pActiveContext;
    };
}

#endif // ALPHA_HID_CONTEXT_MANAGER_H