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
    { }
    Matrix::~Matrix() { }

    void Matrix::Rotate(const Quaternion & /*rotation*/)
    {
        // XXX fix this
    }

    void Matrix::Translate(const Vector3 & /*position*/)
    {
        // XXX fix this
    }

    void Matrix::Scale(const Vector3 & /*scale*/)
    {
        // XXX fix this
    }

    Matrix operator* (const Matrix& /*left*/, const Matrix& /*right*/)
    {
        // XXX implement this
        return Matrix();
    }
}
