#pragma once

#include <string>
#include <vector>

#include <assimp\assimp\include\assimp\Importer.hpp>
#include <assimp\assimp\include\assimp\scene.h>
#include <assimp\assimp\include\assimp\postprocess.h>

#include "mesh.h"
#include "shader.h"
#include "texture.h"

class Model
{
private:
	std::vector<Mesh> meshes;
	std::string directory;
	static std::vector<Texture> textures_loaded;
public:
	Model(const std::string& directoryPath, const std::string& filename) { 
		loadModel(directoryPath, filename); 
	}
	void draw(const Shader& shader);
private:
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
	void loadModel(const std::string& directoryPath, const std::string& filename);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
};

