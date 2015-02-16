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

#include "Graphics/SceneNode.h"
#include "Graphics/RenderData.h"
#include "Entities/EntityComponent.h"
#include "Math/Matrix.h"
#include "Math/Vector3.h"

namespace alpha
{
    SceneNode::SceneNode(std::shared_ptr<SceneComponent> component, std::map<unsigned int, std::shared_ptr<SceneNode> > children)
        : m_parent(nullptr)
        , m_children(children)
        , m_pSceneComponent(component)
        , m_pRenderData(nullptr)
    { }
    SceneNode::~SceneNode()
    {
        if (m_pRenderData) { delete m_pRenderData; }
    }

    bool SceneNode::IsRenderable() const
    {
        // is renderable if it contains a valid scene component
        return m_pSceneComponent != nullptr;
    }

    RenderData * SceneNode::GetRenderData()
    {
        if (m_pRenderData == nullptr)
        {
            m_pRenderData = new RenderData();
            
            // set world position
            XMMATRIX m = XMMatrixTranslationFromVector(XMLoadFloat3(&m_pSceneComponent->GetPosition()));
            Vector3 scale = m_pSceneComponent->GetScale();
            XMMATRIX mscale = XMMatrixScaling(scale.x, scale.y, scale.z);
            m = mscale * m;
            //XMMatrixPosition
            XMStoreFloat4x4(&m_pRenderData->m_world, m);
        }
        return m_pRenderData;
    }

    std::map<unsigned int, std::shared_ptr<SceneNode> > SceneNode::GetChildren() const
    {
        return this->m_children;
    }
}
