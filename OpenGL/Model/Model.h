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
	Model(GLchar *path) { this->loadModel(path); }
	void draw(Shader shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(
		aiMaterial *mat, aiTextureType type, std::string typeName);
};
GLint textureFromFile(const char *path, std::string directory);

#endif // !MODEL_H__
