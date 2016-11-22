#pragma once
#ifndef MESH_H__
#define MESH_H__

#include <glm\glm.hpp>
#include <GL45\glew.h>
#include <string>
#include <vector>
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

#include "..\Shader\Shader.h"
struct Vertex
{
	// Position
	glm::vec3 position;
	// Normal
	glm::vec3 normal;
	// TexCoords
	glm::vec2 texCoords;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

class Mesh {
public:
	/* Mesh Data */
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	/* Functions */
	// Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices,
		std::vector<Texture> textures) :vertices(vertices), indices(indices),
		textures(textures){ setupMesh(); }
	// Render the mesh
	void draw(Shader shader);

	const GLuint getVAO() const { return VAO; }

private:
	/* Render data */
	GLuint VAO, VBO, EBO;
	/* Functions */
	// Initializes all the buffer objects/arrays
	void setupMesh();
};


#endif // !MESH_H__
