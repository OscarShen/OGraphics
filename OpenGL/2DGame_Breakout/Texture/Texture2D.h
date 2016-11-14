#pragma once
#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <GL45\glew.h>

// Store and configure 2d texture in OpenGL
class Texture2D {
public:
	// ID of 2d texture object
	GLuint				ID;
	// Texture width and height
	GLuint				width, height;
	// Texture format
	GLuint				internal_format; // Format of texture object
	GLuint				image_format; // Format of loaded image
	// Texture configuration
	GLuint				wrap_S; // Wrapping mode on S axis
	GLuint				wrap_T; // Wrapping mode on T axis
	GLuint				filter_min; // Filtering mode if texture pixels < screen pixels
	GLuint				filter_max; // Filtering mode if texture pixels > screen pixels

	Texture2D();
	// Generate texture from image data
	void generate(GLuint width, GLuint height, unsigned char *data);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void bind() const;
};

#endif // !TEXTURE_H__
