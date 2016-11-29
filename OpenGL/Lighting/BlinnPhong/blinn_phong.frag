#version 330 core

in VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool blinn;
uniform sampler2D floorTexture;

out vec4 FragColor;

void main() {
	vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;
	// Ambient
	vec3 ambient = color * 0.05f;
	// Diffuse
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;
	// Specular
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	float spec = 0.0f;

	if(blinn) {
		vec3 reflectDir = reflect(-lightDir, normal);
		spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	}else{
		vec3 halfwayDir = normalize(viewDir + lightDir);
		spec = pow(dot(normal, halfwayDir), 32.0f);
	}
	vec3 specular = vec3(0.3f) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}