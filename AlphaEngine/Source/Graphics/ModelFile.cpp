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
#include <string.h>

#include "Graphics/ModelFile.h"
#include "Graphics/Model.h"
#include "Graphics/Mesh.h"
#include "Assets/Asset.h"

namespace alpha
{
    Model * LoadModelFromAsset(std::shared_ptr<Asset> pAsset)
    {
        std::vector<unsigned char> data = pAsset->GetData();
        if (data.size() > 0)
        {
            char * buffer = reinterpret_cast<char *>(&data[0]);
            //unsigned char * buffer = &data[0];
            //std::stringstream str(buffer, std::ios::in | std::ios::binary);
            std::stringstream str;
            str.write(buffer, data.size());
            return DeserializeModel(str);
        }
        return nullptr;
    }

    Model * DeserializeModel(std::istream & stream)
    {
        ModelFileHeader header;
        stream.read(reinterpret_cast<char *>(&header), sizeof(ModelFileHeader));

        if (strcmp(header.signature, MODELFILE_SIG) == 0 && header.version == MODELFILE_V1)
        {
            return Model::Deserialize(stream);
        }
        return nullptr;
    }

    void SerializeModel(const Model * const pModel, std::ostream & stream)
    {
        ModelFileHeader header;
        sprintf(header.signature, "%s", MODELFILE_SIG);
        header.version = MODELFILE_V1;

        // add header data to stream
        stream.write(reinterpret_cast<char *>(&header), sizeof(ModelFileHeader));

        // let the model serialize itself OO style
        pModel->Serialize(stream);
    }
}
