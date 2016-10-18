#version 450 core
layout (location = 0) in vec3 position;
layout (location = 0) in vec3 nomal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Nomal;

void main()
{
    gl_Position = projection * view * model * vec4(position,1.0f);
	Nomal=nomal;
}