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

#include <memory>
#include <string.h>
#include <vector>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "Scripting/LuaScript.h"
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
            //if (data)
            if (data.size() > 0)
            {
                char * buffer = reinterpret_cast<char *>(&data[0]);
                LOG("Attempting to load buffer data into LUA environment");
                //int error = luaL_loadbuffer(m_pLuaState, data, strlen(data), "line");
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
     * Loads a global variable into the lua state, and tests to see if it is a valid table
     */
    void LuaScript::LoadTable(const char * name)
    {
        lua_getglobal(m_pLuaState, name);
        if (!lua_istable(m_pLuaState, -1))
        {
            LOG_WARN("LUA: global is not a valid table.");
        }
        
    }
}
