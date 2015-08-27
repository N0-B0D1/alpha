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

// MRT Texture globals
Texture2D positionTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D albedoSpecTexture : register(t2);

// texture sampler
SamplerState SampleTypePoint : register(s0);

cbuffer LightBuffer : register(b0)
{
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

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float _spacer;
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

// Pixel Shader
float4 PS(PS_INPUT input) : SV_Target
{
    float4 final_color = float4(0,0,0,0);

    // sample color information
    float4 pixel_diffuse = albedoSpecTexture.Sample(SampleTypePoint, input.tex);
    float pixel_specular = pixel_diffuse.a;
    pixel_diffuse = float4(pixel_diffuse.xyz, 1.0f);
    // sample normal info
    float4 pixel_normal = normalTexture.Sample(SampleTypePoint, input.tex);
    // sample position info
    float4 pixel_position = positionTexture.Sample(SampleTypePoint, input.tex);

    // calculate view direction
    float4 view_direction = normalize(float4(cameraPosition, 1.f) - pixel_position);


    // handle directional light
    // ambient
    float4 ambient_color = directionalLight.ambient * pixel_diffuse;
    // diffuse
    float4 diffuse_color = saturate(dot(pixel_normal, directionalLight.direction)) * pixel_diffuse * directionalLight.diffuse;

    final_color += ambient_color + diffuse_color;


    // handle each point light
    for (int i = 0; i < 2; ++i)
    {
        // diffuse
        float4 light_direction = normalize(pointLight[i].position - pixel_position);
        float4 diffuse_color = saturate(dot(pixel_normal, light_direction)) * pixel_diffuse * pointLight[i].diffuse;

        // specular
        float3 halfway_direction = normalize(-light_direction + view_direction).xyz;
        float spec = pow(saturate(dot(pixel_normal.xyz, -halfway_direction)), 16.0);
        float4 specular_color = pointLight[i].specular * spec * pixel_specular;

        // attenuation
        float distance = length(pointLight[i].position.xyz - pixel_position.xyz);
        float attenuation = 1.0f / (1.0f + (pointLight[i].attenuationLinear * distance) + (pointLight[i].attenuationQuadratic * (distance * distance)));

        diffuse_color *= attenuation;
        specular_color *= attenuation;

        final_color += diffuse_color + specular_color;
    }
    
    return float4(final_color.xyz, 1.f);
}
