#ifndef RENDERER_H
#define RENDERER_H

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

#include <Windows.h>
#include <d3d11_1.h>

#include <string>
#include <memory>

//#include "AlphaSystem.h"

namespace alpha
{
    class GraphicsWindow;
    class RenderData;
    class Asset;

    class GraphicsRenderer
    {
    public:
        static const std::string sk_shader_extension;
        GraphicsRenderer();
        virtual ~GraphicsRenderer();

        bool Initialize();
        bool Update(double currentTime, double elapsedTime);
        bool Shutdown();

        void Render(std::vector<RenderData *> renderables);

        void SetBasicShaders(std::shared_ptr<Asset> psShader, std::shared_ptr<Asset> vsShader);

        // for each object to be rendered
        //
        // -- pre render -<<-------------------------------------------|
        // calculate world transform matrix                            |
        // get or compile/load vertex/pixel shader                     |
        // define vertext layout (maybe once statically?)              |
        // get/fill vertex buffer                                      |
        // get/fill index buffer                                       |
        // -- render                                                   |
        // create constant buffer (can do during pre-render stage?)    |
        // set vertex buffer with constant buffer                      |
        // set pixel shader with constant buffer                       |
        // draw indexed                                                |
        // -- pre-render ->>-------------------------------------------|

    private:
        //virtual bool VUpdate(double currentTime, double elapsedTime);

        // DirectX methods
        //static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
        //HRESULT InitializeWindow();
        HRESULT InitializeDevice();
        void CleanupDevice();

        GraphicsWindow *m_pWindow;

        std::shared_ptr<Asset> m_vsDefaultShader;
        std::shared_ptr<Asset> m_psDefaultShader;

        /** PreRender takes a list of data that will be rendered, and preps it rendering. */
        void PreRender(std::vector<RenderData *> renderables);

        /** Compile a shader from a given asset file, which is presumably an hlsl file. */
        bool CompileShaderFromAsset(std::shared_ptr<Asset> asset, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
        /** Creates and returns a Vertex Shader from the given asset, also outputs blob data which can be passed into Input Layout creation */
        ID3D11VertexShader * CreateVertexShaderFromAsset(std::shared_ptr<Asset> vsAsset, const std::string & sEntryPoint, ID3DBlob** ppVSBlobOut);
        /** Creates and returns a Pixel Shader from the given asset */
        ID3D11PixelShader * CreatePixelShaderFromAsset(std::shared_ptr<Asset> psAsset, const std::string & sEntryPoint);
        /** Creates and returns a Vertx Shader Layout from the given blobl data */
        ID3D11InputLayout * CreateInputLayoutFromVSBlob(ID3DBlob ** const pVSBlob);
        /** Creates a Vertex Buffer based on the given input vertex array */
        //ID3D11Buffer * CreateVertexBuffer()
    };
}

#endif // RENDERER_H
