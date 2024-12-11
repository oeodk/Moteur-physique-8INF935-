#define _USE_MATH_DEFINES
#include "ofApp.h"
#include "test/TestQuaternion.h"
#include "particles/BulletParticle.h"
#include "particles/CannonballParticle.h"
#include "particles/FireballParticle.h"
#include "particles/BubbleParticle.h"
#include "particles/rigid_bodies/Chicken.h"
#include "particles/rigid_bodies/Goat.h"
#include "particles/rigid_bodies/Sephiroth.h"
#include "particles/rigid_bodies/Masamune.h"
#include "particles/rigid_bodies/MovingTestCube.h"
#include "particles/rigid_bodies/ImmovableBlock.h"
#include "particles/rigid_bodies/StaticTestCube.h"
#include "particles/Anchor.h"
#include "forces/GravityParticleForce.h"
#include "forces/FrictionForceGenerator.h"
#include "constrain/BarConstrain.h"
#include "constrain/CableConstrain.h"
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
	TestQuaternion::launchTests();
	Particle::testParticle();
	Matrix3::testMatrix3();

	_blob_key.insert({ ofKey::OF_KEY_RETURN, false });
	_blob_key.insert({ ofKey::OF_KEY_UP    , false });
	_blob_key.insert({ ofKey::OF_KEY_DOWN  , false });
	_blob_key.insert({ ofKey::OF_KEY_RIGHT , false });
	_blob_key.insert({ ofKey::OF_KEY_LEFT  , false });

	Sephiroth::loadMesh();
}

//--------------------------------------------------------------
void ofApp::update() {
	_dt = ofGetLastFrameTime();
	_elapsed_time += _dt;

	GravityParticleForce gravity_force(Vector3D(0, -9.8f, 0));
	FrictionForceGenerator friction_force;

#ifndef RIGID_BODY_ONLY
	// Apply forces to the particles
	for (Particle *particle : _particles) {
		_forces_registry.add(particle, &gravity_force);
		_forces_registry.add(particle, &friction_force);
	}
	for (Blob* blob: _blobs)
	{
		blob->updateBlob();
	}
	moveBlobs();


	_forces_registry.updateForces(_dt);
	_physics_engine.updateParticles(_dt, _particles, &_terrain);
#else
	if(_do_simulation)
	{
		for (RigidBody* rigid_body : _rigid_body)
		{
			if (!dynamic_cast<StaticTestCube*>(rigid_body) && !dynamic_cast<MovingTestCube*>(rigid_body) && !dynamic_cast<ImmovableBlock*>(rigid_body))
			{
				_forces_registry.add(rigid_body, &gravity_force);
				_forces_registry.add(rigid_body, &friction_force);
			}
		}

		_particles_octree = ocTree(_render_engine.getCameraPosition(), Vector3D(5000));
		_particles_octree.build(_rigid_body);

		_forces_registry.updateForces(_dt);
		_physics_engine.updateRigidBody(_dt, _rigid_body, &_terrain, _particles_octree);
#endif

		_terrain.update(_render_engine.getCameraPosition(), _dt);
		_forces_registry.clear();
#ifndef RIGID_BODY_ONLY

		//Delete a particle if it is too high or too low
		for (size_t i = 0; i < _particles.size(); i++)
		{
			Vector3D particle_dist(_particles[i]->getParticlePosition() - _render_engine.getCameraPosition());
			if (particle_dist.squareNorm() > std::pow(_render_engine.getFarPlane(), 2))
			{
				if (Blob* blob = dynamic_cast<Blob*>(_particles[i]))
				{
					auto& blob_pos = std::find(_blobs.begin(), _blobs.end(), blob);
					if (blob_pos != _blobs.end())
					{
						_blobs.erase(blob_pos);
					}
				}
				delete _particles[i];
				_particles.erase(_particles.begin() + i);
				i--;
	}
}
#else
		//Delete a particle if it is too high or too low
		for (size_t i = 0; i < _rigid_body.size(); i++)
		{
			Vector3D particle_dist(_rigid_body[i]->getParticlePosition() - _render_engine.getCameraPosition());
			if (particle_dist.squareNorm() > std::pow(_render_engine.getFarPlane(), 2))
			{
				delete _rigid_body[i];
				_rigid_body.erase(_rigid_body.begin() + i);
				i--;
			}
		}

#endif

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
#ifndef RIGID_BODY_ONLY
	for (const auto& particle : _particles) {
		_render_engine.addRenderTarget(particle);
		_render_engine.addRenderTarget(particle, false);
	}
#else
	for (const auto& rigid_body : _rigid_body)
	{
		_render_engine.addRenderTarget(rigid_body);
		_render_engine.addRenderTarget(rigid_body, false);
	}
#endif
	if(_show_octree)
	{
		_render_engine.addRenderTarget(&_particles_octree, false);
	}

	_render_engine.render();
	_gui_manager.draw();

#ifndef RIGID_BODY_ONLY
	_gui_manager.setBlobParticleCount(
		_blobs.size() > 0 ? _blobs[0]->getParticleCount() : -1
	);
#endif
}

void ofApp::exit() {
	for (size_t i = 0; i < _particles.size(); i++) {
		delete _particles[i];
	}

	for (size_t i = 0; i < _rigid_body.size(); i++)
	{
		delete _rigid_body[i];
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ofKey::OF_KEY_RIGHT_SHIFT) {
		for (Blob* blob : _blobs)
		{
			blob->resetMovement();
		}
	}
	if (key == ofKey::OF_KEY_RIGHT_CONTROL)
	{
		for(Blob* blob : _blobs)
		{
			blob->split();
		}
	}
	if (key == ofKey::OF_KEY_F1)
	{
		_do_simulation = !_do_simulation;
		if (_do_simulation)
		{
			_za_warudo.load("sound/time_resume.mp3");
			_za_warudo.play();
		}
		else
		{
			_za_warudo.load("sound/time_stop.mp3");
			_za_warudo.play();
		}
	}
	if (key == ofKey::OF_KEY_F2)
	{
		_show_octree = !_show_octree;
	}
	if (key == ofKey::OF_KEY_F3)
	{
		_show_hitbox = !_show_hitbox;
	}
	if (key == ofKey::OF_KEY_ALT)
	{
		_blobs.clear();
		for (size_t i = 0; i < _particles.size(); i++)
		{
			delete _particles[i];
		}
		_particles.clear();

		for (size_t i = 0; i < _rigid_body.size(); i++)
		{
			delete _rigid_body[i];
		}
		_rigid_body.clear();
	}
#ifndef RIGID_BODY_ONLY
	if (key == '1')
	{
		Anchor* anchor = new Anchor(_render_engine.getCameraPosition(), Vector3D(150, 150, 150));
		Particle* particle = new FireballParticle(_render_engine.getCameraPosition()- Vector3D(0,30,0), Vector3D());
		_particles.push_back(anchor);
		_particles.push_back(particle);

		particle->addConstrain(std::make_shared<CableConstrain>(100, anchor));
	}
	if (key == '2')
	{
		Anchor* anchor = new Anchor(_render_engine.getCameraPosition(), Vector3D(0, 0, 0));
		Particle* particle = new FireballParticle(_render_engine.getCameraPosition() - Vector3D(0, 30, 0), Vector3D());
		_particles.push_back(anchor);
		_particles.push_back(particle);

		particle->addConstrain(std::make_shared<BarConstrain>(100, anchor));
	}
#endif
	if (key == 'p') {
#ifndef RIGID_BODY_ONLY
		spawnParticle(E_END);
#else
		spawnRigidBody(E_END);
#endif
	}
#ifndef RIGID_BODY_ONLY
	for(auto& input : _blob_key)
	{
		if (key == input.first)
		{
			_blob_key.at(input.first) = true;
		}
	}
#endif

	if (key == ofKey::OF_KEY_TAB)
	{
		Particle::_draw_trail = !Particle::_draw_trail;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
#ifndef RIGID_BODY_ONLY
	for (auto& input : _blob_key)
	{
		if (key == input.first)
		{
			_blob_key.at(input.first) = false;
		}
	}
#endif

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
#ifndef RIGID_BODY_ONLY
		spawnParticle(_particle_types[_selected_particle]);
#else
		spawnRigidBody(_particle_types[_selected_particle]);
#endif
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
	switch (type)
	{
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
		_blobs.push_back(dynamic_cast<Blob*>(newParticle));
		_particles.push_back(newParticle);
		break;
	case CHICKEN:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
		Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

		newParticle = new Chicken(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), base_force * 10000, up_dir * 10);

		_particles.push_back(newParticle);
	}
	break;
	case MASAMUNE:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new Masamune(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 1100000, up_dir * 100);

		_particles.push_back(newParticle);
	}
	break;
	case SEPHIROTH:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new Sephiroth(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), up_dir * 300000, side_dir * 120);

		_particles.push_back(newParticle);
	}
	break;
	case GOAT:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new Goat(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

		_particles.push_back(newParticle);
	}
	break;
	case STATIC_CUBE:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new StaticTestCube(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

		_particles.push_back(newParticle);
	}
	break;
	case IMMOBILE_BLOCK:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new ImmovableBlock(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

		_particles.push_back(newParticle);
	}
	break;
	case MOVING_CUBE:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new MovingTestCube(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

		_particles.push_back(newParticle);
	}
	break;
		default:
			for (int i = 0; i < 50; i++)
			{
				Vector3D current_position2 = current_position + side_dir * (rand() % 1000 - 500) + Vector3D(0, rand() % 500, 0);
				switch (_particle_types[_selected_particle])
				{
				case BULLET:
					newParticle = new BulletParticle(current_position2, look_at_dir + Vector3D((rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 200.f));
					break;
				case CANNONBALL:
					newParticle = new CannonballParticle(current_position2, look_at_dir + Vector3D((rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 25.f, (rand() % 100 - 50) / 25.f));
					break;
				case FIREBALL:
					newParticle = new FireballParticle(current_position2, look_at_dir + Vector3D((rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 100.f, (rand() % 100 - 50) / 100.f));
					break;
				case BUBBLE:
					newParticle = new BubbleParticle(current_position2, look_at_dir + Vector3D((rand() % 100 - 50) / 200.f, (rand() % 100 - 50) / 50.f, (rand() % 100 - 50) / 50.f));
					break;
				case BLOB:
					newParticle = new Blob(&_particles, &_forces_registry, current_position, look_at_dir);
					_blobs.push_back(dynamic_cast<Blob*>(newParticle));
					break;
				case CHICKEN:
				{
					Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
					Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

					newParticle = new Chicken(current_position2, look_at_dir,
						base_orientation,
						Vector3D(0, 0, 0), base_force * 10000, up_dir * 10);
				}
				break;
				case MASAMUNE:
				{
					Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

					newParticle = new Masamune(current_position2, look_at_dir,
						base_orientation,
						Vector3D(0, 0, 0), look_at_dir * 900000, up_dir * 100);
				}
				break;
				case SEPHIROTH:
				{
					Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

					newParticle = new Sephiroth(current_position2, look_at_dir,
						base_orientation,
						Vector3D(0, 0, 0), up_dir * 500000, side_dir * 120);
				}
				break;
				case GOAT:
				{
					Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

					newParticle = new Goat(current_position2, look_at_dir,
						base_orientation,
						Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

				}
				break;
				case STATIC_CUBE:
				{
					Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

					newParticle = new StaticTestCube(current_position, look_at_dir,
						base_orientation,
						Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

				}
				break;
				case MOVING_CUBE:
				{
					Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

					newParticle = new MovingTestCube(current_position, look_at_dir,
						base_orientation,
						Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

				}
				break;
				case IMMOBILE_BLOCK:
				{
					Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

					newParticle = new ImmovableBlock(current_position, look_at_dir,
						base_orientation,
						Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

				}
				break;
				default:
					break;
				}
				_particles.push_back(newParticle);
			}
			break;
		
		
	}
}

void ofApp::spawnRigidBody(BulletType type)
{
	const ofCamera& camera = _render_engine.getCamera();
	Vector3D current_position = camera.getPosition();
	const Vector3D& look_at_dir = camera.getLookAtDir();
	const Vector3D& side_dir = camera.getSideDir();
	const Vector3D up_dir = Vector3D::crossProduct(side_dir, look_at_dir);
	RigidBody* newParticle;

	Vector3D cannon_offset;

	static int dep = 0;
	dep++;
	current_position += (side_dir * 6 + look_at_dir * 24 + up_dir * (-4));
	switch (type)
	{
	case CHICKEN:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
		Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

		newParticle = new Chicken(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), base_force * 10000, up_dir * 10);

		_rigid_body.push_back(newParticle);
	}
	break;
	case MASAMUNE:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
		Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

		newParticle = new Masamune(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 250000, up_dir * 100);

		_rigid_body.push_back(newParticle);
	}
	break;
	case SEPHIROTH:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
		Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

		newParticle = new Sephiroth(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), up_dir * 300000, side_dir * 120);

		_rigid_body.push_back(newParticle);
	}
	break;
	case GOAT:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
		Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

		newParticle = new Goat(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

		_rigid_body.push_back(newParticle);
	}
	break;
	case STATIC_CUBE:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new StaticTestCube(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

		_rigid_body.push_back(newParticle);
	}
	break;
	case MOVING_CUBE:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new MovingTestCube(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

		_rigid_body.push_back(newParticle);
	}
	break;
	case IMMOBILE_BLOCK:
	{
		Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

		newParticle = new ImmovableBlock(current_position, look_at_dir,
			base_orientation,
			Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

		_rigid_body.push_back(newParticle);
	}
	break;
	default:
		for (int i = 0; i < 50; i++)
		{
			Vector3D current_position2 = current_position + side_dir * (rand() % 1000 - 500) + Vector3D(0, rand() % 500, 0);
			switch (_particle_types[_selected_particle])
			{
			case CHICKEN:
			{
				Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
				Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

				newParticle = new Chicken(current_position2, look_at_dir,
					base_orientation,
					Vector3D(0, 0, 0), base_force * 10000, up_dir * 10);
			}
			break;
			case MASAMUNE:
			{
				Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
				Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

				newParticle = new Masamune(current_position2, look_at_dir,
					base_orientation,
					Vector3D(0, 0, 0), look_at_dir * 250000, up_dir * 100);
			}
			break;
			case SEPHIROTH:
			{
				Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
				Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

				newParticle = new Sephiroth(current_position2, look_at_dir,
					base_orientation,
					Vector3D(0, 0, 0), up_dir * 500000, side_dir * 120);
			}
			break;
			case GOAT:
			{
				Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));
				Vector3D base_force(rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1, rand() % 1000 / 500.f - 1);

				newParticle = new Goat(current_position2, look_at_dir,
					base_orientation,
					Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

			}
			break;
			case STATIC_CUBE:
			{
				Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

				newParticle = new StaticTestCube(current_position2, look_at_dir,
					base_orientation,
					Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

			}
			break;
			case MOVING_CUBE:
			{
				Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

				newParticle = new MovingTestCube(current_position2, look_at_dir,
					base_orientation,
					Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

			}
			break;
			case IMMOBILE_BLOCK:
			{
				Quaternion base_orientation = Quaternion::fromEulerAngle(Vector3D(camera.getPitchRad(), camera.getHeadingRad(), camera.getRollRad()));

				newParticle = new ImmovableBlock(current_position2, look_at_dir,
					base_orientation,
					Vector3D(0, 0, 0), look_at_dir * 100000, side_dir * 15);

			}
			break;
			default:
				break;
			}
			_rigid_body.push_back(newParticle);
		}
		break;


	}
}

void ofApp::moveBlobs()
{
	if (_blobs.size() > 0)
	{
		Vector3D blob_forces;
		const float BLOB_MASS = _blobs[0]->getMass();
		Vector3D look_at_dir = _render_engine.getCamera().getLookAtDir();
		look_at_dir.y = 0;
		look_at_dir.normalize();
		const Vector3D side_dir = _render_engine.getCamera().getSideDir();
		if (_blob_key.at(ofKey::OF_KEY_RETURN))
		{
			blob_forces.y += (-2) * G_ACC.y;
		}
		if (_blob_key.at(ofKey::OF_KEY_UP))
		{
			blob_forces -= look_at_dir * G_ACC.y;
		}
		if (_blob_key.at(ofKey::OF_KEY_DOWN))
		{
			blob_forces += look_at_dir * G_ACC.y;
		}
		if (_blob_key.at(ofKey::OF_KEY_RIGHT))
		{
			blob_forces -= side_dir * G_ACC.y;
		}
		if (_blob_key.at(ofKey::OF_KEY_LEFT))
		{
			blob_forces += side_dir * G_ACC.y;
		}

		for (Blob* blob : _blobs)
		{
			blob->addForce(blob_forces * BLOB_MASS * 5);
		}
	}
}

