#include "shapes.h"

bool NewShape::addTriangle(Point pa, Point pb, Point pc, Point refPoint) {
	Triangle t{ Vertex{pa}, Vertex{pb}, Vertex{pc} };
	return addTriangle(t, refPoint);
}
bool NewShape::addTriangle(Triangle& t, Point refPoint) {
	if (triangles_.size() >= maxTriangles_) return false;
	refPoint_ = refPoint;
	assignNormals(t, refPoint);
	triangles_.push_back(std::move(t));
	return true;
}
void NewShape::assignNormals(Triangle& t, Point& refPoint) {
	glm::vec3 v1 = t[1].pos - t[0].pos;
	glm::vec3 v2 = t[2].pos - t[0].pos;
	glm::vec3 normal = glm::normalize(glm::cross(v2, v1));
	
	//TODO Compare to reference point to determine if normal points away from ref point
	//If not, reverse normal


	for (Vertex& v : t) {
		v.normals = normal;
	}
}
bool NewShape::normalCorrect(Point p, Normal n, Point refPoint) {
	glm::vec3 pointToRef = refPoint - p;
	//a.b = ||a||*||b||cos(theta) where theta is the angle between a and b
	//If a.b is negative, the vectors point away from each other
	return glm::dot(pointToRef, n) <= 0;
}
Vertex& NewShape::vertex(const unsigned int index) {
	Vertex v;
	assert(index < 3 * triangles_.size());
	return triangles_[index % 3][index / 3];
}

// NewRectangle Members
// ====================
bool NewRectangle::extractTriangles(Point pa, Point pb, Point pc, Point pd,
	Triangle& ta, Triangle& tb, glm::vec3 refPoint) {
	//Determine the two conjoined rightangle triangles
	//Constraints
	// 1) all points must be in same plane
	// 2) points must specify a rectangle

	//



	//addTriangle(ta, refPoint);
	//addTriangle(tb, refPoint);
	return false;
}