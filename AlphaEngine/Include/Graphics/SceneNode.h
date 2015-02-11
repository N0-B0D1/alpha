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
#include "Math/Transform.h"

namespace alpha
{
    class SceneNode
    {
    public:
        SceneNode();
        virtual ~SceneNode();

    private:
        /** Pointer to this nodes parent node, nullptr if this is the root node. */
        std::shared_ptr<SceneNode> m_parent;

        /** A list of children nodes that belong to this scene node. */
        std::vector<std::shared_ptr<SceneNode> > m_children;

        /** The transform that represents this scene nodes position, scale, and rotation in the world. */
        Transform m_transform;
    };
}

#endif // ALPHA_SCENE_NODE_H
