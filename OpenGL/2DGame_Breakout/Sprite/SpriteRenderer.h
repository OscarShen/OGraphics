#pragma once
#ifndef SPRITERENDER_H__
#define SPRITERENDER_H__
#include "../Shader/Shader.h"
#include "../Texture/Texture2D.h"

#include <glm/glm.hpp>

class SpriteRenderer
{
public:
	SpriteRenderer(const Shader &shader);

	~SpriteRenderer();

	// Renders a defined quad textured with given sprite
	void drawSprite(Texture2D &texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));
private:
	// Render state
	Shader shader;
	GLuint quadVAO;
	// Initializes and configures the quad's buffer and vertex attributes
	void initRenderData();
};


#endif // !SPRITERENDER_H__
