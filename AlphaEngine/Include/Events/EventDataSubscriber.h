#ifndef EVENT_DATA_SUBSCRIBER_H
#define EVENT_DATA_SUBSCRIBER_H

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

#include <memory>

#include "Toolbox/uuid.h"
#include "Toolbox/ConcurrentQueue.h"

#include "Events/EventData.h"

namespace alpha
{
    //class EventData;

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

        /** Retrieve the unique identifier that represents this subscriber instance */
        unsigned int GetID() const { return m_id; }
        /** Publish a new event to this subscriber */
        virtual void VQueueEventData(std::shared_ptr<const EventData> pEvent) = 0;

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

        /** Retrieve the most recent event that has een published to this subscriber */
        std::shared_ptr<const EventDataType> GetNextEvent()
        {
            std::shared_ptr<const EventData> pEventData = nullptr;
            m_eventData.TryPop(pEventData);
            if (pEventData)
            {
                return std::dynamic_pointer_cast<const EventDataType>(pEventData);
            }
            return nullptr;
        }

        virtual void VQueueEventData(std::shared_ptr<const EventData> pEvent)
        {
            m_eventData.Push(pEvent);
        }

    private:
        ConcurrentQueue<std::shared_ptr<const EventData> > m_eventData;
    };
}

#endif // EVENT_DATA_SUBSCRIBER_H
