#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;

//layout (std140) uniform instanceMatrix{
//	mat4 matrix[1000];
//}Instance;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(position, 1.0f);
    //gl_Position = projection * view * Instance.matrix[gl_InstanceID] * vec4(position, 1.0f);
    TexCoords = texCoords;
}