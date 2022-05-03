#include <iostream>

#include "shapes.h"

bool NewShape::addTriangle(Triangle& t, Point refPoint) {
	if (t.size() != 3 || triangles_.size() >= maxTriangles_) return false;
	refPoint_ = refPoint;
	std::cout << "Adding triangle "; printTriangle(t); 
	std::cout << " with refPoint: "; NewShape::printGLMVec3(refPoint_);
	std::cout << " to " << name_ << "(";
	printShapeType(); std::cout << ")\n";
	Geometry::assignNormals(t, refPoint);
	triangles_.push_back(t);
	return true;
}
void NewShape::printShapeType() {
	switch (shapeType_) {
	case SHAPE_TYPE::e_shape:		std::cout << "base_shape";		break;
	case SHAPE_TYPE::e_triangle:	std::cout << "triangle";	break;
	case SHAPE_TYPE::e_rectangle:	std::cout << "rectangle";	break;
	case SHAPE_TYPE::e_cube:		std::cout << "cube";		break;
	default:
		break;
	}
}
void NewShape::printGLMVec3(const glm::vec3& v) {
	std::cout << v.x << "," << v.y << "," << v.z;
}
void NewShape::printTriangle(const Triangle& t) {
	std::cout << "(" << t[0] << ", " << t[1] << ", " << t[2] << ")";
}
Vertex& NewShape::vertex(const unsigned int index) {
	assert(index < 3 * triangles_.size());
	return triangles_[index / 3][index % 3];
}
void NewShape::setMaxTriangles(SHAPE_TYPE s) {
	switch (shapeType_) {
	case SHAPE_TYPE::e_shape:		maxTriangles_ = 0;	break;
	case SHAPE_TYPE::e_triangle:	maxTriangles_ = 1;	break;
	case SHAPE_TYPE::e_rectangle:	maxTriangles_ = 2;	break;
	case SHAPE_TYPE::e_cube:		maxTriangles_ = 12;	break;
	default:
		break;
	}
}
// NewTriangle Members
// ===================
NewTriangle::NewTriangle(Triangle& t, const std::string& name, const unsigned int newIndex, Point refPoint) {
	initTriangle(t, name, newIndex, refPoint);
}
NewTriangle::NewTriangle(Point pa, Point pb, Point pc, const std::string& name = "", const unsigned int newIndex,
	Point refPoint) {
	Vertex va{ pa };
	Vertex vb{ pb };
	Vertex vc{ pc };
	Triangle t{ va, vb, vc };
	initTriangle(t, name, newIndex, refPoint);
}
void NewTriangle::initTriangle(Triangle& t, const std::string& name, const unsigned int newIndex, Point refPoint) {
	name_ = name;
	newShapeType(SHAPE_TYPE::e_triangle);
	addTriangle(t, refPoint);
	assert(size() == 1);
	shaderIndex_ = newIndex;
}

// NewRectangle Members
// ====================
NewRectangle::NewRectangle(Point pa, Point pb, Point pc, Point pd, const std::string& name,
	const unsigned int newIndex, Point refPoint) {
	Triangle ta, tb;
	//Create the two triangles that form this rectangle
	assert(Geometry::extractTrianglesFromRectangle(pa, pb, pc, pd, ta, tb));
	initRectangle(ta, tb, name, newIndex, refPoint);
}
void NewRectangle::initRectangle(Triangle& ta, Triangle& tb, const std::string& name, const unsigned int newIndex,
	Point refPoint) {
	if (Geometry::verifyRectangle(ta, tb)) {
		name_ = name;
		newShapeType(SHAPE_TYPE::e_rectangle);
		addTriangle(ta, refPoint);
		addTriangle(tb, refPoint);
		assert(size() == 2);
		shaderIndex_ = newIndex;
	}
}


// NewCube Members
// ===============
NewCube::NewCube(std::vector<Triangle>& t, const std::string& name, 
	const unsigned int newIndex, Point refPoint) {
	assert(t.size() == 12);
	assert(t[0].size() == 3 && t[1].size() == 3 && t[2].size() == 3
		&& t[3].size() == 3 && t[4].size() == 3 && t[5].size() == 3
		&& t[6].size() == 3 && t[7].size() == 3 && t[8].size() == 3
		&& t[9].size() == 3 && t[10].size() == 3 && t[11].size() == 3);
	name_ = name;
	newShapeType(SHAPE_TYPE::e_cube);
	for (Triangle& triangle : t) addTriangle(triangle, refPoint);
	initCube(newIndex);
}

NewCube::NewCube(std::vector<NewRectangle>& rect, const std::string& name,
	const unsigned int newIndex, Point refPoint) {
	assert(rect.size() == 6);
	assert(rect[0][0].size() == 3 && rect[0][1].size() == 3 && rect[1][0].size() == 3
		&& rect[1][1].size() == 3 && rect[2][0].size() == 3 && rect[2][1].size() == 3
		&& rect[3][0].size() == 3 && rect[3][1].size() == 3 && rect[4][0].size() == 3
		&& rect[4][1].size() == 3 && rect[5][0].size() == 3 && rect[5][1].size() == 3);
	name_ = name;
	newShapeType(SHAPE_TYPE::e_cube);
	for (NewRectangle& r : rect) {
		addTriangle(r[0], refPoint);
		addTriangle(r[1], refPoint);
	}
	initCube(newIndex);
}
void NewCube::initCube(const unsigned int newIndex) {
	assert(size() == 12);
	shaderIndex_ = newIndex;
}


// Geometry Class Member Methods
//==============================
bool Geometry::allPointsSamePlane(const Point& a, const Point& b, const Point& c, const Point& d){
	//Compare normals of two different triangles sharing two points
	// Normals will be same or opposite direction if same plane
	glm::vec3 normal1 = getNormal(a, b, c);
	glm::vec3 normal2 = getNormal(a, b, d);
	return normal1 == normal2 || normal1 == ( - 1.0f * normal2);
}
bool Geometry::allPointsUnique(const Point& a, const Point& b) {
	//3 unique points
	return !(a.x == b.x && a.y == b.y && a.z == b.z);
}
bool Geometry::allPointsUnique(const Point& a, const Point& b, const Point& c) {
	//3 unique points
	return allPointsUnique(a, b) && allPointsUnique(a,c) && allPointsUnique(b, c);
}
bool Geometry::allPointsUnique(const Triangle& t, const Point& refPoint) {
	return allPointsUnique(t[0].pos, t[1].pos, t[2].pos, refPoint);
}
bool Geometry::allPointsUnique(const Point& a, const Point& b, const Point& c, const Point& d) {
	//4 unique points
	return allPointsUnique(a, b, c) 
		&& allPointsUnique(a, d) && allPointsUnique(b, d) && allPointsUnique(c, d);
}
bool Geometry::allPointsUnique(const Point& a, const Point& b, const Point& c, const Point& d
	, const Point& e) {
	//5 unique points
	return allPointsUnique(a, b, c, d)
		&& allPointsUnique(a, e) && allPointsUnique(b, e) 
		&& allPointsUnique(c, e) && allPointsUnique(d, e);
}
void Geometry::assignNormals(Triangle& t, const Point& refPoint) {
	//The refPoint should not be in the plane of the triangle
	// Make sure the ref poin is not the same as the vertices

	if (allPointsSamePlane(t[0].pos, t[1].pos, t[2].pos, refPoint)
		|| !allPointsUnique(t[0].pos, refPoint)
		|| !allPointsUnique(t[1].pos, refPoint) 
		|| !allPointsUnique(t[2].pos, refPoint)) {
		std::cout << "Warning: refPoint (";
		NewShape::printGLMVec3(refPoint);
		std::cout << ") in plane of triangle ";
		NewShape::printTriangleln(t);
	}
	//Normals are always assigned to vertices in this routine
	glm::vec3 v1 = t[1].pos - t[0].pos;
	glm::vec3 v2 = t[2].pos - t[0].pos;
	glm::vec3 normal = glm::normalize(glm::cross(v2, v1));
	//Compare normal and a vertex to reference point to determine if normal points away from ref point
	//If not, reverse normal
	if (!normalCorrect(t[0].pos, normal, refPoint)) {
		normal *= -1;
		std::cout << "(Corrected) ";
	}
	for (Vertex& v : t) v.normals = normal;
	std::cout << "Normal: "; NewShape::printGLMVec3(normal);
	std::cout << " "; printShapePlane(t); std::cout << std::endl;
}
bool Geometry::extractTrianglesFromRectangle(const Point& pa, const Point& pb, const Point& pc,
	const Point& pd, Triangle& ta, Triangle& tb) {
	//Determine the two conjoined rightangle triangles

	// Points must specify a rectangle
	if (!Geometry::verifyRectangle(pa, pb, pc, pd)) return false;

	// Specify first triangle then determine points of second triangle
	ta[0].pos = pa; ta[1].pos = pb; ta[2].pos = pc;
	tb[2].pos = pd;
	// Determine which are the two shared vertices
	glm::vec3 vab = pb - pa;
	glm::vec3 vbc = pc - pb;
	glm::vec3 vca = pa - pc;
	if (glm::dot(vab, vca) == 0) {
		//a is rightangle
		tb[0].pos = pb; 
		tb[1].pos = pc;
	}
	else if (glm::dot(vbc, vab) == 0) {
		//b is rightangle
		tb[0].pos = pa; 
		tb[1].pos = pc;
	}
	else if (glm::dot(vbc, vca) == 0) {
		//c is rightangle
		tb[0].pos = pa; 
		tb[1].pos = pb;
	}
	else return false;
	return true;
}
Normal Geometry::getNormal(const Point& a, const Point& b, const Point& c) {
	glm::vec3 v1 = a - b;
	glm::vec3 v2 = a - c;
	return glm::normalize(glm::cross(v2, v1));
}
bool Geometry::normalCorrect(const Point& p, const Normal& n, const Point& refPoint) {
	glm::vec3 pointToRef = refPoint - p;
	//a.b = ||a||*||b||cos(theta) where theta is the angle between a and b
	//If a.b is negative, the vectors point away from each other
	return glm::dot(pointToRef, n) <= 0;
}
void Geometry::printShapePlane(const Triangle& t) {
	std::cout << "Object plane: ";
	if ((t[0].pos.x == t[1].pos.x) && (t[0].pos.x == t[2].pos.x))
		std::cout << "x = " << t[0].pos.x;
	else if ((t[0].pos.y == t[1].pos.y) && (t[0].pos.y == t[2].pos.y))
		std::cout << "y=" << t[0].pos.y;
	else if ((t[0].pos.z == t[1].pos.z) && (t[0].pos.z == t[2].pos.z))
		std::cout << "z=" << t[0].pos.z;
	else std::cout << " not calculated";
}
bool Geometry::verifyRectangle(const Point& a, const Point& b, const Point& c, const Point& d) {
	//4 unique points
	if (!allPointsUnique(a,b,c,d)) return false;
	// All points all in same plane
	if(!allPointsSamePlane(a, b, c, d)) return false;
	// 4 rightangles formed by sides if rectangle
	// 1 rightangle formed by the diagonals if a square
	std::vector<glm::vec3> v{ a - b, a - c, a - d, b - c, b - d, c - d };
	unsigned int rightAngleCt{ 0 };
	for (int i = 0; i < v.size()-2; i++) {
		for (int j = i+1; j < v.size(); j++) {
			float current = glm::dot(v[i], v[j]);
			if (current == 0) rightAngleCt++;
			//std::cout << current << ", ";
		}
	}
	//std::cout << std::endl;
	if (rightAngleCt < 4) return false;
	return true;
};
bool Geometry::verifyRectangle(const Triangle& ta, const Triangle& tb) {
	// Triangles must have three pts
	if (ta.size() != 3 || tb.size() != 3) return false;
	
	// Points in each triangle must be unique
	if (!allPointsUnique(ta[0].pos, ta[1].pos, ta[2].pos) ||
		!allPointsUnique(tb[0].pos, tb[1].pos, tb[2].pos)) return false;
	
	// Triangles must share 2 points in common
	unsigned int sharedPts = 0;
	unsigned int tbLastIndex = 3; //Sum of three indices is 0 + 1 + 2 = 3
	std::vector<Point> rect;
	for (int i = 0; i < ta.size(); i++) {
		bool matched = false;
		for (int j = 0; j < tb.size(); j++) {
			if (ta[i].pos == tb[j].pos) {
				//Found shared point between both triangles
				sharedPts++;
				matched = true;
				rect.push_back(tb[j].pos);
				tbLastIndex -= j;
			}
		}
		//Was this point in ta not found in tb?
		if(!matched) rect.push_back(ta[i].pos);
	}
	//Push back the last unused tb point
	rect.push_back(tb[tbLastIndex].pos);
	if (sharedPts == 2 && rect.size() == 4)
		return verifyRectangle(rect[0], rect[1], rect[2], rect[3]);
	else return false;
}