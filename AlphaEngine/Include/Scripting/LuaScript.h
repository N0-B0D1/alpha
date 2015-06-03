#ifndef LUA_SCRIPT_H
#define LUA_SCRIPT_H

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
#include <vector>

struct lua_State;

namespace alpha
{
    class Asset;
    class LuaTable;
    class LuaVar;

    class LuaScript
    {
    public:
        explicit LuaScript();
        virtual ~LuaScript();

        void Add(std::shared_ptr<Asset> asset);
        void Load();
        void Run();

        /** Helper function for retrieving float var from script tables */
        void GetTableFloatValue(std::shared_ptr<LuaTable> table, const std::string key, float * const out);
        /** Helper function for retrieving std::string var from script tables */
        void GetTableStringValue(std::shared_ptr<LuaTable> table, const std::string key, std::string * const out);
        /** Helper function for retrieving boolean var from script tables */
        void GetTableBoolValue(std::shared_ptr<LuaTable> table, const std::string key, bool * const out);

    protected:
        std::shared_ptr<LuaTable> GetGlobalTable(const std::string & key);

    private:
        std::shared_ptr<LuaVar> BuildTable(std::string table_name, int index);
        std::shared_ptr<LuaVar> BuildString(std::string name, std::string value);
        std::shared_ptr<LuaVar> BuildNumber(std::string name, double value);
        std::shared_ptr<LuaVar> BuildBoolean(std::string name, bool value);

        lua_State *m_pLuaState;
        std::vector<std::shared_ptr<Asset> > m_scriptAssets;
    };
}

#endif // LUA_SCRIPT_H
