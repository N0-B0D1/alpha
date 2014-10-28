#ifndef EVENT_DATA_RECEIVER_H
#define EVENT_DATA_RECEIVER_H

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

#include "Toolbox/ConcurrentQueue.h"

#include "AlphaEvent.h"

namespace alpha
{
	/** Base class for use in the Event Manage to hanlde generic event receivers */
	class EventReceiverBase
	{
		friend class EventManager;

	public:
		EventReceiverBase();
		EventReceiverBase(EventReceiverBase const & rhs);
		EventReceiverBase& operator=(EventReceiverBase const & rhs);

		bool operator==(EventReceiverBase const & rhs) const;

	protected:
		virtual void VSafeQueueEvent(EventPtr pEvent) = 0;

	private:
		unsigned long int m_id;
	};


	/** Templated variant of the Event Receiver which should be used in systems to connect to event streams */
	template <class EvDataType>
	class EventReceiver : public EventReceiverBase
	{
	public:
		EventReceiver()
		{
			// register as a receiver here?
		}
		virtual ~EventReceiver()
		{
			// remove self as a receiver here?
		}

		std::shared_ptr<EvDataType *> GetNextEvent()
		{
			std::shared_ptr<EventDataBase *> pEvData = nullptr;
			if (m_qEventData.TryPop(pEvData))
			{
				return std::dynamic_pointer_cast<EvDataType *>(pEvData);
			}

			return std::shared_ptr<EvDataType *>();
		}

	protected:
		virtual void VSafeQueueEvent(EventPtr pEvent)
		{
			if (pEvent)
			{
				m_qEventData.Push(pEvent->VCopy());
			}
		}

	private:
		ConcurrentQueue<EventPtr> m_qEventData;
	};
}

#endif // EVENT_DATA_RECEIVER_H