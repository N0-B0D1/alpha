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

#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"

namespace alpha
{
    Matrix::Matrix()
        : m_11(1.f), m_12(0.f), m_13(0.f), m_14(0.f)
        , m_21(0.f), m_22(1.f), m_23(0.f), m_24(0.f)
        , m_31(0.f), m_32(0.f), m_33(1.f), m_34(0.f)
        , m_41(0.f), m_42(0.f), m_43(0.f), m_44(1.f)
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

    Vector3 Matrix::Position() const
    {
        return Vector3(m_41, m_42, m_43);
    }

    Matrix Matrix::Rotate(const Quaternion & rotation)
    {
        float x2 = rotation.x * rotation.x;
        float y2 = rotation.y * rotation.y;
        float z2 = rotation.z * rotation.z;

        float xy = rotation.x * rotation.y;
        float xz = rotation.x * rotation.z;
        float xw = rotation.x * rotation.w;

        float yz = rotation.y * rotation.z;

        float wy = rotation.w * rotation.y;
        float wz = rotation.w * rotation.z;

        float m11 = 1 - 2 * y2 - 2 * z2;
        float m12 = 2 * xy + 2 * wz;
        float m13 = 2 * xz - 2 * wy;
        float m21 = 2 * xy - 2 * wz;
        float m22 = 1 - 2 * x2 - 2 * z2;
        float m23 = 2 * yz + 2 * xw;
        float m31 = 2 * xz + 2 * wy;
        float m32 = 2 * yz - 2 * xw;
        float m33 = 1 - 2 * x2 - 2 * y2;

        return Matrix(m11, m12, m13, 0.f,
                      m21, m22, m23, 0.f,
                      m31, m32, m33, 0.f,
                      0.f, 0.f, 0.f, 1.f);
    }

    Matrix Matrix::Translate(const Vector3 & position)
    {
        return Matrix(1.f, 0.f, 0.f, 0.f,
                      0.f, 1.f, 0.f, 0.f,
                      0.f, 0.f, 1.f, 0.f,
                      position.x, position.y, position.z , 1.f);
    }

    Matrix Matrix::Scale(const Vector3 & scale)
    {
        return Matrix(scale.x, 0.f, 0.f, 0.f,
                      0.f, scale.y, 0.f, 0.f,
                      0.f, 0.f, scale.z, 0.f,
                      0.f, 0.f, 0.f, 1.f);
    }

    Matrix Matrix::Projection(float fov, float aspect, float near, float far)
    {
        float depth = far - near;
        float oneOverDepth = 1 / depth;
        Matrix projection;

        projection.m_22 = 1 / tan(0.5f * fov);
        projection.m_11 = (1) * projection.m_22 / aspect;
        projection.m_33 = far * oneOverDepth;
        projection.m_43 = (-far * near) * oneOverDepth;
        projection.m_34 = 1.f;
        projection.m_44 = 0.f;

        return projection;
    }

    Matrix operator* (const Matrix& left, const Matrix& right)
    {
        float m11 = (left.m_11 * right.m_11) + (left.m_12 * right.m_21) + (left.m_13 * right.m_31) + (left.m_14 * right.m_41);
        float m12 = (left.m_11 * right.m_12) + (left.m_12 * right.m_22) + (left.m_13 * right.m_32) + (left.m_14 * right.m_42);
        float m13 = (left.m_11 * right.m_13) + (left.m_12 * right.m_23) + (left.m_13 * right.m_33) + (left.m_14 * right.m_43);
        float m14 = (left.m_11 * right.m_14) + (left.m_12 * right.m_24) + (left.m_13 * right.m_34) + (left.m_14 * right.m_44);

        float m21 = (left.m_21 * right.m_11) + (left.m_22 * right.m_21) + (left.m_23 * right.m_31) + (left.m_24 * right.m_41);
        float m22 = (left.m_21 * right.m_12) + (left.m_22 * right.m_22) + (left.m_23 * right.m_32) + (left.m_24 * right.m_42);
        float m23 = (left.m_21 * right.m_13) + (left.m_22 * right.m_23) + (left.m_23 * right.m_33) + (left.m_24 * right.m_43);
        float m24 = (left.m_21 * right.m_14) + (left.m_22 * right.m_24) + (left.m_23 * right.m_34) + (left.m_24 * right.m_44);

        float m31 = (left.m_31 * right.m_11) + (left.m_32 * right.m_21) + (left.m_33 * right.m_31) + (left.m_34 * right.m_41);
        float m32 = (left.m_31 * right.m_12) + (left.m_32 * right.m_22) + (left.m_33 * right.m_32) + (left.m_34 * right.m_42);
        float m33 = (left.m_31 * right.m_13) + (left.m_32 * right.m_23) + (left.m_33 * right.m_33) + (left.m_34 * right.m_43);
        float m34 = (left.m_31 * right.m_14) + (left.m_32 * right.m_24) + (left.m_33 * right.m_34) + (left.m_34 * right.m_44);

        float m41 = (left.m_41 * right.m_11) + (left.m_42 * right.m_21) + (left.m_43 * right.m_31) + (left.m_44 * right.m_41);
        float m42 = (left.m_41 * right.m_12) + (left.m_42 * right.m_22) + (left.m_43 * right.m_32) + (left.m_44 * right.m_42);
        float m43 = (left.m_41 * right.m_13) + (left.m_42 * right.m_23) + (left.m_43 * right.m_33) + (left.m_44 * right.m_43);
        float m44 = (left.m_41 * right.m_14) + (left.m_42 * right.m_24) + (left.m_43 * right.m_34) + (left.m_44 * right.m_44);

        return Matrix(m11, m12, m13, m14,
                      m21, m22, m23, m24,
                      m31, m32, m33, m34,
                      m41, m42, m43, m44);
    }
}
