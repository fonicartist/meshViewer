// Headers for the Triangle, Face, and Mesh classes
//
// By Victor La
#pragma once
#include "ofMain.h"

class Triangle {
	public:
		int index[3];
		Triangle(int x, int y, int z) {
			index[0] = x;
			index[1] = y;
			index[2] = z;
		};
		void draw(vector<glm::vec3> verts);
};

class Face {
	public:
		int size;
		int index[4];
		Face(int x, int y, int z, int w = -1);
		void draw(vector<glm::vec3> verts);
		void drawBetter(vector<glm::vec3> verts, bool **edges);
};

class Mesh {
	public:
		bool bDrawVerts = true;
		int selectedIndex;
		vector<glm::vec3> vertices;
		vector<Face> faces;
		void draw();
		bool intersects(glm::vec3 rayOrigin, glm::vec3 rayDir, 
						glm::vec3 &intersectPoint, glm::vec3 &intersectNormal);
};