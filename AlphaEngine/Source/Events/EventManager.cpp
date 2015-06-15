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

#include "Events/EventManager.h"
#include "Events/EventInterface.h"
#include "Events/Event.h"

namespace alpha
{
    EventManager::~EventManager() { }


        
    bool EventManager::Initialize()
    {
        return true;
    }

    bool EventManager::Update()
    {
        return true;
    }

    bool EventManager::Shutdown()
    {
        return true;
    }

    /** Register an interface so that it can receive events. */
    void EventManager::RegisterEventInterface(EventInterface * const pEventInterface)
    {

    }
    
    /** Remove the event interface from the manager so that it no longer recieves events */
    void EventManager::UnregisterEventInterface(EventInterface * const pEventInterface)
    {

    }
}
