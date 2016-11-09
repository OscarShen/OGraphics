#pragma once
#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <GL45\glew.h>
#include <SOIL.h>
#include <iostream>
#include <vector>

#include "../Tools/TGALoader.h"

GLuint loadTexture(GLchar * path, GLboolean loadAlfa = GL_FALSE, GLenum param = GL_REPEAT)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char *image;
	if (loadAlfa) {
		image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);

		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	else {
		image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);

		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	std::cout << width << std::endl;
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	return textureID;
}

#endif // !TEXTURE_H__
