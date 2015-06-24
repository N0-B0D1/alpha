#ifndef ALPHA_SYSTEM_H
#define ALPHA_SYSTEM_H

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

#include <cstdint>
#include <functional>
#include <map>

namespace alpha
{
    class EventManager;
    class EventInterface;
    class AEvent;

    /**
     * The AlphaSystem represents a classic engine sub-system, such as Graphcs, AI, Physics, etc.
     * Or it might represent a set of game logic.
     */
    class AlphaSystem
    {
        friend class AlphaController;

    public:
        AlphaSystem(uint8_t hertz);
        virtual ~AlphaSystem();

        bool Initialize(EventManager * pEventManager);
        bool Update(double currentTime, double elapsedTime);
        bool Shutdown(EventManager * pEventManager);

    protected:
        /** Publish an event to the event interface to be sent to other systems. THREAD-SAFE */
        void PublishEvent(AEvent * pEvent);
        /** Register a function handler for an incoming event type */
        void AddEventHandler(unsigned int event_id, std::function<void(AEvent * const)> handler);
        /** Removed the handler for the incoming event type */
        void RemoveEventHandler(unsigned int event_id);

    private:
        // non-copyable
        AlphaSystem(const AlphaSystem&);
        AlphaSystem & operator=(const AlphaSystem&);

        virtual bool VInitialize() = 0;
        virtual bool VUpdate(double currentTime, double elapsedTime) = 0;
        virtual bool VShutdown() = 0;

        /** Handle any events recieved since the last update */
        void HandleEvents();

        /** update frequency */
        uint8_t m_hertz;
        double m_updateFrequency;
        double m_elapsedTime = 0.0f;

        /** Interface for receiving and publishing events */
        EventInterface * m_pEventInterface;
        /** Map event id to a function handler */
        std::map<unsigned int, std::function<void(AEvent * const)>> m_mEventHandlers;
    };
}

#endif // ALPHA_SYSTEM_H
