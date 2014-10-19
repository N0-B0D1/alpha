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

#include <iostream>

#include "Graphics/GraphicsSystem.h"

namespace alpha
{
    GraphicsSystem::GraphicsSystem() : AlphaSystem(30) { }
    GraphicsSystem::~GraphicsSystem() { }

    bool GraphicsSystem::VInitialize()
    {
        return true;
    }

    bool GraphicsSystem::VShutdown()
    {
        return true;
    }

    bool GraphicsSystem::VUpdate(double currentTime, double elapsedTime)
        //bool LogicSystem::VUpdate(double /*currentTime*/, double /*elapsedTime*/)
    {
        std::cout << "    Graphics " << currentTime << " [" << elapsedTime << "]" << std::endl;
        return true;
    }

    void GraphicsSystem::Render()
    {
        // do something cool
        std::cout << "    Graphics rendering" << std::endl;
    }
}