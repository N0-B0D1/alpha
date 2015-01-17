/**
Copyright 2014 Jason R. Wendlandt

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

#include <cstring>
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    Asset::Asset(const char * path, struct stat fileStats)
        : m_pPath(path)
        , m_fileStats(fileStats)
    { }
    Asset::~Asset() { }

    std::vector<unsigned char> Asset::GetData()
    {
        if (m_data.size() == 0)
        {
            LOG("Loading file into memory: ", m_pPath);

            FILE * fp = fopen(m_pPath, "rb");
            if (fp)
            {
                LOG("File handler opened, attempting to load.");
                size_t bufSize = (sizeof(char) * m_fileStats.st_size) + 1;
                m_data.resize(bufSize);

                if (m_data.size() == bufSize)
                {
                    LOG("File opened, loading into memory.");
                    fread(&m_data[0], 1, bufSize, fp);
                }
                fclose(fp);
            }
            else
            {
                LOG_ERR("Failed to open file handler...");
            }
        }
        return m_data;
    }
}
