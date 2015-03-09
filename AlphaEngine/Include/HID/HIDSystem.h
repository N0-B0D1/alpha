#ifndef ALPHA_HID_SYSTEM_H
#define ALPHA_HID_SYSTEM_H

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

#include "AlphaSystem.h"
#include "Events/EventDataSubscriber.h"
#include "Events/EventDataPublisher.h"
#include "Events/EventData_HIDKeyAction.h"

namespace alpha
{
    class HIDWindowListener;
    class HIDContextManager;

    class HIDSystem : public AlphaSystem
    {
    public:
        HIDSystem();
        virtual ~HIDSystem();

        virtual bool VInitialize();
        virtual bool VShutdown();

        /** Allow subscribing to HIDKeyAction events */
        void SubscribeToHIDKeyAction(std::shared_ptr<AEventDataSubscriber> pSubscriber);

    private:
        virtual bool VUpdate(double currentTime, double elapsedTime);

        /** Pointer to the platform specific window listener, that will forward user input on the game window to this class */
        HIDWindowListener * m_pWindowListener;

        /** Publisher for HID Key Action events */
        EventDataPublisher<EventData_HIDKeyAction> m_pubHIDKeyAction;
    };
}

#endif // ALPHA_HID_SYSTEM_H