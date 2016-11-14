#include "Shader.h"
#include <iostream>
Shader& Shader::use() {
	glUseProgram(this->ID);
	return *this;
}



void Shader::compile(const GLchar * vertexSource, const GLchar * fragmentSource, const GLchar * geometrySource)
{
	// Vertex shader, fragment shader and geometry shader
	GLuint vs, fs, gs;
	// Vertex shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexSource, nullptr);
	glCompileShader(vs);
	checkCompileError(vs, "VERTEX");
	// Fragment shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentSource, nullptr);
	glCompileShader(fs);
	checkCompileError(vs, "FRAGMENT");
	// Geometry shader
	if (geometrySource != nullptr) {
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gs, 1, &geometrySource, nullptr);
		glCompileShader(gs);
		checkCompileError(gs, "GEOMETRY");
	}
	// Shader program
	this->ID = glCreateProgram();
	glAttachShader(this->ID, vs);
	glAttachShader(this->ID, fs);
	if (geometrySource != nullptr)
		glAttachShader(this->ID, gs);
	glLinkProgram(this->ID);
	checkCompileError(this->ID, "PROGRAM");
	// Delete shaders
	glDeleteShader(vs);
	glDeleteShader(fs);
	if (geometrySource != nullptr)
		glDeleteShader(gs);
}

void Shader::setFloat(const GLchar * name, GLfloat value, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}

void Shader::seInteger(const GLchar * name, GLint value, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}

void Shader::setVector2f(const GLchar * name, GLfloat x, GLfloat y, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}

void Shader::setVector2f(const GLchar * name, const glm::vec2 & value, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}

void Shader::setVector3f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}

void Shader::setVector3f(const GLchar * name, const glm::vec3 & value, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);

}

void Shader::setVector4f(const GLchar * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);

}

void Shader::setVector4f(const GLchar * name, const glm::vec4 & value, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix4(const GLchar * name, const glm::mat4 & matrix, GLboolean useShader)
{
	if (useShader)
		this->use();
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileError(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(object, 1024, nullptr, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else {
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(object, 1024, nullptr, infoLog);
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}
