#ifndef EVENT_DATA_H
#define EVENT_DATA_H

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

namespace alpha
{
    class EventData
    {
    public:
        virtual ~EventData();

        /** Get the hashed string ID for the derived EventData type. */
        unsigned int GetTypeID() const;
        /** Get the name that represents the EventData type, NOT the instance. */
        virtual std::string VGetTypeName() const = 0;
        /** Hashes the given string and returns the unsigned int representation. */
        static unsigned int GetIDFromName(const std::string & name);
    };
}

#endif // EVENT_DATA_H
