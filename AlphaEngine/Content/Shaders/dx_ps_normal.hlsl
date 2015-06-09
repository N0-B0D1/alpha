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

// lighting buffer
cbuffer ConstantBuffer : register(b1)
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
    float shininess;
    float _spacer1;
    float _spacer2;
    float _spacer3;
	float4 vOutputColor;

	struct PointLight
	{
		float4 position;
		float4 ambient;
		float4 diffuse;
		float4 specular;
        float attenuationConstant;
        float attenuationLinear;
        float attenuationQuadratic;
        float _spacer;
	} pointLight[2];
	
	struct DirectionalLight
	{
		float4 direction;
		float4 ambient;
		float4 diffuse;
		float4 specular;
	} directionalLight;
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

float4 calcAmbient(float4 light_ambient)
{
	return (light_ambient * ambient);
}

float4 calcDiffuse(PS_INPUT input, float4 light_diffuse, float3 light_direction)
{
	float diff = saturate(dot(input.Norm, -light_direction));
	return light_diffuse * (diff * diffuse);
}

float4 calcSpecular(PS_INPUT input, float4 light_specular, float3 light_direction)
{
	float3 reflectDirection = reflect(-light_direction, input.Norm);
	float spec = pow(saturate(dot(reflectDirection, input.ViewDir)), shininess);
	return (light_specular * (spec * specular));
}

// calculate pixel color based on point lighting
float4 calculatePointLighting(int index, PS_INPUT input)
{
	float3 lightDirection = normalize(input.WorldPos - pointLight[index].position);

    float distance = length(pointLight[index].position.xyz - input.WorldPos.xyz);
    float attenuation = 1.0f / (pointLight[index].attenuationConstant + (pointLight[index].attenuationLinear * distance) + (pointLight[index].attenuationQuadratic * (distance * distance)));

    float4 ambientColor = calcAmbient(pointLight[index].ambient) * attenuation;
    float4 diffuseColor = calcDiffuse(input, pointLight[index].diffuse, lightDirection) * attenuation;
    float4 specularColor = calcSpecular(input, pointLight[index].specular, lightDirection) * attenuation;

    return (ambientColor + diffuseColor + specularColor);
}

// calculate pixel color based on point lighting
float4 calculateDirectionalLighting(PS_INPUT input)
{
    float4 ambientColor = calcAmbient(directionalLight.ambient);
    float4 diffuseColor = calcDiffuse(input, directionalLight.diffuse, directionalLight.direction.xyz);
    float4 specularColor = calcSpecular(input, directionalLight.specular, directionalLight.direction.xyz);

    return (ambientColor + diffuseColor + specularColor);
}

// Pixel Shader
float4 PS( PS_INPUT input) : SV_Target
{
	float4 finalColor = calculateDirectionalLighting(input);
    
    for(int i = 0; i < 2; i++)
    {
		finalColor += calculatePointLighting(i, input);
    }
	
    finalColor.a = 1;
    return finalColor;
}
