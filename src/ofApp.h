// Headers for the OF application, mesh object, and triangle class
//
// by Victor La
#pragma once
#include "ofMain.h"
#include <iostream>
#include <fstream>

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

class Mesh {
public:
	vector<glm::vec3> vertices;
	vector<Triangle> triangles;
	void draw() {
		for (int i = 0; i < triangles.size(); i++)
			triangles[i].draw(vertices);
	};
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawAxis(glm::vec3);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		// Boolean flags
		bool bTestMeshVisible = true;
		bool bObjLoaded = false;

		// Camera properties
		enum CurrentCamera { easy, front };
		int _currentCamera;
		ofEasyCam cam;
		ofCamera *theCam;
		ofCamera frontCam;
		void changeCamera();

		// Mesh objects
		Mesh objMesh,
			 testMesh;
		
};
