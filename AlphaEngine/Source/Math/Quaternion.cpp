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

#include "Math/Quaternion.h"
#include "Math/Vector3.h"

namespace alpha
{
    Quaternion::Quaternion()
        : x(0), y(0), z(0), w(1.f)
    { }
    Quaternion::Quaternion(float _x, float _y, float _z, float _w)
        : x(_x), y(_y), z(_z), w(_w)
    { }

    bool Quaternion::operator == (const Quaternion& q) const
    {
        return (this->x == q.x && this->y == q.y && this->z == q.z && this->w == q.w);
    }

    Quaternion Quaternion::RotationFromAxisAngle(const Vector3 & axis, float radians)
    {
        float x = sin(radians / 2) * axis.x;
        float y = sin(radians / 2) * axis.y;
        float z = sin(radians / 2) * axis.z;
        float w = cos(radians / 2);
        return Quaternion(x, y, z, w);
    }

    Quaternion Quaternion::Slerp(const Quaternion & start, const Quaternion & end, float time)
    {
        float k0, k1;
        float ex = end.x;
        float ey = end.y;
        float ez = end.z;
        float ew = end.w;
        float cosOmega = (start.x * end.x) + (start.y * end.y) + (start.z * end.z) + (start.w * end.w);
        
        // if needed, reverse end to take shortest path
        if (cosOmega < 0.0f)
        {
            ex = -ex;
            ey = -ey;
            ez = -ez;
            ew = -ew;
            cosOmega = -cosOmega;
        }

        // if close, use linear interpolate
        if (cosOmega > 0.9999f)
        {
            k0 = 1.0f - time;
            k1 = time;
        }
        else
        {
            float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
            float omega = atan2(sinOmega, cosOmega);
            float oneOverSinOmega = 1.0f / sinOmega;

            k0 = sin((1.0f - time) * omega) * oneOverSinOmega;
            k1 = sin(time * omega) * oneOverSinOmega;
        }

        return Quaternion(start.x * k0 + ex * k1,
                          start.y * k0 + ey * k1,
                          start.z * k0 + ez * k1,
                          start.w * k0 + ew * k1);
    }
}
