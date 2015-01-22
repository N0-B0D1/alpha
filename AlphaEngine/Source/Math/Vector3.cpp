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

#include "Math/Vector3.h"

namespace alpha
{
    Vector3::Vector3() { }
    Vector3::Vector3(float fx, float fy, float fz)
        : x(fx), y(fy), z(fz)
    { }

    Vector3::Vector3(const Vector3 &vec)
    {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
    }

    Vector3::~Vector3() { }

    Vector3 & Vector3::operator=(const Vector3 & right)
    {
        this->x = right.x;
        this->y = right.y;
        this->z = right.z;
        return *this;
    }
}
