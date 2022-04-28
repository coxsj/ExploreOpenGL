#include "shapes.h"

bool NewShape::addTriangle(Point pa, Point pb, Point pc, Point refPoint) {
	Triangle t{ Vertex{pa}, Vertex{pb}, Vertex{pc} };
	return addTriangle(t, refPoint);
}
bool NewShape::addTriangle(Triangle& t, Point refPoint) {
	if (triangles_.size() >= maxTriangles_ || Geometry::refPtNotInTrianglePlane(t, refPoint)) return false;
	refPoint_ = refPoint;
	Geometry::assignNormals(t, refPoint);
	triangles_.push_back(std::move(t));
	return true;
}

Vertex& NewShape::vertex(const unsigned int index) {
	Vertex v;
	assert(index < 3 * triangles_.size());
	return triangles_[index % 3][index / 3];
}

// NewTriangle Members
// ===================
NewTriangle::NewTriangle(Triangle& t, Point refPoint, const unsigned int newIndex) {
	initTriangle(t, refPoint, newIndex);
}
NewTriangle::NewTriangle(Point pa, Point pb, Point pc, Point refPoint,
	const unsigned int newIndex) {
	Vertex va{ pa };
	Vertex vb{ pb };
	Vertex vc{ pc };
	Triangle t{ va, vb, vc };
	initTriangle(t, refPoint, newIndex);
}
void NewTriangle::initTriangle(Triangle& t, Point refPoint, const unsigned int newIndex) {
	assert(addTriangle(t, refPoint));
	maxTriangles_ = 1;
	shaderIndex_ = newIndex;
}

// NewRectangle Members
// ====================
NewRectangle::NewRectangle(Point pa, Point pb, Point pc, Point pd, Point refPoint, 
	const unsigned int newIndex) {
	Triangle ta, tb;
	assert(Geometry::extractTrianglesFromRectangle(pa, pb, pc, pd, ta, tb, refPoint));
	initRectangle(ta, tb, refPoint, newIndex);
}

void NewRectangle::initRectangle(Triangle& ta, Triangle& tb, glm::vec3 refPoint, const unsigned int newIndex) {
	if (Geometry::verifyRectangle(ta, tb)) {
		addTriangle(ta, refPoint);
		addTriangle(tb, refPoint);
		assert(triangles() == 2);
		maxTriangles_ = 2;
		shaderIndex_ = newIndex;
	}
}


// Geometry Class Member Methods
//==============================
void Geometry::assignNormals(Triangle& t, Point& refPoint) {
	//This assumes the refPoint is not in the plane of the triangle
	//Normals will always be assigned to vertices in this routine
	glm::vec3 v1 = t[1].pos - t[0].pos;
	glm::vec3 v2 = t[2].pos - t[0].pos;
	glm::vec3 normal = glm::normalize(glm::cross(v2, v1));
	
	//Compare normal and a vertex to reference point to determine if normal points away from ref point
	//If not, reverse normal
	if (!normalCorrect(v1, normal, refPoint)) normal *= -1;

	for (Vertex& v : t) v.normals = normal;
}
bool Geometry::extractTrianglesFromRectangle(Point pa, Point pb, Point pc, Point pd,
	Triangle& ta, Triangle& tb, glm::vec3 refPoint) {
	//Determine the two conjoined rightangle triangles

	//Ref point cannot be same as vertex point
	if (pa == refPoint || pb == refPoint || pc == refPoint || pd == refPoint) return false;

	// Points must specify a rectangle
	if (!Geometry::verifyRectangle(pa, pb, pc, pd)) return false;

	// TODO Specify first triangle then determine points of second triangle

	assert(0);


	//ta[0] = ; ta[1]= ; ta[2]= ;
	//tb[0] = ; tb[1]= ; tb[2]= ;
	return false;
}
bool Geometry::normalCorrect(Point p, Normal n, Point refPoint) {
	glm::vec3 pointToRef = refPoint - p;
	//a.b = ||a||*||b||cos(theta) where theta is the angle between a and b
	//If a.b is negative, the vectors point away from each other
	return glm::dot(pointToRef, n) <= 0;
}
bool Geometry::refPtNotInTrianglePlane(Triangle& t, Point refPoint) {
	//Determine if refPoint is on plane of triangle
	assert(0);
	return false;
}
bool Geometry::verifyRectangle(Point a, Point b, Point c, Point d) {
	// points all in same plane
	// 4 rightangles formed by sides
	assert(0);
	return false;
}
bool Geometry::verifyRectangle(Triangle& ta, Triangle& tb) {
	// points all in same plane
	// 4 rightangles formed by sides
	assert(0);
	return false;
}