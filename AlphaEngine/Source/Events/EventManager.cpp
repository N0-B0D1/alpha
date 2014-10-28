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

#include "Events/EventManager.h"

namespace alpha
{
	EventManager::EventManager() { }
	EventManager::~EventManager() { }

	bool EventManager::Initialize()
    {
        return true;
	}

	bool EventManager::Update(double /*currentTime*/, double /*elapsedTime*/)
	{
		return true;
	}

	bool EventManager::Shutdown()
    {
        return true;
	}

	bool EventManager::AddReceiver(EventReceiverBase *pReceiver, const EventType & evType)
	{
		ReceiverFor newReceiver;
		newReceiver.m_pReceiver = pReceiver;
		newReceiver.m_evType = evType;

		m_newReceiverQueue.Push(newReceiver);

		return true;
	}

	bool EventManager::RemoveReceiver(EventReceiverBase *pReceiver, const EventType & evType)
	{
		ReceiverFor oldReceiver;
		oldReceiver.m_pReceiver = pReceiver;
		oldReceiver.m_evType = evType;

		m_removeReceiverQueue.Push(oldReceiver);

		return false;
	}

	bool EventManager::QueueEvent(EventPtr pEvent)
	{
		m_eventQueue.Push(pEvent);
		return true;
	}

	int EventManager::AddNewReceivers()
	{
		int processed = 0;

		ReceiverFor newReceiverFor;
		while (m_newReceiverQueue.TryPop(newReceiverFor))
		{
			bool exists = false;

			// retrieve the list of receivers for the given event type
			// if it doesn't exist, this will also create the new mapping
			EventReceiverList &eventListeners = m_eventReceivers[newReceiverFor.m_evType];
			for (EventReceiverList::iterator it = eventListeners.begin(); it != eventListeners.end(); ++it)
			{
				EventReceiverBase *pReceiver = (*it);
				if (pReceiver == newReceiverFor.m_pReceiver)
				{
					exists = true;
					break; // event receiver already assigned, was this erroneously added again?
				}
			}

			if (!exists)
			{
				// make new pair for listener/delegate
				eventListeners.push_back(newReceiverFor.m_pReceiver);
			}

			++processed;
		}

		return processed;
	}

	int EventManager::RemoveOldReceivers()
	{
		int processed = 0;

		ReceiverFor oldReceiverFor;
		while (m_removeReceiverQueue.TryPop(oldReceiverFor))
		{
			EventReceiverMap::iterator eventIt = m_eventReceivers.find(oldReceiverFor.m_evType);
			if (eventIt != m_eventReceivers.end())
			{
				EventReceiverList &evReceivers = eventIt->second;
				for (EventReceiverList::iterator it = evReceivers.begin(); it != evReceivers.end(); ++it)
				{
					EventReceiverBase *pReceiver = (*it);
					if (pReceiver == oldReceiverFor.m_pReceiver)
					{
						evReceivers.erase(it);
						break; // only remove one, there should only be one anyway.
					}
				}
			}

			++processed;
		}

		return processed;
	}

	int EventManager::QueueNewEvents()
	{
		int processed = 0;

		EventPtr pEvent;
		while (m_newEventQueue.TryPop(pEvent))
		{
			m_eventQueue.Push(pEvent);
			++processed;
		}

		return processed;
	}
}