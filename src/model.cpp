

#include "model.h"

#include "stb_image.h"

#include "mesh.h"
#include "shapes.h"
#include "utility.h"

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
		Texture texture;
		texture.id = textureFromFile(str.C_Str(), directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}
void Model::loadModel(const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		assert(false);
		return;
	}
	directory = path;
	// Extract meshes
	processNode(scene->mRootNode, scene);
}
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Process vertices
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Point point{ mesh[i].mVertices->x, mesh[i].mVertices->y, mesh[i].mVertices->z };
		Normal normal{ mesh[i].mNormals->x, mesh[i].mNormals->y, mesh[i].mNormals->z };

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
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// Now do the same thing for each of the node's children
	for (int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}
