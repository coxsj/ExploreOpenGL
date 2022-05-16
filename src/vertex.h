#pragma once

#include <glm\glm.hpp>

typedef glm::vec3 Point;
typedef glm::vec3 ColorRGB;
typedef glm::vec2 TextureCoord2D;
typedef glm::vec3 Normal;

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normals;
	glm::vec2 textureCoord;
	static unsigned int posLength() { return sizeof(pos) / sizeof(float); }
	static unsigned int normalLength() { return sizeof(normals) / sizeof(float); }
	static unsigned int textureCoordLength() { return sizeof(textureCoord) / sizeof(float); }
};

constexpr unsigned long long int OFFSET_POS = offsetof(Vertex, pos);
constexpr unsigned long long int OFFSET_NORMALS = offsetof(Vertex, normals);
constexpr unsigned long long int OFFSET_TEXTURECOORD = offsetof(Vertex, textureCoord);
