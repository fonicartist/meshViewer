// Class method implementations for the Triangle, Face, and Mesh class
// defined in mesh.h
//
// By Victor La

#include "mesh.h"

void Triangle::draw(vector<glm::vec3> verts) {
	// Draw line between vertex 0 and 1
	ofDrawLine(verts[index[0]], verts[index[1]]);
	// Draw line between vertex 1 and 2
	ofDrawLine(verts[index[1]], verts[index[2]]);
	// Draw line between vertex 2 and 0
	ofDrawLine(verts[index[2]], verts[index[0]]);
}

//--------------------------------------------------------------
Face::Face(int x, int y, int z, int w) {
	if (w == -1)
		size = 3;
	else
		size = 4;
	index[0] = x;
	index[1] = y;
	index[2] = z;
	index[3] = w;
}

//--------------------------------------------------------------
void Face::draw(vector<glm::vec3> verts) {
	int v0 = index[0],
		v1 = index[1],
		v2 = index[2],
		v3 = index[3];

	// Draw line between vertex 0 and 1
	ofDrawLine(verts[v0], verts[v1]);
	// Draw line between vertex 1 and 2
	ofDrawLine(verts[v1], verts[v2]);

	if (size == 3) {
		// Draw line between vertex 2 and 0
		ofDrawLine(verts[v2], verts[v0]);
	}
	else {
		// Draw line between vertex 2 and 3
		ofDrawLine(verts[v2], verts[v3]);
		// Draw line between vertex 3 and 0
		ofDrawLine(verts[v3], verts[v0]);
	}
}

//--------------------------------------------------------------
void Face::drawBetter(vector<glm::vec3> verts, bool **edges) {
	int v0 = index[0],
		v1 = index[1],
		v2 = index[2],
		v3 = index[3];

	// Draw line between vertex 0 and 1 if it hasn't been done yet
	if (!edges[v0][v1] && !edges[v1][v0]) {
		ofDrawLine(verts[v0], verts[v1]);
		edges[v0][v1] = true;
	}

	// Draw line between vertex 1 and 2 if it hasn't been done yet
	if (!edges[v1][v2] && !edges[v2][v1]) {
		ofDrawLine(verts[v1], verts[v2]);
		edges[v1][v2] = true;
	}

	// Draw last edge if triangular
	if (size == 3) {
		// Draw line between vertex 2 and 0 if it hasn't been done yet
		if (!edges[v2][v0] && !edges[v0][v2]) {
			ofDrawLine(verts[v2], verts[v0]);
			edges[v2][v0] = true;
		}
	}
	// Draw last two edges for quad
	else {
		// Draw line between vertex 2 and 3 if it hasn't been done yet
		if (!edges[v2][v3] && !edges[v3][v2]) {
			ofDrawLine(verts[v2], verts[v3]);
			edges[v2][v3] = true;
		}
		// Draw line between vertex 3 and 0 if it hasn't been done yet
		if (!edges[v3][v0] && !edges[v0][v3]) {
			ofDrawLine(verts[v3], verts[v0]);
			edges[v3][v0] = true;
		}
	}
}

//--------------------------------------------------------------
void Mesh::draw() {

	// Define an array to check for which edge has been drawn already.
	int size = vertices.size();
	bool **edges = new bool *[size];
	for (int i = 0; i < size; i++)
		edges[i] = new bool[size];

	// Loop through and set every edge case to false
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			edges[i][j] = false;


	// Draw faces and set edges to true as they're drawn
	for (int i = 0; i < faces.size(); i++)
		faces[i].drawBetter(vertices, edges);

	// Draw Vertices
	if (bDrawVerts) {
		//ofSetColor(ofColor::blue);
		for (glm::vec3 vert : vertices)
			ofDrawSphere(vert, .02);
	}

	// Cleanup
	for (int i = 0; i < size; i++)
		delete edges[i];
	delete edges;

};

//--------------------------------------------------------------
bool Mesh::intersects(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 &intersectPoint, glm::vec3 &intersectNormal) {
	vector<glm::vec3> vertsIntersected;
	glm::vec3 closestVert;

	// Cycle through all vertices and add them to a list of vertices along the same path
	for (glm::vec3 v : vertices) {
		glm::vec3 intPoint, intNorm;
		if (glm::intersectRaySphere(rayOrigin, rayDir, v, .05,
			intersectPoint, intersectNormal))
			vertsIntersected.push_back(v);
	}

	// Check which vertex is closest to rayOrigin
	if (vertsIntersected.size() != 0) {
		closestVert = vertsIntersected[0];
		for (glm::vec3 v : vertsIntersected) {
			if (glm::distance(rayOrigin, v) < glm::distance(rayOrigin, closestVert))
				closestVert = v;
		}
		intersectPoint = closestVert;
		return true;
	}
	else
		return false;
};