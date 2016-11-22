#version 330 core

in vec2 TexCoords;
out vec4 color;
uniform sampler2D tex1;

void main() {
	color = texture(tex1, TexCoords);
}