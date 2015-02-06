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
#include <string.h>
#include <vector>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "Scripting/LuaScript.h"
#include "Scripting/LuaVar.h"
#include "Assets/Asset.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    LuaScript::LuaScript()
        : m_pLuaState(luaL_newstate())
    {
        luaL_openlibs(m_pLuaState);
    }

    LuaScript::~LuaScript()
    {
        m_scriptAssets.empty();
        lua_close(m_pLuaState);
    }

    void LuaScript::Add(std::shared_ptr<Asset> asset)
    {
        m_scriptAssets.push_back(asset);
    }

    void LuaScript::Load()
    {
        // load each script into the lua state
        for (auto script : m_scriptAssets)
        {
            std::vector<unsigned char> data = script->GetData();
            if (data.size() > 0)
            {
                char * buffer = reinterpret_cast<char *>(&data[0]);
                LOG("Attempting to load buffer data into LUA environment");
                int error = luaL_loadbuffer(m_pLuaState, buffer, strlen(buffer), "line");
                if (error)
                {
                    LOG_ERR("LUA: ", lua_tostring(m_pLuaState, -1));
                    lua_pop(m_pLuaState, 1);
                }
            }
            else
            {
                LOG_ERR("Failed to retrieve data for LUA script.");
            }
        }
    }

    void LuaScript::Run()
    {
        if (lua_pcall(m_pLuaState, 0, LUA_MULTRET, 0))
        {
            LOG_ERR("LUA: ", lua_tostring(m_pLuaState, -1));
            lua_pop(m_pLuaState, 1);
        }
    }

    /**
     * Recursively traverses a global table and creates LuaVar representations for each key/value pair.
     */
    std::shared_ptr<LuaTable> LuaScript::GetGlobalTable(const std::string & key)
    {
        int stack_top = lua_gettop(m_pLuaState);

        // load the global variable onto the stack
        // and verify it is actually a table.
        int type = lua_getglobal(m_pLuaState, key.c_str());
        if (type != LUA_TTABLE)
        {
            LOG_WARN("Attempt to access global variable [", key, "] as table failed, it is not a table, <", lua_typename(m_pLuaState, lua_type(m_pLuaState, -1)), "> detected.");
            lua_pop(m_pLuaState, 1);
            return nullptr;
        }

        // Recurse the table key/values and build a table class
        auto table = this->BuildTable(key, lua_gettop(m_pLuaState));

        // pop the global table from the stack
        // so that we dont leave anything lingering...
        lua_pop(m_pLuaState, 1);

#if ALPHA_DEBUG
        if (stack_top != lua_gettop(m_pLuaState))
        {
            LOG_ERR("Stack top should be ", stack_top, ", actual value is: ", lua_gettop(m_pLuaState));
        }
#endif

        // cast and return the LuaTable
        return std::dynamic_pointer_cast<LuaTable>(table);
    }

    std::shared_ptr<LuaVar> LuaScript::BuildTable(std::string table_name, int index)
    {
        std::shared_ptr<LuaTable> table = std::make_shared<LuaTable>(table_name);

        lua_pushnil(m_pLuaState);
        while (lua_next(m_pLuaState, index) != 0)
        {
            /* now 'key' at index -2, 'value' at index -1 */
            std::string key = lua_tostring(m_pLuaState, -2);

            // switch on the value type, and create LuaVar's depending on the type
            // for now we only support basic types, and sub tables.  This could be
            // expanded to support other values such as nil, or even functions.
            switch (lua_type(m_pLuaState, -1))
            {
            case LUA_TSTRING:
                table->Push(key, this->BuildString(key, lua_tostring(m_pLuaState, -1)));
                break;
            case LUA_TNUMBER:
                table->Push(key, this->BuildNumber(key, lua_tonumber(m_pLuaState, -1)));
                break;
            case LUA_TBOOLEAN:
                switch (lua_toboolean(m_pLuaState, -1))
                {
                case 0:
                    table->Push(key, this->BuildBoolean(key, false));
                    break;
                case 1:
                    table->Push(key, this->BuildBoolean(key, true));
                    break;
                default:
                    LOG_WARN("Invalid boolean value, could not push to LuaTable");
                }
                break;
            case LUA_TTABLE:
                // recurse to make a new table
                table->Push(key, this->BuildTable(lua_tostring(m_pLuaState, -2), lua_gettop(m_pLuaState)));
                break;
            default:
                LOG_WARN("Unsupported type in table: ", lua_typename(m_pLuaState, lua_type(m_pLuaState, -1)));
            }

            /* pop value, not the key */
            lua_pop(m_pLuaState, 1);
        }

        return table;
    }

    std::shared_ptr<LuaVar> LuaScript::BuildString(std::string name, std::string value)
    {
        return std::shared_ptr<LuaVar>(new LuaStatic<std::string>(name, value));
    }

    std::shared_ptr<LuaVar> LuaScript::BuildNumber(std::string name, double value)
    {
        return std::shared_ptr<LuaVar>(new LuaStatic<double>(name, value));
    }

    std::shared_ptr<LuaVar> LuaScript::BuildBoolean(std::string name, bool value)
    {
        return std::shared_ptr<LuaVar>(new LuaStatic<bool>(name, value));
    }
}
