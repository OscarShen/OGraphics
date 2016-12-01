#version 450 core

in vec2 TexCoords;

uniform sampler2D tex1;

out vec4 outColor;

void main() {
	outColor = texture(tex1, TexCoords);
}