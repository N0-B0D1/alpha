#ifndef VECTOR_4_H
#define VECTOR_4_H

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

#include "Math/Vector3.h"

namespace alpha
{
    struct Vector4
    {
        float x, y, z, w;

        Vector4();
        Vector4(float _x, float _y, float _z, float _w);
        Vector4(const Vector3 & vec3, float _w);
        Vector4(const Vector4 &vec);

        Vector4 & operator=(const Vector4 & right);
    };

    Vector4 operator+(const Vector4 & left, const Vector4 & right);
    Vector4 operator-(const Vector4 & left, const Vector4 & right);
    Vector4 operator*(const Vector4 & left, const Vector4 & right);
    Vector4 operator*(const Vector4 & left, float right);
    Vector4 operator/(const Vector4 & left, const Vector4 & right);
    Vector4 operator*(float left, const Vector4 & right);
}

#endif // VECTOR_4_H
