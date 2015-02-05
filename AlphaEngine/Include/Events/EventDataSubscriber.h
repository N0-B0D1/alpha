#ifndef EVENT_DATA_SUBSCRIBER_H
#define EVENT_DATA_SUBSCRIBER_H

/**
Copyright 2014 Jason R. Wendlandt

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

//#include "Events/EventData.h"
#include "Toolbox/uuid.h"

namespace alpha
{
    class EventData;

    /**
     * class AEventDataSubscriber
     *
     * Defines an abstract base class for EventDataSubscriber's.
     * Useful for implementing containers of event subscribers.
     */
    class AEventDataSubscriber
    {
    public:
        AEventDataSubscriber()
            : m_id(GenerateUUID())
        { }

        unsigned int GetID() const { return m_id; }
        virtual void VQueueEventData(EventData * pEvent) = 0;

    private:
        unsigned int m_id;
    };

    /**
     * class EventDataSubscriber
     *
     * Defines a templeted implementation of the AEventDataSubscriber abstract base class.
     * Used to implement a specific type of subscriber to subscribe to a specific event type.
     */
    template<class EventDataType>
    class EventDataSubscriber : public AEventDataSubscriber
    {
    public:
        virtual ~EventDataSubscriber() { }

        EventDataType * GetNextEvent()
        {
            EventData *pEventData = nullptr;
            m_eventData.TryPop(pEventData);
            if (pEventData)
            {
                return static_cast<EventDataType *>(pEventData);
            }
            return nullptr;
        }
        virtual void VQueueEventData(EventData * pEvent)
        {
            // XXX make a copy of the event data.
            m_eventData.Push(pEvent);
        }

    private:
        ConcurrentQueue<EventData *> m_eventData;
    };
}

#endif // EVENT_DATA_SUBSCRIBER_H