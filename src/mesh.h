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

	Mesh(const std::vector<Vertex>& v, const std::vector<unsigned int>& i,
		const std::vector<Texture>& t) : vertices(v), indices(i), textures(t) {	setupMesh(); }
	void draw(Shader& shader);
private:
	//Render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

