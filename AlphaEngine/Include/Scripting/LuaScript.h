#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

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

#include <vector>

struct lua_State;

namespace alpha
{
    class Asset;

    class LuaScript
    {
    public:
        explicit LuaScript();
        virtual ~LuaScript();

        void Add(std::shared_ptr<Asset> asset);

        void Load();
        void Run();

    private:
        lua_State *m_pLuaState;
        std::shared_ptr<Asset> m_pScriptAsset;

        std::vector<std::shared_ptr<Asset> > m_scriptAssets;
    };
}

#endif // LUA_SCRIPT_H