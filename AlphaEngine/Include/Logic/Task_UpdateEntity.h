#ifndef ALPHA_TASK_UPDATE_ENTITY_H
#define ALPHA_TASK_UPDATE_ENTITY_H

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

#include <functional>
#include <memory>

#include "Threading/ATask.h"

namespace alpha
{
    class AEvent;
    class Entity;

    class Task_UpdateEntity : public ATask
    {
    public:
        Task_UpdateEntity(float fCurrentTime, float fElapsedTime, std::shared_ptr<Entity> pEntity, std::function<void(AEvent *)> delPublishEvent);
        bool VExecute();

    private:
        float m_fCurrentTime;
        float m_fElapsedTime;
        std::shared_ptr<Entity> m_pEntity;
        std::function<void(AEvent *)> m_delPublishEvent;
    };
}

#endif // ALPHA_TASK_UPDATE_ENTITY_H