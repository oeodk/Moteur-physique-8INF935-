#include "Particle.h"

void Particle::integrate(float duration) {
	integrateVerlet(duration);
}

void Particle::integrateVerlet(float duration) {
	Vector3D copy_previous_position = _previous_position;
	_previous_position = _position;
	_position = 2 * _position - copy_previous_position + duration * duration * _acceleration;
}
