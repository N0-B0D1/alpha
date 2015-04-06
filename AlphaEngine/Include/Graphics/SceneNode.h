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
    class Asset;
    class RenderSet;
    class Light;

    class SceneNode
    {
    public:
        SceneNode(SceneNode * pParent, std::shared_ptr<SceneComponent> component);
        virtual ~SceneNode();

        /** Set this nodes parent node */
        void SetParent(SceneNode * pParent);

        /** Check if this node is renderable, or just a spacer node. */
        bool IsRenderable() const;

        RenderSet * GetRenderSet();

        /** Attach children */
        void SetChildren(std::map<unsigned int, SceneNode *> children);
        /** Retrieve this nodes child nodes */
        std::map<unsigned int, SceneNode *> GetChildren() const;

        /** Build and return this nodes world transform */
        Matrix GetWorldTransform() const;

        /** Set the model mesh that should for which render data will be build for this node */
        void SetMesh(std::shared_ptr<Asset> pAsset);

        /** Set the light data for this node */
        void SetLight(Light * pLight);
        Light * GetLight() const;

    private:
        /** Pointer to this nodes parent node, nullptr if this is the root node. */
        SceneNode * m_parent;

        /** A list of children nodes that belong to this scene node. */
        std::map<unsigned int, SceneNode *> m_children;

        /** The transform that represents this scene nodes position, scale, and rotation in the world. */
        Matrix m_world;

        /** A handle to the SceneComponent which this SceneNode represents */
        std::shared_ptr<SceneComponent> m_pSceneComponent;

        /** The mesh model that this node will build render data for. */
        std::shared_ptr<Asset> m_pMeshAsset;

        /** Store render data, and only update/destroy it as needed */
        RenderSet * m_pRenderSet;

        /** Light information for this node */
        Light * m_pLight;
    };
}

#endif // ALPHA_SCENE_NODE_H
