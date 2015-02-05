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

#include <string>
#include "Events/EventData.h"

namespace alpha
{
    EventData::~EventData() { }

    unsigned int EventData::GetTypeID() const
    {
        return GetIDFromName(this->VGetTypeName());
    }

    unsigned int EventData::GetIDFromName(const std::string & name)
    {
        static std::hash<std::string> string_hash;
        return string_hash(name);
    }
}
