#ifndef ALPHA_CAMERA_H
#define ALPHA_CAMERA_H

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

#include <memory>

#include "Math/Matrix.h"
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

namespace alpha
{
    class CameraComponent;

    /**
     * A camera creates and manages a view and projection matrix.
     * Contains a pointer to a camera component which can be use
     * in the game logic to drive the camera.
     */
    class Camera
    {
    public:
        explicit Camera(Vector3 position = Vector3(0.f, 0.f, 0.f));
        virtual ~Camera();

        /** Update the camera each frame, expects the current screen width and height as arguments */
        void Update(short width, short height);

        /** Set the camera component that will drive this camera. */
        void SetCameraComponent(std::weak_ptr<CameraComponent> pCameraComponent);

        /** Obtain the pre-calcualted view matrix for rendering */
        Matrix GetView() const;
        /** Obtain the pre-calculated projection matrix for rendering */
        Matrix GetProjection() const;

    private:
        /** The calculated view matrix */
        Matrix m_view;
        /** The calculated projection matrix */
        Matrix m_projection;

        /** A shared pointer to the current camera component that will drive this camera. */
        std::weak_ptr<CameraComponent> m_pCameraComponent;

        float m_fov;
        float m_near;
        float m_far;

        Vector3 m_position;
        Quaternion m_rotation;
    };
}

#endif // ALPHA_CAMERA_H
