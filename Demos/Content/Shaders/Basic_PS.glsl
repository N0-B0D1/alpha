#version 330 core
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main ()
{
    vec3 ambient = 0.2f * lightColor;
    color = vec4(ambient * objectColor, 1.0f);
}
