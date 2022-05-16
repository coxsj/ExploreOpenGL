#include <assert.h>

#include "mesh.h"
#include "window.h"

void Mesh::draw(const Shader& shader){
	unsigned int diffuseCt = 1;
	unsigned int specularCt = 1;
	unsigned int normalCt = 1;
	unsigned heightCt = 1;
	for (unsigned int i = 0; i < textures.size(); i++) {
		assert(textures.size() <= Window::getOpenGLAttrbutes());
		// activate texture unit first
		glActiveTexture(GL_TEXTURE0 + i); //Leave GL_TEXTURE0 for no texture
		// Assumes naming convention: Each diffuse texture is named texture_diffuseN 
		// and each specular texture is named texture_specularN,
		// where N is any number ranging from 1 to the maximum number of texture samplers allowed.
		std::string number;
		//TODO convert string name ID's to enum class
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseCt++);
		else if (name == "texture_specular")
			number = std::to_string(specularCt++);
		shader.setInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0],
		GL_STATIC_DRAW);

	//Vertex offset pointers
	//Position
	glEnableVertexAttribArray(0); //location 0
	glVertexAttribPointer(0, Vertex::posLength(), GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)OFFSET_POS);
	//Normals
	glEnableVertexAttribArray(1); //relates to the locations declared in the vertex shader
	glVertexAttribPointer(1, Vertex::normalLength(), GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)OFFSET_NORMALS);
	//Texture coords
	glEnableVertexAttribArray(2); //relates to the locations declared in the vertex shader
	glVertexAttribPointer(2, Vertex::textureCoordLength(), GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)OFFSET_TEXTURECOORD);
	
	glBindVertexArray(0);
}