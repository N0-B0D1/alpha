#ifndef ALPHA_MODEL_H
#define ALPHA_MODEL_H

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

#include "Graphics/RenderSet.h"

namespace alpha
{
    class Mesh;

    class Model : public RenderSet
    {
    public:
        explicit Model(std::vector<Renderable *> meshes);
        virtual ~Model();

        std::vector<Renderable *> GetRenderables();

        //std::vector<Mesh> GetMeshes() const;

        /** Write model data out to stream */
        void Serialize(std::ostream & stream) const;
        /** Read model data from stream, create a return a new model */
        static Model * Deserialize(std::istream & stream);

    private:
        std::vector<Renderable *> m_meshes;
    };
}

#endif // ALPHA_MODEL_H
