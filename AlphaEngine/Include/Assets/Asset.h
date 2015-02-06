#ifndef ASSET_H
#define ASSET_H

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

#include <sys/stat.h>
#include <string>
#include <vector>

namespace alpha
{
    class Asset
    {
    public:
        Asset(const char * path, struct stat fileStats);
        virtual ~Asset();

        std::vector<unsigned char> GetData();

    private:
        const char * m_pPath;
        struct stat m_fileStats;
        //char *m_pBuffer;
        std::vector<unsigned char> m_data;
    };
}

#endif // ASSET_H
