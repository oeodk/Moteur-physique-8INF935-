#define _USE_MATH_DEFINES
#include "ofApp.h"
#include <execution>
#include <cmath>

constexpr int SPEED_RANGE = 100;
constexpr float TRAJECTORY_POINT_NUMBER = 100;

//--------------------------------------------------------------
void ofApp::setup()
{
	time(NULL);
	_elapsed_time = 0;
	_selected_particle = 0;

	_gui_manager.setup();

	_terrain.sedRenderDistance(_render_engine.getFarPlane());
	_terrain.setup();
	_render_engine.setCameraPosition(Vector3D(0,1300, 0));
}

//--------------------------------------------------------------
void ofApp::update()
{
	_dt = ofGetLastFrameTime();
	_elapsed_time += _dt;

	_physics_engine.updateParticles(_dt, _particles);
	_terrain.update(_render_engine.getCameraPosition());

	_gui_manager.update(_dt, _selected_particle);

}

//--------------------------------------------------------------
void ofApp::draw()
{
	_render_engine.update(_dt);
	const auto& terrain_rendered_chunk = _terrain.getRenderedChunk();
	for (const auto& chunk : terrain_rendered_chunk)
	{
		_render_engine.addRenderTarget(chunk);
	}
	_render_engine.render();
	_gui_manager.draw();
}

void ofApp::exit()
{
	for (size_t i = 0; i < _particles.size(); i++)
	{
		delete _particles[i];
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	_gui_manager.onMouseScrolled(scrollY);
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

}
