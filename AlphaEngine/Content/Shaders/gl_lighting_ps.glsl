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

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

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

uniform vec3 viewPosition;

void main ()
{
    vec3 pixel_position = texture(gPosition, TexCoords).rgb;
    vec3 pixel_normal = normalize(texture(gNormal, TexCoords).rgb);
    vec4 pixel_color = texture(gAlbedoSpec, TexCoords);
    float pixel_specular = pixel_color.a;
    pixel_color.a = 1.0f;

    // static ambient ...
    vec3 lighting = pixel_color.xyz * directionalLight.ambient;
    lighting += max(dot(pixel_normal, directionalLight.direction), 0.0) * pixel_color.xyz * directionalLight.diffuse;

    vec3 view_direction = normalize(viewPosition - pixel_position);

    for (int i = 0; i < 2; ++i)
    {
        // diffuse
        vec3 lightDir = normalize(pointLight[i].position - pixel_position);
        vec3 diffuse = max(dot(pixel_normal, lightDir), 0.0) * pixel_color.xyz * pointLight[i].diffuse;

        // specular
        vec3 half_direction = normalize(lightDir + view_direction);
        float spec = pow(max(dot(pixel_normal, half_direction), 0.0), 16.0);
        vec3 specular = pointLight[i].diffuse * spec * pixel_specular;
        
        // attenuation
        float distance = length(pointLight[i].position - pixel_position);
        float attenuation = 1.0f / (pointLight[i].constant + (pointLight[i].linear * distance) + (pointLight[i].quadratic * (distance * distance)));

        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse + specular;
    }

    FragColor = vec4(lighting, 1.0f);
}
