#ifndef ALPHA_SCENE_NODE_H
#define ALPHA_SCENE_NODE_H

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

#include <memory>
#include <vector>
#include "Entities/EntityComponent.h"
#include "Math/Matrix.h"

namespace alpha
{
    class RenderData;

    class SceneNode
    {
    public:
        SceneNode(std::shared_ptr<SceneNode> pParent, std::shared_ptr<SceneComponent> component);
        virtual ~SceneNode();

        /** Set this nodes parent node */
        void SetParent(std::shared_ptr<SceneNode> pParent);

        /** Check if this node is renderable, or just a spacer node. */
        bool IsRenderable() const;

        RenderData * GetRenderData();

        /** Attach children */
        void SetChildren(std::map<unsigned int, std::shared_ptr<SceneNode> > children);
        /** Retrieve this nodes child nodes */
        std::map<unsigned int, std::shared_ptr<SceneNode> > GetChildren() const;

        /** Build and return this nodes world transform */
        Matrix GetWorldTransform() const;

    private:
        /** Pointer to this nodes parent node, nullptr if this is the root node. */
        std::shared_ptr<SceneNode> m_parent;

        /** A list of children nodes that belong to this scene node. */
        std::map<unsigned int, std::shared_ptr<SceneNode> > m_children;

        /** The transform that represents this scene nodes position, scale, and rotation in the world. */
        Matrix m_world;

        /** A handle to the SceneComponent which this SceneNode represents */
        std::shared_ptr<SceneComponent> m_pSceneComponent;

        /** Store render data, and only update/destroy it as needed */
        RenderData * m_pRenderData;
    };
}

#endif // ALPHA_SCENE_NODE_H
