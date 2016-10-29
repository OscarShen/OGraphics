#pragma once
#ifndef MODEL_H__
#define MODEL_H__
#include <vector>
#include <string>
#include "Mesh.h"
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>

class Model
{
public:
	/* Functions */
	// Constructor, expects a filepath to a 3D model
	Model(GLchar *path) { this->loadModel(path); }
	// Draw the model, and thus all its meshes
	void draw(Shader shader);

private:
	/* Model data */
	std::vector<Mesh> meshes;
	/* Model directory */
	std::string directory;
	/* Stores all the textures loaded so far, potimization to make sure texture aren't loaded more than once.*/
	std::vector<Texture> textures_loaded;

	/* Functions */
	// Loads a model with supported ASSIMP extends form file and 
	// stores the resulting meshes in the meshes vector.
	void loadModel(std::string path);

	// Process each individual mesh located at the node and 
	// repeats this process on its children.
	void processNode(aiNode *node, const aiScene *scene);
	// The actually processes of mesh
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// Checks all material textures of a given type and loads
	// the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	std::vector<Texture> loadMaterialTextures(
		aiMaterial *mat, aiTextureType type, std::string typeName);
};
// Load texture from assigned file
GLint textureFromFile(const char *path, std::string directory);

#endif // !MODEL_H__
