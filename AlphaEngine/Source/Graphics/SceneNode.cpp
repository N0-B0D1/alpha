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

#include <sstream>

#include "Graphics/SceneNode.h"
#include "Graphics/RenderSet.h"
#include "Graphics/Model.h"
#include "Graphics/ModelFile.h"
#include "Assets/Asset.h"
#include "Entities/EntityComponent.h"
#include "Math/Matrix.h"
#include "Math/Vector3.h"

namespace alpha
{
    SceneNode::SceneNode(SceneNode * pParent, std::shared_ptr<SceneComponent> component)
        : m_parent(pParent)
        , m_pSceneComponent(component)
        , m_pRenderSet(nullptr)
    { }
    SceneNode::~SceneNode()
    {
        // destroy children
        for (auto pair : m_children)
        {
            delete pair.second;
        }
        // destroy render data, so all gpu resources are released
        if (m_pRenderSet) { delete m_pRenderSet; }
    }

    void SceneNode::SetParent(SceneNode * pParent)
    {
        m_parent = pParent;
    }

    bool SceneNode::IsRenderable() const
    {
        // is renderable if it contains a valid scene component
        return m_pSceneComponent != nullptr;
    }

    RenderSet * SceneNode::GetRenderSet()
    {
        if (m_pRenderSet != nullptr)
        {
            m_pRenderSet->worldTransform = this->GetWorldTransform();
        }

        return m_pRenderSet;
    }

    void SceneNode::SetChildren(std::map<unsigned int, SceneNode *> children)
    {
        m_children = children;
    }

    std::map<unsigned int, SceneNode *> SceneNode::GetChildren() const
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

    void SceneNode::SetMesh(std::shared_ptr<Asset> pAsset)
    {
        m_pMeshAsset = pAsset;

        // set model if thise node has one
        if (m_pMeshAsset != nullptr)
        {
            // Create the render set for this scene node
            m_pRenderSet = LoadModelFromAsset(pAsset);
        }
    }
}
