// Copyright 2014-2015 Jason R. Wendlandt
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Constant Buffers

// matrix buffer
cbuffer MatrixBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

// object color data buffer
cbuffer DeferredBuffer : register(b1)
{
    float4 diffuse;
    float specular;
    float _spacer1;
    float _spacer2;
    float _spacer3;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 world_position : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
};

// Pixel Shader
PS_OUTPUT PS(PS_INPUT input) : SV_Target
{
    PS_OUTPUT output = (PS_OUTPUT)0;

    output.position = float4(input.world_position, 1.f);
    output.normal = float4(input.normal, 1.f);
    output.color = float4(diffuse.xyz, specular);

    return output;
}
