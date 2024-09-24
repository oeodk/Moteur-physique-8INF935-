#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	ofSetupOpenGL(WINDOW_BASE_WIDTH, WINDOW_BASE_HEIGHT, OF_WINDOW);		// <-------- setup the GL context
	ofRunApp(new ofApp());
}
