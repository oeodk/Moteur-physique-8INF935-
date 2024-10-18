#include "Particle.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>
#include <stdexcept>

Particle::Particle() :_position(), _previous_position(), _velocity(), _acceleration(0, 0, 0), _inverse_mass(0), _mass(FLT_MAX), _radius(), _color(), _alpha(), _time_counter(0) {
	_world_position = &_position;
	_trail.setMode(OF_PRIMITIVE_POINTS);
	_trail.addVertex(_position);
	_trail.addVertex(_position);
}

Particle::Particle(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius, const Vector3D& color, float alpha)
	: _position(init_pos), _velocity(init_vel), _acceleration(0), _accum_force(0), _radius(radius), _color(color), _alpha(alpha), _time_counter(0) {
	_mass = mass;
	_inverse_mass = mass == 0 ? FLT_MAX : 1 / mass;
	_world_position = &_position;
	_trail.setMode(OF_PRIMITIVE_LINES);
	_trail.addVertex(_position);
}

void Particle::addForce(const Vector3D& force) {
	_accum_force += force;
}

void Particle::clearAccum() {
	_accum_force = 0;
}


void Particle::computeForces() {
	//std::cout << _accum_force.y << " x " << _inverse_mass << "\n";
	_acceleration = _accum_force * _inverse_mass;
}

void Particle::integrate(float dt, IntegrationMethods method) {
	switch (method) {
	case Particle::VERLET:
		integrateVerlet(dt); break;
	default:
		integrateEuler(dt); break;
	}


	// Generate the trail effect
	_time_counter += dt;
	//if (_time_counter > _TRAIL_POINT_DELAY) {
	//	_trail.addVertex(_position);
	//	_time_counter -= _TRAIL_POINT_DELAY;
	//}
	//_trail.getVertices().back() = _position;
}

Vector3D Particle::eulerUpdateVelocity(float dt) {
	return _velocity + dt * _acceleration;
}

Vector3D Particle::eulerUpdatePosition(float dt) {
	return _position + dt * _velocity;
}

void Particle::integrateEuler(float dt) {
	_velocity.set(eulerUpdateVelocity(dt));
	_position.set(eulerUpdatePosition(dt));
}

void Particle::integrateVerlet(float dt) {
	// can be done because particle are deleted below 80
	if (_previous_position != constants::EMPTY_VECTOR3D) {
		Vector3D copy_previous_position = _previous_position;
		_previous_position = _position;
		_position = 2 * _position - copy_previous_position + dt * dt * _acceleration;
	}
	else {
		_previous_position = _position;
		integrateEuler(dt);
	}
}

void Particle::draw() {
	ofSetColor(_color.x, _color.y, _color.z, _alpha);
	ofDrawSphere(_position, _radius);
}

void Particle::drawNoLight() {
	ofSetColor(_color.x, _color.y, _color.z, 255);
	_trail.draw();
}

void Particle::checkCollision(Particle* otherParticle, float dt) {
	const float distance = (*otherParticle->getPosition() - _position).squareNorm();
	const Vector3D contactNormal = (*otherParticle->getPosition() - _position).squareNorm();
	const Vector3D contactPoint = _position + _radius * contactNormal;
	const float jesaipas = (Vector3D::dotProduct(G_ACC * dt, contactNormal));
	if (jesaipas*jesaipas > (_velocity).squareNorm()) {
		addForce(G_ACC*-1);
		return;
	}
	if (distance <= _radius + otherParticle->getRadius()) {
		solveCollision(otherParticle, contactNormal, contactPoint);
	}
}


void Particle::testParticle() {
	Particle particle1;

	_ASSERT(particle1._position == Vector3D(0, 0, 0));
	_ASSERT(particle1._velocity == Vector3D(0, 0, 0));
	//_ASSERT(particle1._acceleration == Vector3D(0, -g, 0));
	_ASSERT(particle1._inverse_mass == 0);

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
