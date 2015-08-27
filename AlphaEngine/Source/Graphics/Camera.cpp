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
#include "Entities/CameraComponent.h"
#include "Toolbox/Logger.h"

namespace alpha
{
    Camera::Camera(Vector3 position /*= Vector3(0.f, 0.f, 0.f)*/)
        : m_fov(45.f)
        , m_near(0.1f)
        , m_far(100.f)
        , m_width(800.f)
        , m_height(600.f)
        , m_position(position)
    {
        // set view matrix to default position
        m_view = Matrix::Translate(position);
        m_mBaseView = Matrix::Translate(Vector3(0.f, 0.f, 0.f));

        // set projection matrix to default
        m_projection = Matrix::Projection(m_fov, m_width / m_height, m_near, m_far);
        m_mOrthoProjection = Matrix::OrthoProjection(m_width, m_height, m_near, m_far);
    }
    Camera::~Camera() { }

    void Camera::Update(float width, float height)
    {
        if (auto pCameraComponent = m_pCameraComponent.lock())
        {
            // update the projection matrix if field of view, near, or far values have changed
            float fov = pCameraComponent->GetFov();
            float near = pCameraComponent->GetNear();
            float far = pCameraComponent->GetFar();

            if (fov != m_fov || near != m_near || far != m_far || width != m_width || height != m_height)
            {
                m_fov = fov;
                m_near = near;
                m_far = far;
                m_width = width;
                m_height = height;

                LOG("Set camera values fov: ", m_fov, ", near: ", m_near, ", far: ", m_far);

                m_projection = Matrix::Projection(m_fov, width / height, m_near, m_far);
                m_mOrthoProjection = Matrix::OrthoProjection(width, height, m_near, m_far);
            }

            // update the view matrix if position or rotation have changed
            Vector3 position = pCameraComponent->GetPosition();
            Quaternion rotation = pCameraComponent->GetRotation();

            if (!(position == m_position) || !(rotation == m_rotation))
            {
                m_position = position;
                m_rotation = rotation;
                Matrix rotMatrix = Matrix::Rotate(m_rotation);
                Matrix posMatrix = Matrix::Translate(m_position);
                m_view = rotMatrix * posMatrix;
            }
        }
    }

    void Camera::SetCameraComponent(std::weak_ptr<CameraComponent> pCameraComponent)
    {
        m_pCameraComponent = pCameraComponent;
    }

    Matrix Camera::GetView() const
    {
        return m_view;
    }

    Matrix Camera::GetBaseView() const
    {
        return m_mBaseView;
    }

    Matrix Camera::GetProjection() const
    {
        return m_projection;
    }

    Matrix Camera::GetOrthoProjection() const
    {
        return m_mOrthoProjection;
    }
}
