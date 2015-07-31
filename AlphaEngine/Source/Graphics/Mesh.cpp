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
#include <sstream>

#include "Graphics/Mesh.h"
#include "Math/Constants.h"

#include "Toolbox/Logger.h"

namespace alpha
{
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
        : Renderable(vertices, indices)
    { }

    std::vector<Vertex> Mesh::GetVertices() const
    {
        return vertices;
    }

    std::vector<unsigned int> Mesh::GetIndices() const
    {
        return indices;
    }

    void Mesh::Serialize(std::ostream & stream) const
    {
        // output number of vertices
        char vertSizeBuf[UINT_LENGTH] = { 0 };
        sprintf(vertSizeBuf, "%d", (unsigned int)vertices.size());
        stream.write(vertSizeBuf, UINT_LENGTH);

        // then output the actual vertex data one at a time
        for (auto vertex : vertices)
        {
            stream.write(reinterpret_cast<char *>(&vertex), sizeof(Vertex));
        }

        // output the number of indices
        char indSizeBuf[UINT_LENGTH] = { 0 };
        sprintf(indSizeBuf, "%d", (unsigned int)indices.size());
        stream.write(indSizeBuf, UINT_LENGTH);

        // then output the actual indices list
        char indBuf[UINT_LENGTH] = { 0 };
        for (auto index : indices)
        {
            sprintf(indBuf, "%d", index);
            stream.write(indBuf, UINT_LENGTH);
        }
    }

    Mesh * Mesh::Deserialize(std::istream & stream)
    {
        // get the number of vertices to make
        unsigned int numVerts = 0;
        char vertBuf[UINT_LENGTH] = { 0 };
        stream.read(vertBuf, UINT_LENGTH);
        std::stringstream vertStr(vertBuf);
        vertStr >> numVerts;

        // make Vertex's up to numVerts
        std::vector<Vertex> vertices;
        for (unsigned int i = 0; i < numVerts; ++i)
        {
            Vertex v;
            stream.read(reinterpret_cast<char *>(&v), sizeof(Vertex));
            vertices.push_back(v);
            //LOG_ERR("vertex normal: ", v.normal.x, ", ", v.normal.y, ", ", v.normal.z);
        }

        // get the number of indices to pull from stream
        unsigned int numIndices = 0;
        char indBuf[UINT_LENGTH] = { 0 };
        stream.read(indBuf, UINT_LENGTH);
        std::stringstream index_size_stream(indBuf);
        index_size_stream >> numIndices;

        // create indices list up to numIndices
        std::vector<unsigned int> indices;
        for (unsigned int i = 0; i < numIndices; ++i)
        {
            unsigned int ind;
            stream.read(indBuf, UINT_LENGTH);
            std::stringstream index_stream(indBuf);
            index_stream >> ind;
            indices.push_back(ind);
        }

        // make and return the new mesh
        return new Mesh(vertices, indices);
    }
}
