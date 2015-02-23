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

#include <math.h>

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

    void Vector3::Normalize()
    {
        *this /= this->Length();
    }

    float Vector3::Length() const
    {
        float x = this->x * this->x;
        float y = this->y * this->y;
        float z = this->z * this->z;
        return sqrtf(x + y + z);
    }

    float Vector3::Distance(const Vector3 & right)
    {
        return (right - (*this)).Length();
    }

    float Vector3::Dot(const Vector3 & right)
    {
        float x = this->x * right.x;
        float y = this->y * right.y;
        float z = this->z * right.z;
        return x + y + z;
    }

    Vector3 Vector3::Cross(const Vector3 & right)
    {
        float x = this->y * right.z - this->z * right.y;
        float y = this->z * right.x - this->x * right.z;
        float z = this->x * right.y - this->y * right.x;
        return Vector3(x, y, z);
    }

    Vector3 & Vector3::operator=(const Vector3 & right)
    {
        this->x = right.x;
        this->y = right.y;
        this->z = right.z;
        return *this;
    }

    Vector3 & Vector3::operator += (const Vector3 & right)
    {
        this->x += right.x;
        this->y += right.y;
        this->z += right.z;
        return *this;
    }

    Vector3 & Vector3::operator -= (const Vector3 & right)
    {
        this->x -= right.x;
        this->y -= right.y;
        this->z -= right.z;
        return *this;
    }

    Vector3 & Vector3::operator *= (const Vector3 & right)
    {
        this->x *= right.x;
        this->y *= right.y;
        this->z *= right.z;
        return *this;
    }

    Vector3 & Vector3::operator *= (float right)
    {
        this->x *= right;
        this->y *= right;
        this->z *= right;
        return *this;
    }

    Vector3 & Vector3::operator /= (float right)
    {
        this->x /= right;
        this->y /= right;
        this->z /= right;
        return *this;
    }

    Vector3 operator+(const Vector3 & left, const Vector3 & right)
    {
        return Vector3(left.x + right.x,
                       left.y + right.y,
                       left.z + right.z);
    }

    Vector3 operator-(const Vector3 & left, const Vector3 & right)
    {
        return Vector3(left.x - right.x,
                       left.y - right.y,
                       left.z - right.z);
    }

    Vector3 operator*(const Vector3 & left, const Vector3 & right)
    {
        return Vector3(left.x * right.x,
                       left.y * right.y,
                       left.z * right.z);
    }

    Vector3 operator*(const Vector3 & left, float right)
    {
        return Vector3(left.x * right,
                       left.y * right,
                       left.z * right);
    }

    Vector3 operator/(const Vector3 & left, const Vector3 & right)
    {
        return Vector3(left.x / right.x,
                       left.y / right.y,
                       left.z / right.z);
    }

    Vector3 operator*(float left, const Vector3 & right)
    {
        return Vector3(left * right.x,
                       left * right.y,
                       left * right.z);
    }
}
