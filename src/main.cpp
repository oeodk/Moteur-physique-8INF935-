#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	////Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
	//ofGLWindowSettings settings;
	//settings.setSize(1000, 1000);
	//settings.setGLVersion(4, 5);

	//settings.windowMode = OF_WINDOW; //can also be OF_FULLSCREEN

	//auto window = ofCreateWindow(settings);

	//ofRunApp(window, make_shared<ofApp>());
	//ofRunMainLoop();
	ofSetupOpenGL(1024, 1024, OF_WINDOW);// OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
