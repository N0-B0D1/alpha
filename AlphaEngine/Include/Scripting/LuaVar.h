#ifndef LUA_VAR_H
#define LUA_VAR_H

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

#include <map>
#include <memory>

namespace alpha
{
    typedef enum LUA_VARTYPE
    {
        VT_STATIC = 1,
        VT_TABLE = 2,
    } LUA_VARTYPE;

    /**
     * Base LUA variable class, stores the variable name
     */
    class LuaVar
    {
    public:
        explicit LuaVar(const std::string & name)
            : m_name(name)
        { }
        virtual ~LuaVar() { }

        virtual LUA_VARTYPE GetVarType() const = 0;

    private:
        std::string m_name;
    };

    /**
     * LuaVar used to represent the contents of a LUA table.
     * Maintains a key/value mapping of variables in the LUA table.
     */
    class LuaTable : public LuaVar
    {
    public:
        explicit LuaTable(const std::string & name);
        virtual ~LuaTable();

        virtual LUA_VARTYPE GetVarType() const;

        void Push(std::string key, std::shared_ptr<LuaVar> value);
        std::shared_ptr<LuaVar> Get(const std::string & key) const;
        const std::map<std::string, std::shared_ptr<LuaVar> > & GetAll() const;

    private:
        std::map<std::string, std::shared_ptr<LuaVar> > m_vars;
    };

    /**
     * LuaVar used to encapsulate a static variable type.
     * and presents it as its C/C++ counterpart.
     * LUA_TSTRING = std::string
     * LUA_TNUMBER = double,
     * LUA_TBOOLEAN = bool,
     * TODO: could possibly represent functions and nil values as well ... needs testing
     */
    template <typename T>
    class LuaStatic : public LuaVar
    {
    public:
        explicit LuaStatic(const std::string & name, T value)
            : LuaVar(name)
            , m_value(value)
        { }
        virtual ~LuaStatic() { }

        virtual LUA_VARTYPE GetVarType() const { return VT_STATIC; }

        T GetValue() const { return m_value; };

    private:
        T m_value;
    };
}

#endif
