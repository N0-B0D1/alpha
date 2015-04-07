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
uniform vec3 lightColor[2];
uniform vec3 lightPos[2];

void main ()
{
    // static vars
    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;

    // apply global ambient light first
    vec3 finalColor;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < lightColor.length(); i++)
    {
        // calculate Ambient
        vec3 ambient = ambientStrength * lightColor[i];

        // calculate Diffuse
        vec3 lightDir = normalize(lightPos[i] - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor[i];

        // calculate Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor[i];

        finalColor += (ambient + diffuse + specular);
    }

    // finally apply object color and set result
    finalColor *= objectColor;
    color = vec4(finalColor, 1.0f);
}
