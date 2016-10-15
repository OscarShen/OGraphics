#version 450 core

out vec4 color;

uniform vec3 lightColor;
uniform vec3 containerColor;

void main()
{
    color = vec4(lightColor*containerColor, 1.0f);
}