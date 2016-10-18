#version 450 core

out vec4 color;

uniform vec3 lightColor;
uniform vec3 containerColor;

void main()
{
	float ambientColor = 0.3f;
    color = vec4(lightColor*containerColor*ambientColor, 1.0f);
}