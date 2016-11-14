#pragma once
#ifndef SHADER_H_
#define SHADER_H_
#include <string>

#include <GL45\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

// Compile from file, and link to program
class Shader {
public:
	// Program ID of shader
	GLuint				ID;
	Shader() {}
	Shader& use();
	// Compile shader from given source code.( We will only need 
	// vertex shader, fragment shader and optional geometry shader.
	void compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);
	// Uniform utility
	void setFloat	(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void seInteger  (const GLchar *name, GLint value, GLboolean useShader = false);
	void setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);
	void setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);
	void setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);
	void setMatrix4 (const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);

private:
	// Print error logs if compile or link failed
	// @parameter object: object of shader
	// @parameter type: "VERTEX" means vertex shader, "FRAGMENT" means fragment shader, "GEOMETRY" means geometry shader
	void checkCompileError(GLuint object, std::string type);
};
#endif // !SHADER_H__
