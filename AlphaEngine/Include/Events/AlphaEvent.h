#ifndef EVENT_DATA_H
#define EVENT_DATA_H

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

#include <memory>

namespace alpha
{
	// EventType is a static, globally unique, identifier 
	// used to differentiate Events in the system
	//
	// eg: static const EventType sk_EventType = 0x6cd8186f;
	//
	// TODO: See if a randomly generated UUID would work here
	typedef unsigned long EventType;

	class AlphaEvent
	{
	public:
		explicit AlphaEvent(const float fTimeStamp = 0.0f);
		virtual ~AlphaEvent();

		virtual AlphaEvent *VCopy() const = 0;

		virtual const EventType&	VGetEventType() const = 0;
		virtual float				VGetTimeStamp() const;
		virtual const char *		VGetName() const = 0;

	private:
		// non-assignable
		AlphaEvent & operator=(const AlphaEvent &);

		const float m_fTimeStamp;
	};


	// The rest of the engine will deal with shared pointer AlphaEvent * objects
	typedef std::shared_ptr<AlphaEvent *> EventPtr;
}

#endif // EVENT_DATA_H