#ifndef ALPHA_QUATERNION_H
#define ALPHA_QUATERNION_H

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

namespace alpha
{
    struct Vector3;

    struct Quaternion
    {
        float x, y, z, w;

        Quaternion();
        Quaternion(float _x, float _y, float _z, float _w);

        bool operator == (const Quaternion& q) const;

        static Quaternion RotationFromAxisAngle(const Vector3 & axis, float radians);
        static Quaternion Slerp(const Quaternion & start, const Quaternion & end, float time);
    };
}

#endif // ALPHA_QUATERNION_H
