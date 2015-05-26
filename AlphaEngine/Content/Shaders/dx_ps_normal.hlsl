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
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 vLightDir[2];
	float4 vLightColor[2];
	float4 ambient;
	float4 diffuse;
	float4 specular;
	//float shininess;
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
};

// Pixel Shader
float4 PS( PS_INPUT input) : SV_Target
{
	float ambientStrength = 0.1f;
	float specularStrength = 0.5f;
	
    float4 finalColor = 0;
	
	float3 norm = normalize(input.Norm);
	float4 viewPos = float4(View._41, View._42, View._43, 1.0f);
	float4 viewDir = normalize(viewPos - input.Pos);
    
    //do NdotL lighting for 2 lights
    for(int i = 0; i < 2; i++)
    {
		// ambient
		float4 ambientColor = vLightColor[i] * ambient;
		
		// diffuse
		float4 lightDirection = normalize(vLightDir[i] * input.Pos);
		float diff = max( dot(norm, lightDirection), 0.0);
		float4 diffuseColor = (diff * diffuse) * vLightColor[i];
		
		// specular
		float3 reflectDirection = reflect(-lightDirection.xyz, norm.xyz);
		float spec = pow(max(dot(viewDir.xyz, reflectDirection), 0.0), 32.0f);
		float4 specularColor = specularStrength * (spec * specular) * vLightColor[i];
		
		finalColor += (ambientColor + diffuseColor + specularColor);
    }
	
	//finalColor *= vOutputColor;
    finalColor.a = 1;
    return finalColor;
}
