#ifndef TRANSFORM_H
#define TRANSFORM_H

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

#include <DirectXMath.h>

using namespace DirectX;

namespace alpha
{
    struct Vector3;
    struct Quaternion;

    /**
     * \brief Engine Matrix representation based on DirectX::XMFLOAT4X4
     * By inheriting XMFLOAT4X4 we gain all of the SSE intrinsic calculations, and can add our own on top as needed.
     */
    struct Matrix : public XMFLOAT4X4
    {
    public:
        Matrix();

        void Translate(const Vector3 & position);
        void Scale(const Vector3 & scale);
        void Rotate(const Quaternion & rotation);

        static Matrix CreateScale(const Vector3 & scale);
        static Matrix CreateTranslation(const Vector3 & position);
    };

    Matrix operator* (const Matrix& left, const Matrix& right);
}

#endif // TRANSFORM_H
