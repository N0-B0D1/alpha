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

// lighting buffer
cbuffer ConstantBuffer : register(b1)
{
	float4 vLightPosition[2];
	float4 vLightAmbient[2];
	float4 vLightDiffuse[2];
	float4 vLightSpecular[2];
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float shininess;
	float _spacer1;
	float _spacer2;
	float _spacer3;
	float4 vOutputColor;
}

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

// Pixel Shader
float4 PS( PS_INPUT input) : SV_Target
{
	float4 finalColor = {0, 0, 0, 0};
	
	float3 norm = normalize(input.Norm);
    
    for(int i = 0; i < 2; i++)
    {
		// ambient
		float4 ambientColor = vLightAmbient[i] * ambient;
		
		// diffuse
		float3 lightDirection = normalize(input.WorldPos - vLightPosition[i]);
		float diff = saturate(dot(input.Norm, -lightDirection));
		float4 diffuseColor = saturate(vLightDiffuse[i] * (diff * diffuse));
		
		// specular
		float3 reflectDirection = reflect(-lightDirection, input.Norm);
		float spec = pow(saturate(dot(reflectDirection, input.ViewDir)), shininess);
		float4 specularColor = vLightSpecular[i] * (spec * specular);
		
		finalColor += (ambientColor + diffuseColor + specularColor);
    }
	
    finalColor.a = 1;
    return finalColor;
}
