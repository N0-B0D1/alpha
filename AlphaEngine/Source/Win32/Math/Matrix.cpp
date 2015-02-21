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
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

namespace alpha
{
    Matrix::Matrix() : XMFLOAT4X4(1.f,   0,   0,   0,
                                    0, 1.f,   0,   0,
                                    0,   0, 1.f,   0,
                                    0,   0,   0, 1.f)
    { }

    void Matrix::Translate(const Vector3 & position)
    {
        _41 = position.x;
        _42 = position.y;
        _43 = position.z;
    }

    void Matrix::Scale(const Vector3 & scale)
    {
        XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
        XMMATRIX thisMatrix = XMLoadFloat4x4(this);
         //mLight = mLightScale * mLight;
        XMStoreFloat4x4(this, scaleMatrix * thisMatrix);

        //_11 = scale.x;
        //_22 = scale.y;
        //_33 = scale.z;
    }

    void Matrix::Rotate(const Quaternion & rotation)
    {
        XMVECTOR quatVector = XMLoadFloat4(&rotation);
        XMMATRIX quatMatrix = XMMatrixRotationQuaternion(quatVector);
        XMMATRIX thisMatrix = XMLoadFloat4x4(this);
        XMStoreFloat4x4(this, XMMatrixMultiply(thisMatrix, quatMatrix));
    }

    Matrix Matrix::CreateScale(const Vector3 & scale)
    {
        Matrix m;
        XMStoreFloat4x4(&m, XMMatrixScaling(scale.x, scale.y, scale.z));
        return m;
    }

    Matrix Matrix::CreateTranslation(const Vector3 & position)
    {
        Matrix m;
        XMStoreFloat4x4(&m, XMMatrixTranslation(position.x, position.y, position.z));
        return m;
    }

    Matrix operator* (const Matrix& left, const Matrix& right)
    {
        XMMATRIX mLeft = XMLoadFloat4x4(&left);
        XMMATRIX mRight = XMLoadFloat4x4(&right);

        Matrix result;
        XMStoreFloat4x4(&result, mLeft * mRight);
        return result;
    }
}
