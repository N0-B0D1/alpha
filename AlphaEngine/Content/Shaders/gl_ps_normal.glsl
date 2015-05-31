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

uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightPos[2];
uniform vec3 lightAmbient[2];
uniform vec3 lightDiffuse[2];
uniform vec3 lightSpecular[2];
uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main ()
{
    // apply global ambient light first
    vec3 finalColor = vec3(0.0f);

    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    for (int i = 0; i < 2; i++)
    {
        // calculate Ambient
        vec3 ambientColor = lightAmbient[i] * ambient;

        // calculate Diffuse
        vec3 lightDirection = -normalize(FragPos - lightPos[i]);
        float diff = max(dot(norm, lightDirection), 0.0);
        vec3 diffuseColor = lightDiffuse[i] * (diff * diffuse);

        // calculate Specular
        vec3 reflectDirection = reflect(lightDirection, norm);
        float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), shininess);
        vec3 specularColor = lightSpecular[i] * (spec * specular);

        finalColor += (ambientColor + diffuseColor + specularColor);
    }

    color = vec4(finalColor, 1.0f);
}
