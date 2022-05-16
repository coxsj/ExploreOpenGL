#pragma once

#include <vector>

#include "shader.h"
#include "vertex.h"
#include "texture.h"

class Mesh
{
private:
	//Render data
	unsigned int VAO, VBO, EBO;
public:
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;

	Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i, std::vector<Texture>& t) :
		vertices(v), indices(i), textures(t) {	setupMesh(); }
	void draw(const Shader& shader);
private:
	void setupMesh();
};

