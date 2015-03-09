#ifndef ALPHA_HID_TYPES_H
#define ALPHA_HID_TYPES_H

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

#include <string>

namespace alpha
{
    /**
    * Defines an engine input code, common across all platforms.
    * e.g: 46 = KB_A
    */
    struct HIDAction
    {
        HIDAction(unsigned short actionRaw, std::string actionName);

        unsigned short raw;
        std::string name;
    };

    /** Define supported device types, so they can be easily referenced */
    typedef enum HID
    {
        HID_KEYBOARD,
        HID_MOUSE,
    } HID;

    /** Define simplified mouse actions */
    typedef enum MouseCode
    {
        MC_XAXIS,
        MC_YAXIS,
        MC_LEFT,
        MC_RIGHT,
        MC_MIDDLE,
        MC_4,
        MC_5,
    } MouseCode;

    /** POD class allows tracking of mouse position at a given point in time. */
    struct MousePosition
    {
        MousePosition();

        long xRelativePos;
        long yRelativePos;

        float xAbsolutePos;
        float yAbsolutePos;
    };
}

#endif // ALPHA_HID_TYPES_H