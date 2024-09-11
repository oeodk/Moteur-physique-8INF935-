#pragma once
#include "Vector3D.h"

class Particle
{
private:
	Vector3D _position;
	Vector3D _velocity;
	Vector3D _acceleration;

	float _inverse_mass;


public:
	Particle();
	~Particle();


};

Particle::Particle() : _position(), _velocity(), _acceleration(), _inverse_mass(0)
{
	
}

Particle::Particle(Vector3D init_pos, Vector3D init_vel, Vector3D init_acc, float _mass) : _position(init_pos), _velocity(init_vel), _acceleration(init_acc)
{
	_inverse_mass = _mass == 0 ? 0 : 1 / _mass;
}

Particle::~Particle()
{
}