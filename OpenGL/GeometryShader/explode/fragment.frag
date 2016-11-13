#version 330 core

struct Material{
	sampler2D texture_diffuse1;
};

in vec2 fs_TexCoords;

out vec4 color;

uniform Material material;

void main()
{    
	color = texture(material.texture_diffuse1, fs_TexCoords) + vec4(0.1f);
}

