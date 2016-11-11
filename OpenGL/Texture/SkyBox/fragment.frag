#version 450 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

//uniform sampler2D texture1;
uniform vec3 cameraPos;
uniform samplerCube skybox;

out vec4 color;

void main(){
	//color = texture(texture1, TexCoords);
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	//vec3 R = refract(I, normalize(Normal),1.308);
	color = texture(skybox, R);
}