#include "Particle.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>

Particle::Particle(Vector3D init_pos, Vector3D init_vel, Vector3D init_acc, float mass, float radius, Vector3D color, float alpha)
	: _position(init_pos), _velocity(init_vel), _acceleration(init_acc), _radius(radius), _color(color), _alpha(alpha) {
	_inverse_mass = mass == 0 ? 0 : 1 / mass;
	_world_position = &_position;
}

void Particle::integrate(float dt, IntegrationMethods method) {
	switch (method)
	{
		case Particle::VERLET:
			integrateVerlet(dt); break;
		default:
			integrateEuler(dt); break;
	}
}

Vector3D Particle::eulerUpdateVelocity(float dt) {
	return _velocity + dt * _acceleration;
}

Vector3D Particle::eulerUpdatePosition(float dt) {
	return _position + dt * _velocity;
}

void Particle::integrateEuler(float dt) {
	_velocity = eulerUpdateVelocity(dt);
	_position = eulerUpdatePosition(dt);
}

void Particle::integrateVerlet(float dt) {
	Vector3D copy_previous_position = _previous_position;
	_previous_position = _position;
	_position = 2 * _position - copy_previous_position + dt * dt * _acceleration;
}

void Particle::draw() {
	ofSetColor(ofColor::red);
	ofDrawSphere(_position, _radius);
}
