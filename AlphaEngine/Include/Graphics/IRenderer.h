#ifndef ALPHA_IRENDERER_H
#define ALPHA_IRENDERER_H

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
#include <vector>

namespace alpha
{
    class AssetSystem;
    class Camera;
    class RenderSet;
    class Light;

    class IRenderer
    {
    public:
        virtual ~IRenderer() { };

        virtual bool Initialize(AssetSystem * const pAssets) = 0;
        virtual bool Update(double currentTime, double elapsedTime) = 0;
        virtual bool Shutdown() = 0;

        // rendering
        virtual void PreRender(RenderSet * renderSet) = 0;
        virtual void Render(std::shared_ptr<Camera> pCamera, std::vector<RenderSet *> renderables, std::vector<Light *> lights) = 0;
    };
}

#endif // ALPHA_IRENDERER_H
