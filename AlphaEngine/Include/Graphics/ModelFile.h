#ifndef ALPHA_MODEL_FILE_H
#define ALPHA_MODEL_FILE_H

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

//#include "Graphics/Model.h"
//#include "Graphics/Mesh.h"

namespace alpha
{
    class Asset;
    class Model;
    struct Vertex;

#define MODELFILE_SIG "Alpha-Model" // AM - Alpha Model
#define MODELFILE_V1 0x01

    struct ModelFileHeader
    {
        char signature[12]; // = MODELFILE_SIG;
        unsigned short version;
    };

    /** Load an asset as a ModelFile and convert to an engine Model */
    Model * LoadModelFromAsset(std::shared_ptr<Asset> pAsset);
    Model * DeserializeModel(std::istream & stream);
    /** Convert and engine Model into a ModelFile that can be written to disk. */
    void SerializeModel(const Model * const pModel, std::ostream & stream);
}

#endif // ALPHA_MODEL_FILE_H