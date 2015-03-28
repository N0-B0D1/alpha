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

namespace alpha
{
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
        : m_vertices(vertices)
        , m_indices(indices)
    { }

    std::vector<Vertex> Mesh::GetVertices() const
    {
        return m_vertices;
    }

    std::vector<unsigned int> Mesh::GetIndices() const
    {
        return m_indices;
    }

    void Mesh::Serialize(std::ostream & stream) const
    {
        // output number of vertices
        char * vertSizeBuf = new char[sizeof(unsigned int)];
        sprintf(vertSizeBuf, "%d", (unsigned int)m_vertices.size());
        stream.write(vertSizeBuf, sizeof(unsigned int));

        // then output the actual vertex data one at a time
        for (auto vertex : m_vertices)
        {
            stream.write(reinterpret_cast<char *>(&vertex), sizeof(Vertex));
        }

        // output the number of indices
        char * indSizeBuf = new char[sizeof(unsigned int)];
        sprintf(indSizeBuf, "%d", (unsigned int)m_indices.size());
        stream.write(indSizeBuf, sizeof(unsigned int));

        // then output the actual indices list
        char * indBuf = new char[sizeof(unsigned int)];
        for (auto index : m_indices)
        {
            sprintf(indBuf, "%d", index);
            stream.write(indBuf, sizeof(unsigned int));
        }
    }

    Mesh Mesh::Deserialize(std::istream & stream)
    {
        // get the number of vertices to make
        unsigned int numVerts;
        char * vertBuf = new char[sizeof(unsigned int)];
        stream.read(vertBuf, sizeof(unsigned int));
        std::stringstream vertStr(vertBuf);
        vertStr >> numVerts;

        // make Vertex's up to numVerts
        std::vector<Vertex> vertices;
        for (unsigned int i = 0; i < numVerts; ++i)
        {
            Vertex v;
            stream.read(reinterpret_cast<char *>(&v), sizeof(Vertex));
            vertices.push_back(v);
        }

        // get the number of indices to pull from stream
        unsigned int numIndices;
        char * indBuf = new char[sizeof(unsigned int)];
        stream.read(indBuf, sizeof(unsigned int));
        std::stringstream index_size_stream(indBuf);
        index_size_stream >> numIndices;

        // create indices list up to numIndices
        std::vector<unsigned int> indices;
        for (unsigned int i = 0; i < numIndices; ++i)
        {
            unsigned int ind;
            stream.read(indBuf, sizeof(unsigned int));
            std::stringstream index_stream(indBuf);
            index_stream >> ind;
            indices.push_back(ind);
        }

        // make and return the new mesh
        return Mesh(vertices, indices);
    }
}
