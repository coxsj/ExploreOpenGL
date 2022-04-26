#pragma once
#include <vector>

//External Dependencies
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

typedef glm::vec3 Point;
typedef glm::vec3 ColorRGB;
typedef glm::vec2 TextureCoord;
typedef glm::vec3 Normal;
struct Vertex {
	Point pos;
	ColorRGB colorRGB;
	TextureCoord textureCoords;
	Normal normals;
	Vertex() :pos(0, 0, 0), colorRGB(0, 0, 0), textureCoords(0, 0), normals(0, 0, 0) {}
	Vertex(Point newPos, ColorRGB newColorRGB, TextureCoord newTextureCoords, Normal newNormals = Normal(0.0f))
		:pos(newPos), colorRGB(newColorRGB), textureCoords(newTextureCoords), normals(newNormals) {}
	//NOTE: for now, normal vectors are not compared in a vertex equality comparison
	inline bool operator==(const Vertex& rhs) {
		return pos == rhs.pos
			&& colorRGB == rhs.colorRGB
			&& textureCoords == rhs.textureCoords;
	}
};
typedef std::vector<Vertex> Triangle;

class NewShape {
private:
	std::vector<Triangle> triangles;
	std::vector<Normal> normals;
protected:
	unsigned int maxTriangles;
	unsigned int shaderIndex;
public:
	NewShape() : maxTriangles(0), shaderIndex{0} {}
	bool  addTriangle(const Triangle& t) {
		if (maxTriangles >= triangles.size()) return false;
		triangles.push_back(t);
		return true;
	}
	Triangle& operator[](const unsigned int index) {
		assert(index < triangles.size());
		return triangles[index];
	}
	Vertex& vertex(const unsigned int index) {
		Vertex v;
		if (index >= 3 * triangles.size()) return v;
		return triangles[index % 3][index / 3];
	}
	virtual void determineNormals()=0;
};

class NewTriangle : public NewShape {
public:
	NewTriangle(const Triangle& t, const unsigned int newIndex=0) {
		maxTriangles = 1;
		addTriangle(t);
		shaderIndex = newIndex;
	}
	NewTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3, const unsigned int newIndex = 0) {
		maxTriangles = 1;
		addTriangle(std::move(Triangle{ v1, v2, v3 }));
		shaderIndex = newIndex;
	}
	void determineNormals() override{}
};
class NewRectangle : public NewShape {
public:
	NewRectangle(NewTriangle ta, NewTriangle tb, const unsigned int newIndex=0) {
		maxTriangles = 2;
		addTriangle(ta[0]);
		addTriangle(tb[0]);
		shaderIndex = newIndex;
	}
	NewRectangle(Triangle ta, Triangle tb, const unsigned int newIndex=0) {
		maxTriangles = 2;
		addTriangle(ta);
		addTriangle(tb);
		shaderIndex = newIndex;
	}
	void determineNormals() override{}
};
class NewCube : public NewShape {
public:
	NewCube(NewRectangle ta, NewRectangle tb, NewRectangle tc,
		NewRectangle td, NewRectangle te, NewRectangle tf,
		const unsigned int newIndex=0) {
		maxTriangles = 12;
		addTriangle(ta[0]); addTriangle(ta[1]);
		addTriangle(tb[0]); addTriangle(tb[1]);
		addTriangle(tc[0]); addTriangle(tc[1]);
		addTriangle(td[0]); addTriangle(td[1]);
		addTriangle(te[0]); addTriangle(te[1]);
		addTriangle(tf[0]); addTriangle(tf[1]);
		shaderIndex = newIndex;
	}
	void determineNormals() override {};
};

