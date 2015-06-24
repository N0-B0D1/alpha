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

#include "Logic/Task_UpdateEntity.h"
#include "Entities/Entity.h"
#include "Logic/LogicSystemEvents.h"

namespace alpha
{
    Task_UpdateEntity::Task_UpdateEntity(float fCurrentTime, float fElapsedTime, std::shared_ptr<Entity> pEntity, std::function<void(AEvent *)> delPublishEvent)
        : m_fCurrentTime(fCurrentTime)
        , m_fElapsedTime(fElapsedTime)
        , m_pEntity(pEntity)
        , m_delPublishEvent(delPublishEvent)
    { }

    bool Task_UpdateEntity::VExecute()
    {
        if (m_pEntity->Update(m_fCurrentTime, m_fElapsedTime))
        {
            this->m_delPublishEvent(new Event_EntityUpdated(m_pEntity));
        }
        return true;
    }
}