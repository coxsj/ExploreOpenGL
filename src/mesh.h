#pragma once

#include <vector>

#include "shader.h"
#include"shapes.h"

class Mesh
{
public:
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<Texture>		textures;

	Mesh(std::vector<Vertex>& v, std::vector<unsigned int>& i,
		std::vector<Texture>& t) : vertices(v), indices(i), textures(t) {	setupMesh(); }
	void draw(const Shader& shader);
private:
	//Render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

