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

#include <memory>
#include <map>

#include "Scripting/LuaScript.h"
#include "Scripting/LuaVar.h"
#include "Entities/EntityScript.h"

namespace alpha
{
    EntityScript::EntityScript(std::shared_ptr<Asset> asset)
    {
        this->Add(asset);
        this->Load();
        this->Run();

        // once the script environment is prepared, load and store the components table.
        m_components = this->GetGlobalTable("components");
    }
    EntityScript::~EntityScript() { }

    /**
     * Checks to see if the given component name is present in the components LuaTable loaded from the entity script.
     */
    bool EntityScript::HasComponent(const std::string & name)
    {
        auto search = m_components->Get(name);
        if (search != nullptr)
        {
            return true;
        }
        return false;
    }

    //! Get a list of the components specified by the script.
    const std::map<std::string, std::shared_ptr<LuaVar> > & EntityScript::GetComponentVars()
    {
        return m_components->GetAll();
    }
}
