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
        LOG("getting asset data");
        if (!m_pBuffer)
        {
            /*
            long fsize = 0;
            size_t file_size = 0;

            // open the file
            FILE *file_pointer;
            //file_pointer = fopen(file_path.c_str(), "r");

            if (!fopen_s(&file_pointer, file_path.c_str(), "rb"))
            {
                // get the file size
                fseek(file_pointer, 0, SEEK_END);
                fsize = ftell(file_pointer);
                rewind(file_pointer);

                // allocate memory for the file in the given buffer
                *buffer = (char *)malloc(sizeof(char) * fsize);
                if (*buffer == NULL) assert(false && "Out of memory, unable to read asset file.");

                // copy file contents to buffer
                file_size = fread(*buffer, 1, fsize, file_pointer);
                if (file_size != static_cast<size_t>(fsize)) assert(false && "An error occured while reading the asset file contents.");

                // close the file
                fclose(file_pointer);
            }

            return file_size;
            */

            FILE *fp;

            //if (!fopen_s(&fp, m_pPath, "rb"))
            LOG("opening file:");
            LOG(m_pPath);
            fp = fopen(m_pPath, "rb");
            if (!fp)
            {
                LOG("File opened successfully");

                //size_t result;
                size_t bufSize = (sizeof(char) * m_fileStats.st_size) + 1;
                m_pBuffer = new char[m_fileStats.st_size + 1];
                memset(m_pBuffer, 0, bufSize);
                //memset(m_pBuffer, 0, sizeof(*m_pBuffer));

                if (m_pBuffer != nullptr)
                {
                    //result = fread(m_pBuffer, 1, m_fileStats.st_size, fp);
                    //fread(m_pBuffer, 1, m_fileStats.st_size, fp);
                    fread(m_pBuffer, 1, bufSize, fp);
                }
                fclose(fp);
            }
        }
        return m_pBuffer;
    }
}
