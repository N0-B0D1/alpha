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

// Constant Buffer

// matrix buffer
cbuffer MatrixBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

// camera buffer
cbuffer CameraBuffer : register(b1)
{
	float3 cameraPosition;
	float _spacer;
}

// Typedef input/output
struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Norm : NORMAL;
	float3 WorldPos : TEXCOORD0;
	float3 ViewDir : TEXCOORD1;
};

// Vertex Shader
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	
	input.Pos.w = 1.0f;
	
	float4 posWorld = mul(input.Pos, World);

    output.Pos = posWorld;
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);

    output.Norm = mul( float4( input.Norm, 1 ), World ).xyz;
	output.Norm = normalize(output.Norm);

	output.WorldPos = posWorld;
	output.ViewDir = normalize(cameraPosition - posWorld.xyz);

    return output;
}
