#define _USE_MATH_DEFINES
#include "ofApp.h"
#include "particles/BulletParticle.h"
#include "particles/CannonballParticle.h"
#include "particles/FireballParticle.h"
#include "particles/BubbleParticle.h"
#include "forces/GravityParticleForce.h"
#include "forces/FrictionForceGenerator.h"
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
	_render_engine.setCameraPosition(Vector3D(0, 100 + _terrain.getHeight(0, 0), 0));

	Vector3D::testVector3D();
	Particle::testParticle();
}

//--------------------------------------------------------------
void ofApp::update() {
	_dt = ofGetLastFrameTime();
	_elapsed_time += _dt;

	GravityParticleForce gravity_force(Vector3D(0, -9.8f, 0));
	FrictionForceGenerator friction_force;

	// Apply forces to the particles
	for (Particle *particle : _particles) {
		_forces_registry.add(particle, &gravity_force);
		_forces_registry.add(particle, &friction_force);
	}

	if(_the_blob)
	{
		_the_blob->updateBlob();
	}

	_forces_registry.updateForces(_dt);
	_physics_engine.updateParticles(_dt, _particles, &_terrain);
	_terrain.update(_render_engine.getCameraPosition());
	_forces_registry.clear();

	//Delete a particle if it is too high or too low
	for (size_t i = 0; i < _particles.size(); i++) {
		Vector3D particle_dist(_particles[i]->getParticlePosition() - _render_engine.getCameraPosition());
		if (particle_dist.squareNorm() > std::pow(_render_engine.getFarPlane(), 2)) {
			if (_particles[i] == _the_blob)
			{
				_the_blob = nullptr;
			}
			delete _particles[i];
			_particles.erase(_particles.begin() + i);
			i--;
		}
	}
	_render_engine.update(_dt);

	_gui_manager.update(_dt, _selected_particle);
}

//--------------------------------------------------------------
void ofApp::draw() {
	const auto& terrain_rendered_chunk = _terrain.getRenderedChunk();
	
	// Add terrain and particles to the render
	for (const auto& chunk : terrain_rendered_chunk) {
		_render_engine.addRenderTarget(chunk);
		_render_engine.addRenderTarget(chunk, false);
	}
	for (const auto& particle : _particles) {
		_render_engine.addRenderTarget(particle);
		_render_engine.addRenderTarget(particle, false);
	}

	_render_engine.render();
	_gui_manager.draw();

	auto player_position = _render_engine.getCameraPosition();
	auto& p = _render_engine.point.getVertices()[0];
	//std::cout << _terrain.getHeight(player_position.x, player_position.z) << std::endl;
	p.x = player_position.x;//(Vector3D(player_position.x, 10 + _terrain.getHeight(player_position.x, player_position.z), player_position.z));
	p.y = 10 + _terrain.getHeight(player_position.x, player_position.z);//(Vector3D(player_position.x, 10 + _terrain.getHeight(player_position.x, player_position.z), player_position.z));
	p.z = player_position.z;//(Vector3D(player_position.x, 10 + _terrain.getHeight(player_position.x, player_position.z), player_position.z));

}

void ofApp::exit() {
	for (size_t i = 0; i < _particles.size(); i++) {
		delete _particles[i];
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ofKey::OF_KEY_RIGHT_SHIFT) {
		Particle::_draw_trail = !Particle::_draw_trail;
	}
	if (key == ofKey::OF_KEY_RIGHT_CONTROL)
	{
		if (_the_blob != nullptr)
		{
			_the_blob->split();
		}
	}
	if (key == '0')
	{
		spawnParticle(BulletType::BLOB);
	}
	if (key == ofKey::OF_KEY_RETURN)
	{
		_the_blob = nullptr;
		for (size_t i = 0; i < _particles.size(); i++)
		{
			delete _particles[i];
		}
		_particles.clear();
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
		newParticle = new BulletParticle(current_position, look_at_dir);
		_particles.push_back(newParticle);
		break;
	case CANNONBALL:
		newParticle = new CannonballParticle(current_position, look_at_dir);
		_particles.push_back(newParticle);
		break;
	case FIREBALL:
		newParticle = new FireballParticle(current_position, look_at_dir);
		_particles.push_back(newParticle);
		break;
	case BUBBLE:
		newParticle = new BubbleParticle(current_position, look_at_dir);
		_particles.push_back(newParticle);
		break;
	case BLOB:
		newParticle = new Blob(&_particles, &_forces_registry, current_position, look_at_dir);
		_the_blob = dynamic_cast<Blob*>(newParticle);
		_particles.push_back(newParticle);
		break;
	default:
		//for (int i = 0; i < 250; i++)
		//{
		//	switch (_particle_types[_selected_particle])
		//	{
		//	case BULLET:
		//		newParticle = new BulletParticle(current_position, look_at_dir + Vector3D((rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 200.f));
		//		break;
		//	case CANNONBALL:
		//		newParticle = new CannonballParticle(current_position, look_at_dir + Vector3D((rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 25.f, (rand() % 100 - 50) / 25.f));
		//		break;
		//	case FIREBALL:
		//		newParticle = new FireballParticle(current_position, look_at_dir + Vector3D((rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 100.f, (rand() % 100 - 50) / 100.f));
		//		break;
		//	case BUBBLE:
		//		newParticle = new BubbleParticle(current_position, look_at_dir + Vector3D((rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 50.f, (rand() % 100 - 50) / 50.f));
		//		break;
		//	default:
		//		break;
		//	}
		//	_particles.push_back(newParticle);
		//}
		break;
	}

}
