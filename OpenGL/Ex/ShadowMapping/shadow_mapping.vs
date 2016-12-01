#version 450 core

struct vertex {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
};

layout (std430, binding = 2) buffer VS_IN {
	vertex vertices[];
} vs_in;

uniform mat4 projection;
uniform mat4 view;

out vec2 TexCoords;

void main() {
	gl_Position = projection * view * vec4(vs_in.vertices[gl_VertexID].position, 1.0f);
	TexCoords = vs_in.vertices[gl_VertexID].texCoords;
}