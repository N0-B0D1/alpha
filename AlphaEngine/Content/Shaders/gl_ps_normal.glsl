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
out vec4 color;

in vec3 FragPos;
in vec3 Normal;

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

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

uniform vec3 viewPos;
uniform vec3 objectColor;

uniform PointLight pointLight[2];
uniform DirectionalLight directionalLight;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

vec3 calcAmbient(vec3 light_ambient)
{
    return (light_ambient * ambient);
}

vec3 calcDiffuse(vec3 light_diffuse, vec3 light_direction)
{
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, light_direction), 0.0);
    return (light_diffuse * (diff * diffuse));
}

vec3 calcSpecular(vec3 light_specular, vec3 light_direction)
{
    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    vec3 reflectDirection = reflect(light_direction, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
    return (light_specular * (spec * specular));
}

vec3 calculatePointLight(PointLight light)
{
    vec3 lightDirection = -normalize(FragPos - light.position);

    vec3 norm = normalize(Normal);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + (light.linear * distance) + (light.quadratic * (distance * distance)));

    vec3 ambientColor = calcAmbient(light.ambient) * attenuation;
    vec3 diffuseColor = calcDiffuse(light.diffuse, lightDirection) * attenuation;
    vec3 specularColor = calcSpecular(light.specular, lightDirection) * attenuation;

    return (ambientColor + diffuseColor + specularColor);
}

vec3 calculateDirectionalLight(DirectionalLight light)
{
    vec3 lightDirection = -normalize(light.direction);

    vec3 ambientColor = calcAmbient(light.ambient);
    vec3 diffuseColor = calcDiffuse(light.diffuse, lightDirection);
    vec3 specularColor = calcSpecular(light.specular, lightDirection);

    return (ambientColor + diffuseColor + specularColor);
}

void main ()
{
    vec3 finalColor = calculateDirectionalLight(directionalLight);

    for (int i = 0; i < 2; i++)
    {
        finalColor += calculatePointLight(pointLight[i]);
    }

    color = vec4(finalColor, 1.0f);
}
