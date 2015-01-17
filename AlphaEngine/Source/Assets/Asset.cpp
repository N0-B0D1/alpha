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
        , m_pBuffer(nullptr)
    { }
    Asset::~Asset() { }

    char * Asset::GetData()
    {
        if (!m_pBuffer)
        {
            //if (!fopen_s(&fp, m_pPath, "rb"))
            LOG("Loading file into memory: ", m_pPath);

            FILE * fp = fopen(m_pPath, "rb");
            if (fp)
            {
                LOG("File handler opened, attempting to load.");
                //size_t result;
                size_t bufSize = (sizeof(char) * m_fileStats.st_size) + 1;
                //m_pBuffer = new char[m_fileStats.st_size + 1];
                m_pBuffer = (char *)malloc(bufSize);
                memset(m_pBuffer, 0, bufSize);
                //memset(m_pBuffer, 0, sizeof(*m_pBuffer));

                if (m_pBuffer != nullptr)
                {
                    LOG("File opened, loading into memory.");
                    //result = fread(m_pBuffer, 1, m_fileStats.st_size, fp);
                    //fread(m_pBuffer, 1, m_fileStats.st_size, fp);
                    fread(m_pBuffer, 1, bufSize, fp);
                }
                fclose(fp);
            }
            else
            {
                LOG_ERR("Failed to open file handler...");
            }
        }
        return m_pBuffer;
    }
}
