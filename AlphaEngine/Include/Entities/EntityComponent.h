#ifndef ENTITY_COMPONENT_H
#define ENTITY_COMPONENT_H

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
    class EntityComponent
    {
    public:
        virtual ~EntityComponent();

        virtual bool VUpdate(float fCurrentTime, float fElapsedTime) = 0;

        unsigned int GetID() const;
        virtual std::string VGetName() const = 0;

        static unsigned int GetIDFromName(const std::string & name);
    };
}

#endif // ENTITY_COMPONENT_H
