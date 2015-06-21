#ifndef ALPHA_EVENT_INTERFACE_H
#define ALPHA_EVENT_INTERFACE_H

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

#include "Toolbox/ConcurrentQueue.h"

namespace alpha
{
    class EventManager;
    class AEvent;

    /**
     * EventInterface
     * Defines an interface for interacting with the EventManger, allowing the
     * implementer to publish events and listen for events.
     */
    class EventInterface
    {
        friend class EventManager;

    public:
        virtual ~EventInterface();

        /** Publish an event for consumption by other systems. */
        void PublishEvent(AEvent * pEvent);
        /** Pull the next event off of the incoming event queue. */
        AEvent * GetNextEvent();

    private:
        /** Queue of incoming events from other engine systems */
        ConcurrentQueue<AEvent *> m_qIncomingEvents;
        /** Queue of outgoing events, these will be processed by the event manager */
        ConcurrentQueue<AEvent *> m_qOutgoingEvents;
    };
}

#endif // ALPHA_EVENT_INTERFACE_H
