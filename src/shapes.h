#pragma once
#include <vector>

//External Dependencies
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef glm::vec3 Point;
typedef glm::vec3 ColorRGB;
typedef glm::vec2 TextureCoord2D;
typedef glm::vec3 Normal;
struct Vertex {
	Point pos;
	ColorRGB colorRGB;
	TextureCoord2D textureCoord;
	Normal normals;
	Vertex() :pos(0, 0, 0), colorRGB(0, 0, 0), textureCoord(0, 0), normals(0, 0, 0) {}
	Vertex(Point newPos) :pos(newPos), colorRGB(0, 0, 0), textureCoord(0, 0), normals(0, 0, 0) {}
	Vertex(Point newPos, ColorRGB newColorRGB, TextureCoord2D newTextureCoord, Normal newNormals = Normal(0.0f))
		:pos(newPos), colorRGB(newColorRGB), textureCoord(newTextureCoord), normals(newNormals) {}
	//NOTE: for now, normal vectors are not compared in a vertex equality comparison
	inline bool operator==(const Vertex& rhs) {
		return pos == rhs.pos
			&& colorRGB == rhs.colorRGB
			&& textureCoord == rhs.textureCoord;
	}
};
typedef std::vector<Vertex> Triangle;

class NewShape {
private:
	std::vector<Triangle> triangles_;
protected:
	unsigned int maxTriangles_;
	unsigned int shaderIndex_;
	glm::vec3 refPoint_;
public:
	NewShape() : maxTriangles_(0), shaderIndex_(0), refPoint_(Point{ 0.0f, 0.0f, 0.0f }){}
	Triangle& operator[](const unsigned int index) {
		assert(index < triangles_.size() && triangles_.size() != 0);
		return triangles_[index];
	}
	bool			addTriangle(Triangle& t, Point refPoint=Point{0.0f, 0.0f, 0.0f});
	bool			addTriangle(Point pa, Point bp, Point pc, Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
	Vertex&			vertex(const unsigned int index);
	unsigned int	shaderIndex() { return shaderIndex_; }
	unsigned int	triangles() { return static_cast<unsigned int>(triangles_.size()); }	
};


class NewTriangle : public NewShape {
public:
	NewTriangle(Triangle& t, Point refPoint=Point{0.0f, 0.0f, 0.0f}, 
		const unsigned int newIndex = 0);
	NewTriangle(Point pa, Point pb, Point pc, Point refPoint = Point{ 0.0f, 0.0f, 0.0f },
		const unsigned int newIndex = 0);
private:
	void initTriangle(Triangle& t, Point refPoint, const unsigned int newIndex);
};


class NewRectangle : public NewShape {
public:
	NewRectangle( NewTriangle& ta, NewTriangle& tb, Point refPoint,
		const unsigned int newIndex=0) { initRectangle(ta[0], tb[0], refPoint, newIndex);}
	NewRectangle(Triangle ta, Triangle tb, Point refPoint = Point{ 0.0f, 0.0f, 0.0f },
		const unsigned int newIndex = 0) { initRectangle(ta, tb, refPoint, newIndex);}
	NewRectangle(Point pa, Point pb, Point pc, Point pd,
		Point refPoint = Point{ 0.0f, 0.0f, 0.0f },
		const unsigned int newIndex = 0);
private:
	void initRectangle(Triangle& ta, Triangle& tb, Point refPoint, const unsigned int newIndex);
	
};
class NewCube : public NewShape {
	
public:
	NewCube(NewRectangle ta, NewRectangle tb, NewRectangle tc,
		NewRectangle td, NewRectangle te, NewRectangle tf,
		const unsigned int newIndex=0) {
		maxTriangles_ = 12;
		addTriangle(ta[0]); addTriangle(ta[1]);
		addTriangle(tb[0]); addTriangle(tb[1]);
		addTriangle(tc[0]); addTriangle(tc[1]);
		addTriangle(td[0]); addTriangle(td[1]);
		addTriangle(te[0]); addTriangle(te[1]);
		addTriangle(tf[0]); addTriangle(tf[1]);
		shaderIndex_ = newIndex;
	}
	
};

class Geometry {
private:
	Geometry();
public:
	static void assignNormals(Triangle& t, Point& refPoint); 
	static bool extractTrianglesFromRectangle(Point pa, Point pb, Point pc, Point pd, Triangle& ta, Triangle& tb, Point refPoint);
	static bool normalCorrect(Point p, Normal n, Point refPoint);
	static bool refPtNotInTrianglePlane(Triangle& t, Point refPoint);
	static bool verifyRectangle(Triangle& ta, Triangle& tb);
	static bool verifyRectangle(Point a, Point b, Point c, Point d);
};