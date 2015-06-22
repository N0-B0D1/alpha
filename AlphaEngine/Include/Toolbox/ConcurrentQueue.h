#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

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

#include <queue>
#include <mutex>

namespace alpha
{
	template<typename Data>
	class ConcurrentQueue
	{
	public:
		ConcurrentQueue() { }

		void Push(Data const &data)
		{
			std::lock_guard<std::mutex> guard(m_queueLock);
			m_queue.push(data);
		}
		bool Empty()
		{
			std::lock_guard<std::mutex> guard(m_queueLock);
			return m_queue.empty();
		}
		bool TryPop(Data &poppedValue)
		{
			std::lock_guard<std::mutex> guard(m_queueLock);
            if (m_queue.empty())
            {
                return false;
            }

			poppedValue = m_queue.front();
			m_queue.pop();
			return true;
		}

	private:
		// non-copyable
		ConcurrentQueue(const ConcurrentQueue&);
		ConcurrentQueue & operator=(const ConcurrentQueue&);

		std::queue<Data> m_queue;
		std::mutex m_queueLock;
	};
}

#endif // CONCURRENT_QUEUE_H
