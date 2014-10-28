#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

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

#include <list>
#include <map>

#include "AlphaEvent.h"
#include "EventReceiver.h"

#include "Toolbox/ConcurrentQueue.h"

namespace alpha
{
    class EventManager
    {
    public:
		EventManager();
		virtual ~EventManager();

		bool Initialize();
		bool Update(double currentTime, double elapsedTime);
        bool Shutdown();

		bool AddReceiver(EventReceiverBase *pReceiver, const EventType & evType);
		bool RemoveReceiver(EventReceiverBase *pReceiver, const EventType & evType);

		bool QueueEvent(EventPtr pEvent);

	private:
		typedef std::list<EventReceiverBase *>			EventReceiverList;
		typedef std::map<EventType, EventReceiverList>	EventReceiverMap;
		typedef ConcurrentQueue<EventPtr>				EventQueue;

		EventReceiverMap m_eventReceivers;
		ConcurrentQueue<EventPtr> m_eventQueue;

		struct ReceiverFor
		{
			ReceiverFor() : m_pReceiver(0) { }
			EventReceiverBase *m_pReceiver;
			EventType m_evType;
		};

		ConcurrentQueue<EventPtr> m_newEventQueue;
		ConcurrentQueue<ReceiverFor> m_newReceiverQueue;
		ConcurrentQueue<ReceiverFor> m_removeReceiverQueue;

		int AddNewReceivers();
		int RemoveOldReceivers();
		int QueueNewEvents();
    };
}

#endif // EVENT_MANAGER_H