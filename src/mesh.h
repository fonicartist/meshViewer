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
		void draw(vector<glm::vec3> verts) {
			// Draw line between vertex 0 and 1
			ofDrawLine(verts[index[0]], verts[index[1]]);
			// Draw line between vertex 1 and 2
			ofDrawLine(verts[index[1]], verts[index[2]]);
			// Draw line between vertex 2 and 0
			ofDrawLine(verts[index[2]], verts[index[0]]);
		};
};

class Face {
	public:
		int size;
		int index[4];
		Face(int x, int y, int z) {
			Face(x, y, z, -1);
		};
		Face(int x, int y, int z, int w) {
			if (w == -1) 
				size = 3;
			else 
				size = 4;
			index[0] = x;
			index[1] = y;
			index[2] = z;
			index[3] = w;
		};
		void draw(vector<glm::vec3> verts) {
			// Draw line between vertex 0 and 1
			ofDrawLine(verts[index[0]], verts[index[1]]);
			// Draw line between vertex 1 and 2
			ofDrawLine(verts[index[1]], verts[index[2]]);
			
			if (size == 3) {
				// Draw line between vertex 2 and 0
				ofDrawLine(verts[index[2]], verts[index[0]]);
			}
			else {
				// Draw line between vertex 2 and 3
				ofDrawLine(verts[index[2]], verts[index[3]]);
				// Draw line between vertex 3 and 0
				ofDrawLine(verts[index[3]], verts[index[0]]);
			}
		};
};

class Mesh {
	public:
		bool bDrawVerts = true;
		vector<glm::vec3> vertices;
		vector<Face> faces;
		void draw() {

			// Draw faces
			for (int i = 0; i < faces.size() / 2; i++) {
				faces[i].draw(vertices);
				if (faces.size() - i - 1 != i)
					faces[faces.size() - i - 1].draw(vertices);
			}
			// Draw Vertices
			if (bDrawVerts) {
				//ofSetColor(ofColor::blue);
				for (glm::vec3 vert : vertices)
					ofDrawSphere(vert, .05);
			}
		};
};