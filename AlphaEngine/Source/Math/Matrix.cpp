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

#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"

namespace alpha
{
    Matrix::Matrix()
        : m_11(1.f), m_12(0),   m_13(0),   m_14(0)
        , m_21(0),   m_22(1.f), m_23(0),   m_24(0)
        , m_31(0),   m_32(0),   m_33(1.f), m_34(0)
        , m_41(0),   m_42(0),   m_43(0),   m_44(1.f)
    { }
    Matrix::Matrix(float _11, float _12, float _13, float _14,
                   float _21, float _22, float _23, float _24,
                   float _31, float _32, float _33, float _34,
                   float _41, float _42, float _43, float _44)
        : m_11(_11), m_12(_12), m_13(_13), m_14(_14)
        , m_21(_21), m_22(_22), m_23(_23), m_24(_24)
        , m_31(_31), m_32(_32), m_33(_33), m_34(_34)
        , m_41(_41), m_42(_42), m_43(_43), m_44(_44)
    { }

    Matrix Matrix::Rotate(const Quaternion & /*rotation*/)
    {
        // XXX fix this
        return Matrix();
    }

    Matrix Matrix::Translate(const Vector3 & position)
    {
        return Matrix(1.f, 0.f, 0.f, 0.f,
                      0.f, 1.f, 0.f, 0.f,
                      0.f, 0.f, 1.f, 0.f,
                      position.x, position.y, position.z, 1.f);
    }

    Matrix Matrix::Scale(const Vector3 & scale)
    {
        return Matrix(scale.x, 0.f, 0.f, 0.f,
                      0.f, scale.y, 0.f, 0.f,
                      0.f, 0.f, scale.z, 0.f,
                      0.f, 0.f, 0.f, 1.f);
    }

    Matrix operator* (const Matrix& /*left*/, const Matrix& /*right*/)
    {
        // XXX implement this
        return Matrix();
    }
}
