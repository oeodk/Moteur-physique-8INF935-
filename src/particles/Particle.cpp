#include "Particle.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>
#include <stdexcept>

Particle::Particle() {
	_world_position = &_position;
	_trail.setMode(OF_PRIMITIVE_POINTS);
	_trail.addVertex(_position);
	_trail.addVertex(_position);
}

Particle::Particle(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius, const Vector3D& color, float alpha)
	:  _color(color), _alpha(alpha), _time_counter(0) {
	_position = init_pos;
	_velocity = init_vel;
	_acceleration = 0; 
	_accum_force = 0; 
	_radius = radius;
	_mass = mass;
	_inverse_mass = mass == 0 ? FLT_MAX : 1 / mass;
	_world_position = &_position;
	_trail.setMode(OF_PRIMITIVE_LINE_STRIP);
	_trail.addVertex(_position);
	_friction_k1 = 0.1;
	_friction_k2 = 0.0001;
	_mu_s = 0.15;



	if (!mesh_init_)
	{
		mesh_init_ = true;
		_mesh.load("test.ply");
		_mesh.clearTexCoords();
		_mesh.clearColors();
	}
	//_shoot_sound.load("chicken.wav");
	//_shoot_sound.setSpeed(pow(1 / (_radius), 0.25));
	//_shoot_sound.play();
}



void Particle::integrate(float dt, IntegrationMethods method)
{
	RigidBody::integrate(dt, method);
	// Generate the trail effect
	_time_counter += dt;
	if (_time_counter > _TRAIL_POINT_DELAY)
	{
		_trail.addVertex(_position);
		_time_counter -= _TRAIL_POINT_DELAY;
	}
	_trail.getVertices().back() = _position;
}

void Particle::resetMovement()
{
	_acceleration.set(0);
	_velocity.set(0);
}

void Particle::draw()
{
	_transformation.setPosition(_position);
	_transformation.setScale(_radius);
	_transformation.lookAt(_position + _velocity);
	_transformation.transformGL();
	ofSetColor(_color.x, _color.y, _color.z, _alpha);
	_mesh.draw();
	_transformation.restoreTransformGL();

	//ofSetColor(255,255,255, 120);
	//ofDrawSphere(_position, _radius);
	//ofDrawSphere(_position, _radius);
}

void Particle::drawNoLight()
{
	if (_draw_trail)
	{
		ofSetColor(_color.x, _color.y, _color.z, 255);
		_trail.draw();
	}
}

void Particle::testParticle() {
	Particle particle1;

	_ASSERT(particle1._position == Vector3D(0, 0, 0));
	_ASSERT(particle1._velocity == Vector3D(0, 0, 0));

	Vector3D init_position(1, 2, 3);
	Vector3D init_velocity(0, 0, 0);
	Vector3D init_acceleration(0, -g, 0);
	Particle particle2(init_position, init_velocity, 1., 1., Vector3D(1), 1.);
	_ASSERT(particle2._position == init_position);
	_ASSERT(particle2._velocity == init_velocity);
	//_ASSERT(particle2._acceleration == 0);
	_ASSERT(particle2._inverse_mass == 1.0f);

	particle2.integrate(1.0f, Particle::EULER);
	_ASSERT(particle2._velocity == Vector3D(0, 0, 0));
	_ASSERT(particle2._position == Vector3D(1, 2, 3));

	Particle particle4(Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f), 1., 1., Vector3D(1), 1.);

	// 1 st integration used to initialized p-1 with euler integration
	particle4.integrate(1.0f, Particle::VERLET);
	particle4.integrate(1.0f, Particle::VERLET);
	_ASSERT(particle4._position == Vector3D(1, 1, 1));
}
