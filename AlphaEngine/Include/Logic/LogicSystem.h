#ifndef LOGIC_SYSTEM_H
#define LOGIC_SYSTEM_H

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

#include "AlphaSystem.h"

namespace alpha
{
    class LogicSystem : public AlphaSystem
    {
    public:
        LogicSystem();
        virtual ~LogicSystem();

        virtual bool VInitialize();
        virtual bool VShutdown();

    private:
        virtual bool VUpdate(double currentTime, double elapsedTime);
    };
}

#endif // LOGIC_SYSTEM_H