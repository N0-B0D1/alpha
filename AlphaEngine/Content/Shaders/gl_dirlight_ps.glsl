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

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight directionalLight;

void main ()
{
    vec3 pixel_position = texture(gPosition, TexCoords).rgb;
    vec3 pixel_normal = normalize(texture(gNormal, TexCoords).rgb);
    vec4 pixel_color = texture(gAlbedoSpec, TexCoords);
    float pixel_specular = pixel_color.a;
    pixel_color.a = 1.0f;

    // simple directional lighting
    vec3 lighting = pixel_color.xyz * directionalLight.ambient;
    lighting += max(dot(pixel_normal, directionalLight.direction), 0.0) * pixel_color.xyz * directionalLight.diffuse;

    FragColor = vec4(lighting, 1.0f);
}
