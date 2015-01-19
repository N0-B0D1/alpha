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

#include "Scripting/LuaVar.h"

namespace alpha
{
    LuaTable::LuaTable(const std::string & name)
        : LuaVar(name)
    { }
    LuaTable::~LuaTable() { }

    LUA_VARTYPE LuaTable::GetVarType() const { return VT_TABLE; }

    void LuaTable::Push(std::string key, std::shared_ptr<LuaVar> value)
    {
        m_vars[key] = value;
    }

    /**
     * Find the LuaVar with the given key, if it exists (shallow search)
     */
    std::shared_ptr<LuaVar> LuaTable::Get(const std::string & key)
    {
        auto search = m_vars.find(key);
        if (search != m_vars.end())
        {
            return search->second;
        }
        return nullptr;
    }
}