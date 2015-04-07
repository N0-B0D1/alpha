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

#include "Math/Vector4.h"

namespace alpha
{
    Vector4::Vector4()
        : x(0.f), y(0.f), z(0.f), w(0.f)
    { }
    Vector4::Vector4(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w)
    { }
    Vector4::Vector4(const Vector3 & vec3, float _w)
        : x(vec3.x), y(vec3.y), z(vec3.z), w(_w)
    { }

    Vector4::Vector4(const Vector4 &vec)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
        this->w = vec.w;
    }

    Vector4 & Vector4::operator=(const Vector4 & right)
    {
        this->x = right.x;
        this->y = right.y;
        this->z = right.z;
        this->w = right.w;
        return *this;
    }
}
