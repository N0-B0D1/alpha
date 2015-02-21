#ifndef ALPHA_MATRIX_H
#define ALPHA_MATRIX_H

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
    struct Vector3;
    struct Quaternion;

    struct Matrix
    {
        float m_11, m_12, m_13, m_14;
        float m_21, m_22, m_23, m_24;
        float m_31, m_32, m_33, m_34;
        float m_41, m_42, m_43, m_44;

        Matrix();
        Matrix(float _11, float _12, float _13, float _14,
               float _21, float _22, float _23, float _24,
               float _31, float _32, float _33, float _34,
               float _41, float _42, float _43, float _44);

        static Matrix Rotate(const Quaternion & rotation);
        static Matrix Translate(const Vector3 & position);
        static Matrix Scale(const Vector3 & scale);
    };

    Matrix operator* (const Matrix& left, const Matrix& right);
}

#endif // ALPHA_MATRIX_H
