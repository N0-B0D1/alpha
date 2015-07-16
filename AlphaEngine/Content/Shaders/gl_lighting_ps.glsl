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

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct PointLight
{
    vec3 position;
    vec3 diffuse;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLight[2];
uniform DirectionalLight directionalLight;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

void main ()
{
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec4 Diffuse = texture(gAlbedoSpec, TexCoords);
    float Specular = Diffuse.a;

    vec3 lighting = vec3(0, 0, 0);

    for (int i = 0; i < 2; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(pointLight[i].position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse.xyz * pointLight[i].diffuse;

        // specular
        
        // attenuation
        float distance = length(pointLight[i].position - FragPos);
        float attenuation = 1.0f / (pointLight[i].constant + (pointLight[i].linear * distance) + (pointLight[i].quadratic * (distance * distance)));

        diffuse *= attenuation;

        lighting += diffuse;
    }

    FragColor = vec4(lighting, 1.0f);
}
