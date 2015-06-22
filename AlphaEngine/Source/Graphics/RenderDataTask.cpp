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

#include "Graphics/RenderDataTask.h"
#include "Graphics/SceneNode.h"
#include "Graphics/RenderSet.h"

namespace alpha
{
    RenderDataTask::RenderDataTask(std::map<unsigned int, SceneNode *> nodes)
        : m_nodes(nodes)
    { }

    bool RenderDataTask::VExecute()
    {
        this->UpdateNodes(m_nodes);
        return true;
    }

    void RenderDataTask::UpdateNodes(std::map<unsigned int, SceneNode *> nodes)
    {
        for (auto pair : nodes)
        {
            RenderSet * rs = pair.second->GetRenderSet();
            rs->worldTransform = pair.second->GetWorldTransform();
            this->UpdateNodes(pair.second->GetChildren());
        }
    }
}
