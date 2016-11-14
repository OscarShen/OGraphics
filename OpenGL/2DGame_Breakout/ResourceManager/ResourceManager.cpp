#include "ResourceManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <SOIL.h>
// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>		ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	if(Shaders.find(name)!=Shaders.end())
		return Shaders[name];
	return Shader();
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	if(Textures.find(name)!=Textures.end())
		return Textures[name];
	return Texture2D();
}

void ResourceManager::Clear()
{
	// (Properly) delete all shaders
	for (const auto &iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (const auto &iter : Textures)
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
