// Headers for the OF application, mesh object, and triangle class
//
// by Victor La
#pragma once
#include "ofMain.h"
#include "mesh.h"
#include <iostream>
#include <fstream>

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
		bool bSelected = false;

		// Camera properties
		enum CurrentCamera { easy, front, right, left };
		int _currentCamera;
		ofEasyCam cam;
		ofCamera *theCam;
		ofCamera frontCam, rightCam, leftCam;
		void changeCamera();

		// Mesh objects
		Mesh objMesh,
			 testMesh;

		glm::vec3 intersectPoint;
		
};
