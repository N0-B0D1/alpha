#ifndef EVENT_DATA_PUBLISHER_H
#define EVENT_DATA_PUBLISHER_H

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

#include <map>
#include <memory>

#include "Events/EventDataSubscriber.h"
#include "Toolbox/ConcurrentQueue.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    class EventData;

    /**
     * class EventDataPublisher
     *
     * Maintains a list of EventDataSubscriber's.  When the owner of this publisher, publishes data, 
     * the publisher sends a copy of the data to each subscriber.
     */
    template<class EventDataType>
    class EventDataPublisher
    {
    public:
        virtual ~EventDataPublisher() { }

        /** Add a subscriber to be published to. */
        void Subscribe(std::shared_ptr<AEventDataSubscriber> pSubscriber)
        {
            auto search = m_subscribers.find(pSubscriber->GetID());
            if (search == m_subscribers.end())
            {
                m_subscribers[pSubscriber->GetID()] = pSubscriber;
            }
            else
            {
                LOG_WARN("Attempt to subscribe to a publisher that is already subscribed to!");
            }
        }

        /** Removes an existing subscriber, if it is already subscribed. */
        void Unsubscribe(std::shared_ptr<AEventDataSubscriber> pSubscriber)
        {
            auto search = m_subscribers.find(pSubscriber->GetID());
            if (search != m_subscribers.end())
            {
                m_subscribers.erase(search);
            }
            else
            {
                LOG_WARN("Attempt to unsubscribe from a publisher that was not previously subscribed to!");
            }
        }

        /** Publish an event to all subscribers */
        void Publish(std::shared_ptr<const EventDataType> pEvent)
        {
            // for each subscriber, copy the event, and pass copy to subscriber.
            // the subscriber is responsible for freeing the event data memory.
            for (auto pair : m_subscribers)
            {
                //EventDataType *copy = pEvent->VCopy();
                pair.second->VQueueEventData(pEvent);
            }
        }

    private:
        std::map<unsigned int, std::shared_ptr<AEventDataSubscriber> > m_subscribers;
    };
}

#endif // EVENT_DATA_PUBLISHER_H
