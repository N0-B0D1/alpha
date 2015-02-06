#ifndef VECTOR_3_H
#define VECTOR_3_H

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
    class Vector3
    {
    public:
        Vector3();
        Vector3(float fx, float fy, float fz);
        Vector3(const Vector3 &vec);
        virtual ~Vector3();

        Vector3 & operator=(const Vector3 & right);

        //! Vector position attributes
        float x;
        float y;
        float z;
    };
}

#endif // VECTOR_3_H
