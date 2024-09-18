#include "Particle.h"

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
