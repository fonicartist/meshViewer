// CS 116A Project 1 - Meshes
//
// by Victor La
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	// Setup cameras
	cam.setDistance(10);
	cam.setPosition(glm::vec3(0, 0, 20));
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	cam.disableMouseInput();	// default camera is the wide shot
	theCam = &cam;
	_currentCamera = 0;
		// Camera 2, a still camera looking from the front
	frontCam.setNearClip(.1);
	frontCam.setFov(70);
	frontCam.setPosition(glm::vec3(0, 5, 15));
	frontCam.lookAt(glm::vec3(0, 5, 0));
		// Camera 3, looks at the origin from the right side
	rightCam.setNearClip(.1);
	rightCam.setFov(70);
	rightCam.setPosition(glm::vec3(15, 10, 0));
	rightCam.lookAt(glm::vec3(0, 5, 0));
		// Camera 4, looks at the origin from the left side
	leftCam.setNearClip(.1);
	leftCam.setFov(70);
	leftCam.setPosition(glm::vec3(-15, 10, 0));
	leftCam.lookAt(glm::vec3(0, 5, 0));

	// Set up test geometry, an inverted pyramid with a triangular base
	testMesh.vertices.push_back(glm::vec3(-6, 6, 6));
	testMesh.vertices.push_back(glm::vec3(6, 6, 6));
	testMesh.vertices.push_back(glm::vec3(0, 6, -6));
	testMesh.vertices.push_back(glm::vec3(0, -6, 0));
	testMesh.faces.push_back(Face(0, 1, 2));
	testMesh.faces.push_back(Face(1, 2, 3));
	testMesh.faces.push_back(Face(2, 0, 3));
	testMesh.faces.push_back(Face(0, 1, 3));
	
}

//--------------------------------------------------------------
void ofApp::update(){

	switch (_currentCamera) {
	case easy: theCam = &cam;
		break;
	case front: theCam = &frontCam;
		break;
	case right: theCam = &rightCam;
		break;
	case left: theCam = &leftCam;
		break;
	default:
		break;
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	// Draw a black background
	ofBackground(ofColor::black);

	// Begin drawing in the camera
	theCam->begin();
	ofPushMatrix();

		// Draw world axis
		drawAxis(glm::vec3(0, 0, 0));

		// Set drawing color to white
		ofSetColor(ofColor::white);

		// Draw test Mesh if enabled and another object hasn't been loaded yet
		if (bTestMeshVisible && !bObjLoaded) {
			// testMesh.draw();
			// Draw sphere
			ofNoFill();
			ofDrawSphere(glm::vec3(.5, .5, .5), 1);
		}
		else if (bObjLoaded)
			objMesh.draw();

		// Draw selected vertex
		if (bSelected) {
			ofSetColor(ofColor::red);
			ofFill();
			ofDrawSphere(intersectPoint, .04);
		}

	// End drawing in the camera
	ofPopMatrix();
	theCam->end();


	// Framerate
	string str;
	str += "Frame Rate: " + std::to_string(ofGetFrameRate());
	ofSetColor(ofColor::white);
	ofDrawBitmapString(str, ofGetWindowWidth() - 205, 15);

}

// Draw an XYZ axis in RGB at world location for reference.
//
void ofApp::drawAxis(glm::vec3 location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));


	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	switch (key) {
	case 'B':
	case 'b':
		bTestMeshVisible = !bTestMeshVisible;
		break;
	case 'C':
	case 'c':
		if (cam.getMouseInputEnabled()) cam.disableMouseInput();
		else cam.enableMouseInput();
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'T':
	case 't':
		changeCamera();
	default:
		break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	bool bInteresects = false;

	glm::vec3 screen3dPoint = theCam->screenToWorld(glm::vec3(x, y, 0));
	glm::vec3 rayOrigin = theCam->getPosition();
	glm::vec3 rayDir = glm::normalize(screen3dPoint - rayOrigin);
	glm::vec3 intersectPos, intersectNormal;

	// Check intersection with sphere if OBJ file is not loaded
	if (!bObjLoaded)
		bInteresects = glm::intersectRaySphere(rayOrigin, rayDir, glm::vec3(.5, .5, .5), 1,
											   intersectPoint, intersectNormal);
	// Check intersection if loaded obj file
	else
		bInteresects = objMesh.intersects(rayOrigin, rayDir, intersectPoint, intersectNormal);

	if (bInteresects) {
		cout << "Hit!" << endl;
		bSelected = true;
	}
	else {
		cout << "Missed!" << endl;
		bSelected = false;
	}

	cout << "3D screen position: " << screen3dPoint << endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

	objMesh = Mesh();

	bool isQuad = false;
	string line, val, xstr, ystr, zstr, wstr;
	int verts = 0,
		faces = 0;

	// Load obj into an ifstream variable
	ifstream myfile(dragInfo.files.at(0));
	ifstream file(dragInfo.files.at(0), ios::binary | ios::ate);
	
	// Loop through and read file
	while (getline(myfile, line)) {

		stringstream ss(line);
		ss >> val >> xstr >> ystr >> zstr >> wstr;
			
		// Read in vertices
		if (val.substr(0, val.find(' ')) == "v") {
			float x = stof(xstr),
				  y = stof(ystr),
				  z = stof(zstr);
			//cout << "Pushing vertex (" << x << ", " << y << ", " << z << ")\n";
			verts += 3;
			objMesh.vertices.push_back(glm::vec3(x, y, z));
		}
		// Read in faces
		else if (val[0] == 'f') {
			int x = stoi(xstr.substr(0, xstr.find('/'))) - 1,
				y = stoi(ystr.substr(0, ystr.find('/'))) - 1,
				z = stoi(zstr.substr(0, zstr.find('/'))) - 1,
				w;
			if (!wstr.empty()) {
				w = stoi(wstr.substr(0, wstr.find('/'))) - 1;
				objMesh.faces.push_back(Face(x, y, z, w));
			}
			else
				objMesh.faces.push_back(Face(x, y, z));
			//cout << "Pushing Triangle (" << x << ", " << y << ", " << z << ")\n";
			faces++;
		}
		
	}

	bObjLoaded = true;

	cout << "Number of vertices: " << verts << endl;
	cout << "Number of faces: " << faces << endl;
	cout << "Size of model loaded: " << sizeof(objMesh) / 1000.0 << "kb" << endl;

}

// Swap between the two cameras based on enumeration
//
void ofApp::changeCamera() {
	_currentCamera++;
	if (_currentCamera == 4)
		_currentCamera = 0;
}
