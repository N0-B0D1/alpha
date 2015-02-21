#ifndef ALPHA_RENDER_DATA_TASK_H
#define ALPHA_RENDER_DATA_TASK_H

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

#include <map>

#include "Threading/Task.h"

namespace alpha
{
    class SceneNode;

    /**
     * \brief RenderDataTask handles updating the logic data for a single RenderData object.
     */
    class RenderDataTask : public Task
    {
    public:
        explicit RenderDataTask(std::map<unsigned int, SceneNode *> nodes);

        /** Handle all logic to completion */
        virtual void VExecute();

    private:
        void UpdateNodes(std::map<unsigned int, SceneNode *> nodes);

        std::map<unsigned int, SceneNode *> m_nodes;
    };
}

#endif // ALPHA_RENDER_DATA_TASK_H
