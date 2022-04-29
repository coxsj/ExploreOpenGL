#pragma once
#include <vector>

//External Dependencies
#include <glm\glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

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
			//Note: Does not compare normals as they are solid shape dependent
	}
	static unsigned int posLength() { return sizeof(pos)/sizeof(float); }
	static unsigned int colorRGBLength() { return sizeof(colorRGB) / sizeof(float); }
	static unsigned int textureCoordLength() { return sizeof(textureCoord) / sizeof(float); }
	static unsigned int normalLength() { return sizeof(normals) / sizeof(float); }
};
typedef std::vector<Vertex> Triangle;

class NewShape {
private:
	std::vector<Triangle> triangles_;
protected:
	unsigned int maxTriangles_;
	unsigned int shaderIndex_;
	Point refPoint_;
public:
	NewShape() : maxTriangles_(0), shaderIndex_(0), refPoint_(Point{ 0.0f, 0.0f, 0.0f }){}
	Triangle& operator[](const unsigned int index) {
		assert(index < triangles_.size() && triangles_.size() != 0);
		return at(index);
	}
	bool			addTriangle(Triangle& t, Point refPoint=Point{0.0f, 0.0f, 0.0f});
	bool			addTriangle(Point pa, Point bp, Point pc, Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
	Triangle&		at(unsigned int index) {
		assert(index < triangles_.size() && triangles_.size() != 0);
		return triangles_[index];
	}
	Vertex&			vertex(const unsigned int index);
	unsigned int	shaderIndex() { return shaderIndex_; }
	//unsigned int	triangles() { return static_cast<unsigned int>(triangles_.size()); }	
	unsigned int	size() { return static_cast<unsigned int>(triangles_.size()); }
};


class NewTriangle : public NewShape {
public:
	NewTriangle(Triangle& t, const unsigned int newIndex = 0,
		Point refPoint=Point{0.0f, 0.0f, 0.0f });
	NewTriangle(Point pa, Point pb, Point pc, const unsigned int newIndex = 0,
		Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
private:
	void initTriangle(Triangle& t, Point refPoint, const unsigned int newIndex);
};


class NewRectangle : public NewShape {
public:
	NewRectangle(NewTriangle& ta, NewTriangle& tb, const unsigned int newIndex=0, 
		Point refPoint = Point{ 0.0f, 0.0f, 0.0f }) { initRectangle(ta[0], tb[0], refPoint, newIndex);}
	NewRectangle(Triangle ta, Triangle tb, const unsigned int newIndex = 0, 
		Point refPoint = Point{ 0.0f, 0.0f, 0.0f }) { initRectangle(ta, tb, refPoint, newIndex);}
	NewRectangle(Point pa, Point pb, Point pc, Point pd,
		const unsigned int newIndex = 0,
		Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
private:
	void initRectangle(Triangle& ta, Triangle& tb, Point refPoint, const unsigned int newIndex);
};

// NewCube
//*************************************
class NewCube : public NewShape {
public:
	NewCube(std::vector<NewRectangle>& rect, const unsigned int newIndex = 0, 
		Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
};

class Geometry {
private:
	Geometry();
public:
	static bool allPointsSamePlane(Point a, Point b, Point c, Point d);
	static bool allPointsUnique(Point a, Point b, Point c);
	static bool allPointsUnique(Triangle& t, Point refPoint);
	static bool allPointsUnique(Point a, Point b, Point c, Point d);
	static bool allPointsUnique(Point a, Point b, Point c, Point d, Point e);
	static void assignNormals(Triangle& t, Point& refPoint);
	static bool extractTrianglesFromRectangle(Point pa, Point pb, Point pc, Point pd, Triangle& ta, Triangle& tb);
	static Normal getNormal(Point a, Point b, Point c);
	static bool normalCorrect(Point p, Normal n, Point refPoint);
	static bool verifyRectangle(Triangle& ta, Triangle& tb);
	static bool verifyRectangle(Point a, Point b, Point c, Point d);
};