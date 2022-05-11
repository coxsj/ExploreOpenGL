#pragma once

#include <iostream>
#include <vector>
#include <map>

//External Dependencies
#include <glm\glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "settings.h"

typedef glm::vec3 Point;
typedef glm::vec3 ColorRGB;
typedef glm::vec2 TextureCoord2D;
typedef glm::vec3 Normal;

struct Vertex {
	Point pos;
	Normal normals;
	TextureCoord2D textureCoord;
	//Vertex() :pos(0, 0, 0), colorRGB(0, 0, 0), textureCoord(0, 0), normals(0, 0, 0) {}
	Vertex(Point newPos) :pos(newPos), textureCoord(0, 0), normals(0, 0, 0) {}
	Vertex(Point newPos, TextureCoord2D newTextureCoord, Normal newNormals = Normal(0.0f))
		:pos(newPos), textureCoord(newTextureCoord), normals(newNormals) {}
	//NOTE: for now, normal vectors are not compared in a vertex equality comparison
	inline bool operator==(const Vertex& rhs) {
		return pos == rhs.pos
			&& textureCoord == rhs.textureCoord;
			//Note: Does not compare normals as they are solid shape dependent
	}
	friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
		os << "(" << v.pos.x << "," << v.pos.y << "," << v.pos.z << ")";
		return os;
	}
	void add(std::vector<float>& v);
	static unsigned int posLength() { return sizeof(pos)/sizeof(float); }
	static unsigned int textureCoordLength() { return sizeof(textureCoord) / sizeof(float); }
	static unsigned int normalLength() { return sizeof(normals) / sizeof(float); }
};
constexpr unsigned long long int OFFSET_POS = offsetof(Vertex, pos);
constexpr unsigned long long int OFFSET_TEXTURECOORD = offsetof(Vertex, textureCoord);
constexpr unsigned long long int OFFSET_NORMALS = offsetof(Vertex, normals);

typedef std::vector<Vertex> Triangle;

enum class SHAPE_TYPE {
	e_shape,
	e_triangle,
	e_rectangle,
	e_cube
};
class NewShape {
private:
	std::vector<Triangle> triangles_;
	SHAPE_TYPE shapeType_;
	unsigned int maxTriangles_;
protected:
	unsigned int shaderIndex_;
	Point refPoint_;
	std::string name_;
public:
	NewShape() : maxTriangles_(0), shaderIndex_(0), refPoint_(Point{ 0.0f, 0.0f, 0.0f }),
		shapeType_(SHAPE_TYPE::e_shape), name_(UNNAMED) {}
	Triangle& operator[](const unsigned int index) {
		assert(index < triangles_.size() && triangles_.size() != 0);
		return at(index);
	}
	bool			addTriangle(Triangle& t, Point refPoint=Point{0.0f, 0.0f, 0.0f});
	Triangle&		at(unsigned int index) {
		assert(index < triangles_.size() && triangles_.size() != 0);
		return triangles_[index];
	}
	unsigned int	maxTriangles() { return maxTriangles_; }
	void			newShapeType(SHAPE_TYPE s) { shapeType_ = s; setMaxTriangles(s); }
	void			printName() {std::cout << name_;}
	void			printNameln() { printName(); std::cout << std::endl; }
	void			printShapeType();
	void			printShapeTypeln() { printShapeType(); std::cout << std::endl; }
	static void		printGLMVec3(const glm::vec3& v);
	static void		printGLMVec3ln(const glm::vec3& v) { printGLMVec3(v); std::cout << std::endl; }
	static void		printTriangle(const Triangle& t);
	static void		printTriangleln(const Triangle& t) { printTriangle(t); std::cout << std::endl; }
	unsigned int	shaderIndex() { return shaderIndex_; }	
	unsigned int	size() { return static_cast<unsigned int>(triangles_.size()); }
	Vertex&			vertex(const unsigned int index);
	unsigned int	vertexCount() { return size() * VERTICES_PER_TRIANGLE; }
private:
	void			setMaxTriangles(SHAPE_TYPE s);
	};


class NewTriangle : public NewShape {
public:
	NewTriangle(Triangle& t, const std::string& name = UNNAMED, const unsigned int newIndex = 0,
		Point refPoint = Point{0.0f, 0.0f, 0.0f });
	NewTriangle(Point pa, Point pb, Point pc, const std::string& name, const unsigned int newIndex = 0,
		Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
private:
	void initTriangle(Triangle& t, const std::string& name, const unsigned int newIndex, Point refPoint);
};


class NewRectangle : public NewShape {
public:
	NewRectangle(NewTriangle& ta, NewTriangle& tb, const std::string& name = UNNAMED,
		const unsigned int newIndex=0, Point refPoint = Point{ 0.0f, 0.0f, 0.0f }) {
		initRectangle(ta[0], tb[0], name, newIndex, refPoint);
	}
	NewRectangle(Triangle ta, Triangle tb, const std::string& name = UNNAMED,
		const unsigned int newIndex = 0, Point refPoint = Point{ 0.0f, 0.0f, 0.0f }) { 
		initRectangle(ta, tb, name, newIndex, refPoint);
	}
	NewRectangle(Point pa, Point pb, Point pc, Point pd, const std::string& name,
		const unsigned int newIndex = 0, Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
private:
	void initRectangle(Triangle& ta, Triangle& tb, const std::string& name, const unsigned int newIndex, Point refPoint);
};

// NewCube
//*************************************
class NewCube : public NewShape {
public:
	NewCube(std::vector<Triangle>& t, const std::string& name,
		const unsigned int newIndex = 0, Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
	NewCube(std::vector<NewRectangle>& rect, const std::string& name,
		const unsigned int newIndex = 0, Point refPoint = Point{ 0.0f, 0.0f, 0.0f });
	void initCube(const unsigned int newIndex);
};

class Geometry {
private:
	Geometry();
public:
	static bool allPointsSamePlane(const Point& a, const Point& b, const Point& c, const Point& d);
	static bool allPointsUnique(const Point& a, const Point& b);
	static bool allPointsUnique(const Point& a, const Point& b, const Point& c);
	static bool allPointsUnique(const Triangle& t, const Point& refPoint);
	static bool allPointsUnique(const Point& a, const Point& b, const Point& c, const Point& d);
	static bool allPointsUnique(const Point& a, const Point& b, const Point& c, const Point& d,
		const Point& e);
	static void assignNormals(Triangle& t, const Point& refPoint);
	static bool extractTrianglesFromRectangle(const Point& pa, const Point& pb, const Point& pc,
		const Point& pd, Triangle& ta, Triangle& tb);
	static Normal getNormal(const Point& a, const Point& b, const Point& c);
	static bool normalCorrect(const Point& p, const Normal& n, const Point& refPoint);
	static void printShapePlane(const Triangle& t);
	static bool verifyRectangle(const Triangle& ta, const Triangle& tb);
	static bool verifyRectangle(const Point& a, const Point& b, const Point& c, const Point& d);
};