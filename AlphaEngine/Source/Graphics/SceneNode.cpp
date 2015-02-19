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
    SceneNode::SceneNode(std::shared_ptr<SceneNode> pParent, std::shared_ptr<SceneComponent> component)
        : m_parent(pParent)
        , m_pSceneComponent(component)
        , m_pRenderData(nullptr)
    { }
    SceneNode::~SceneNode()
    {
        if (m_pRenderData) { delete m_pRenderData; }
    }

    void SceneNode::SetParent(std::shared_ptr<SceneNode> pParent)
    {
        m_parent = pParent;
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
        }

        m_pRenderData->m_world = this->GetWorldTransform(); //m_pSceneComponent->GetTransform();
        /*
        if (m_pSceneComponent->IsDirty())
        {
            // update position
            m_pRenderData->SetPosition(m_pSceneComponent->GetPosition());
            m_pRenderData->SetScale(m_pSceneComponent->GetScale());
        }
        */

        return m_pRenderData;
    }

    void SceneNode::SetChildren(std::map<unsigned int, std::shared_ptr<SceneNode> > children)
    {
        m_children = children;
    }

    std::map<unsigned int, std::shared_ptr<SceneNode> > SceneNode::GetChildren() const
    {
        return this->m_children;
    }

    Matrix SceneNode::GetWorldTransform() const
    {
        if (m_parent != nullptr)
        {
            return m_pSceneComponent->GetTransform() * m_parent->GetWorldTransform();
        }
        return m_pSceneComponent->GetTransform();
    }
}
