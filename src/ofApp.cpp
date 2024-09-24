#define _USE_MATH_DEFINES
#include "ofApp.h"
#include "BulletParticle.h"
#include "CannonballParticle.h"
#include "FireballParticle.h"
#include "BubbleParticle.h"
#include <execution>
#include <cmath>

constexpr int SPEED_RANGE = 100;
constexpr float TRAJECTORY_POINT_NUMBER = 100;

//--------------------------------------------------------------
void ofApp::setup() {
	time(NULL);
	_elapsed_time = 0;
	_selected_particle = 0;

	_gui_manager.setup(_particle_types);

	_terrain.sedRenderDistance(_render_engine.getFarPlane());
	_terrain.setup();
	_render_engine.setCameraPosition(Vector3D(0, 1300, 0));

	Vector3D::testVector3D();
	Particle::testParticle();
}

//--------------------------------------------------------------
void ofApp::update() {
	_dt = ofGetLastFrameTime();
	_elapsed_time += _dt;

	_physics_engine.updateParticles(_dt, _particles);
	_terrain.update(_render_engine.getCameraPosition());

	for (size_t i = 0; i < _particles.size(); i++) {
		const Vector3D* particle_pos = _particles[i]->getPosition();
		if (particle_pos->y > 5000 || particle_pos->y < _terrain.getHeight(particle_pos->x, particle_pos->z)) {
			_particles.erase(_particles.begin() + i);
			i--;
		}
	}

	_gui_manager.update(_dt, _selected_particle);
}

//--------------------------------------------------------------
void ofApp::draw() {
	_render_engine.update(_dt);
	const auto& terrain_rendered_chunk = _terrain.getRenderedChunk();
	for (const auto& chunk : terrain_rendered_chunk) {
		_render_engine.addRenderTarget(chunk);
	}
	for (const auto& particle : _particles) {
		_render_engine.addRenderTarget(particle);
		_render_engine.addRenderTarget(particle, false);
	}

	_render_engine.render();
	_gui_manager.draw();
}

void ofApp::exit() {
	for (size_t i = 0; i < _particles.size(); i++) {
		delete _particles[i];
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ofKey::OF_KEY_RETURN) {
		_physics_engine.changeIntegrationMethod();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	switch (button) {
	case OF_MOUSE_BUTTON_LEFT:
		spawnParticle(_particle_types[_selected_particle]);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY) {
	if (scrollY > 0)
		_selected_particle = _selected_particle == 0 ? _particle_types.size() - 1 : _selected_particle - 1;
	else if (scrollY < 0)
		_selected_particle = _selected_particle == _particle_types.size() - 1 ? 0 : _selected_particle + 1;

	_render_engine.updateSelectedParticle(_particle_types[_selected_particle]);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	_render_engine.windowResized(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::spawnParticle(BulletType type) {
	const ofCamera& camera = _render_engine.getCamera();
	Vector3D current_position = camera.getPosition();
	const Vector3D& look_at_dir = camera.getLookAtDir();
	const Vector3D& side_dir = camera.getSideDir();
	const Vector3D up_dir = Vector3D::crossProduct(side_dir, look_at_dir);
	Particle* newParticle;

	Vector3D cannon_offset;

	static int dep = 0;
	dep++;
	current_position += (side_dir * 6 + look_at_dir * 24 + up_dir * (-4));
	switch (type) {
	case BULLET:
		newParticle = new BulletParticle(current_position, look_at_dir, G_ACC);
		_particles.push_back(newParticle);
		break;
	case CANNONBALL:
		newParticle = new CannonballParticle(current_position, look_at_dir, G_ACC);
		_particles.push_back(newParticle);
		break;
	case FIREBALL:
		newParticle = new FireballParticle(current_position, look_at_dir, G_ACC);
		_particles.push_back(newParticle);
		break;
	case BUBBLE:
		newParticle = new BubbleParticle(current_position, look_at_dir, G_ACC);
		_particles.push_back(newParticle);
		break;
	default:
		break;
	}

}
