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

#include "Math/Matrix_Conversions.h"
#include "Math/Matrix.h"

namespace alpha
{
    DirectX::XMMATRIX AMLoadMatrix(const Matrix & source)
    {
        DirectX::XMMATRIX M;
        M.r[0] = _mm_loadu_ps(&source.m_11);
        M.r[1] = _mm_loadu_ps(&source.m_21);
        M.r[2] = _mm_loadu_ps(&source.m_31);
        M.r[3] = _mm_loadu_ps(&source.m_41);
        return M;
    }
}