#ifndef ALPHA_HID_CONTEXT_H
#define ALPHA_HID_CONTEXT_H

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
#include <string>
#include <vector>

namespace alpha
{
    struct HIDAction;

    struct HIDContextAction
    {
        std::string name;
    };

    /**
     * HIDContext abstract base class for translating hardware input into an input state or range.
     */
    class HIDContext
    {
    public:
        HIDContext();
        virtual ~HIDContext();

        HIDContextAction * TranslateAction(const HIDAction * action) const;
        HIDContextAction * TranslateState(const HIDAction * action) const;
        HIDContextAction * TranslateRange(const HIDAction * action) const;

    protected:
        void MapAction(const std::string & action, std::vector<std::string> mappings);
        void MapState(const std::string & action, std::vector<std::string> mappings);
        void MapRange(const std::string & action, std::vector<std::string> mappings);

    private:
        std::map<std::string, HIDContextAction> m_actions;
        std::map<std::string, HIDContextAction> m_states;
        std::map<std::string, HIDContextAction> m_ranges;
    };
}

#endif // ALPHA_HID_CONTEXT_H