#ifndef ALPHA_RENDER_SET_H
#define ALPHA_RENDER_SET_H

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

#include <vector>

#include "Math/Matrix.h"

namespace alpha
{
    class Renderable;

    /**
     * A RenderSet represents a set of Renderables.
     * e.g.: A Model is a RenderSet which represents a set of Renderable Mesh objects.
     */
    class RenderSet
    {
    public:
        explicit RenderSet(std::string psEntryPoint = "PS");
        virtual ~RenderSet();

        virtual std::vector<Renderable *> GetRenderables() = 0;

        /** The pixel shader entry point */
        std::string m_psEntryPoint;

        Matrix worldTransform;
    };
}

#endif // ALPHA_RENDER_SET_H