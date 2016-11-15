#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <SOIL.h>
// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::textures;
std::map<std::string, Shader>		ResourceManager::shaders;

Shader ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders[name];
}

Shader ResourceManager::getShader(std::string name)
{
	if(shaders.find(name)!=shaders.end())
		return shaders[name];
	return Shader();
}

Texture2D ResourceManager::loadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	textures[name] = loadTextureFromFile(file, alpha);
	return textures[name];
}

Texture2D ResourceManager::getTexture(std::string name)
{
	if(textures.find(name)!=textures.end())
		return textures[name];
	return Texture2D();
}

void ResourceManager::clear()
{
	// (Properly) delete all shaders
	for (const auto &iter : shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (const auto &iter : textures)
		glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar * vShaderFile, const GLchar * fShaderFile, const GLchar * gShaderFile)
{
	// Retrieve the vertex/fragment source code from filepath
	std::string vertexCode, fragmentCode, geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// Close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader is usable
		if (gShaderFile != nullptr) {
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}

	const GLchar *vShaderCode = vertexCode.c_str(),
		*fShaderCode = fragmentCode.c_str(),
		*gShaderCode = geometryCode.c_str();
	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderFile == nullptr ? nullptr : gShaderCode);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar * file, GLboolean alpha)
{
	Texture2D texture;
	if (alpha) {
		texture.internal_format = GL_RGBA;
		texture.image_format = GL_RGBA;
	}
	// Load image
	int width, height, channel;
	unsigned char *image = SOIL_load_image(file, &width, &height, &channel, texture.image_format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture.generate(width, height, image);
	// Free image data
	SOIL_free_image_data(image);
	return texture;
}
