#ifndef ALPHA_MESH_H
#define ALPHA_MESH_H

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

#include <fstream>
#include <vector>

#include "Graphics/Renderable.h"
#include "Math/Vector3.h"

namespace alpha
{
    class Mesh : public Renderable
    {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        std::vector<Vertex> GetVertices() const;
        std::vector<unsigned int> GetIndices() const;

        /** Write mesh data out to stream */
        void Serialize(std::ostream & stream) const;
        /** Read data from stream, create and return a new Mesh object */
        static Mesh * Deserialize(std::istream & stream);
    };
}

#endif // ALPHA_MESH_H
