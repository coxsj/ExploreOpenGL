#include "model.h"

#include "stb_image.h"

#include "mesh.h"
#include "shapes.h"
#include "utility.h"

//Needed to avoid linker error LNK2001 unresolved external symbol
std::vector<Texture> Model::textures_loaded;

void Model::draw(const Shader& shader) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader);
	}
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	std::string typeName) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool found = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(),
				str.C_Str()) == 0) {
				//Found match, load from textures_loaded
				textures.push_back(textures_loaded[j]);
				found = true;
				break;
			}
		}
		if (!found) {
			Texture texture;
			texture.id = textureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}
void Model::loadModel(const std::string& directoryPath, const std::string& filename) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(directoryPath + filename, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		assert(false);
		return;
	}
	directory = directoryPath;
	// Extract meshes
	processNode(scene->mRootNode, scene);
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Point point{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
		Normal normal{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

		// Assimp allows a model to have up to 8 different texture coordinates per vertex.
		// We only care about the first set of texture coordinates.
		// Must check if the mesh actually contains texture coordinates
		// (which may not be always the case)
		TextureCoord2D texture2D;
		if (mesh->mTextureCoords[0]){
			texture2D.x = mesh->mTextureCoords[0][i].x;
			texture2D.y = mesh->mTextureCoords[0][i].y;
		}
		else
			texture2D = TextureCoord2D(0.0f, 0.0f);

		vertices.emplace_back(point, texture2D, normal);
	}
	// Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// Process textures
	if (!mesh->mMaterialIndex == 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
			"texture_diffuse");
		// Insert member fxn adds each of the textures in diffuseMaps to the end of textures vector
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
			"texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}
void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// Now do the same thing for each of the node's children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}
