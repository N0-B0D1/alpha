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

#include "Graphics/Camera.h"

namespace alpha
{
    Camera::Camera(Vector3 position /*= Vector3(0.f, 0.f, 0.f)*/)
    {
        // set view matrix
        m_view = Matrix::Translate(position);

        // set projection matrix
        m_projection = Matrix::Projection(45.f, 800.f / 600.f, 0.1f, 100.f);
    }
    Camera::~Camera() { }

    Matrix Camera::GetView() const
    {
        return m_view;
    }

    Matrix Camera::GetProjection() const
    {
        return m_projection;
    }
}
