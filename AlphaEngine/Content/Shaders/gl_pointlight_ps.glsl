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

uniform vec2      gScreenSize;
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

uniform PointLight pointLight;
uniform vec3 viewPosition;

void main ()
{
    vec2 TexCoords = gl_FragCoord.xy / gScreenSize;

    vec3 pixel_position = texture(gPosition, TexCoords).rgb;
    vec3 pixel_normal = normalize(texture(gNormal, TexCoords).rgb);
    vec4 pixel_color = texture(gAlbedoSpec, TexCoords);
    float pixel_specular = pixel_color.a;
    pixel_color.a = 1.0f;

    vec3 view_direction = normalize(viewPosition - pixel_position);

    // diffuse
    vec3 light_direction = normalize(pointLight.position - pixel_position);
    vec3 diffuse = max(dot(pixel_normal, light_direction), 0.0) * pixel_color.xyz * pointLight.diffuse;

    // specular
    vec3 half_direction = normalize(light_direction + view_direction);
    float spec = pow(max(dot(pixel_normal, half_direction), 0.0), 16.0);
    vec3 specular = pointLight.diffuse * spec * pixel_specular;
    
    // attenuation
    float distance = length(pointLight.position - pixel_position);
    float attenuation = 1.0f / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * (distance * distance)));

    diffuse *= attenuation;
    specular *= attenuation;

    FragColor = vec4(diffuse + specular, 1.0f);
}
