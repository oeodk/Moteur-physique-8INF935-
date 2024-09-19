#pragma once
#include "Vector3D.h"
#include "Drawable.h"

constexpr float g = 9.81;

class Particle : public Drawable
{
private:
	Vector3D _position;
	Vector3D _previous_position;
	Vector3D _velocity;
	Vector3D _acceleration;
	float _inverse_mass;
	float _radius;

	Vector3D _color;
	float _alpha;
	


public:
	enum IntegrationMethods { EULER, VERLET };

	Particle() : _position(), _previous_position(), _velocity(), _acceleration(0, -g, 0), _inverse_mass(0), _radius(), _color(), _alpha() { _world_position = &_position; };
	Particle(Vector3D init_pos, Vector3D init_vel, Vector3D init_acc, float mass, float radius, Vector3D color, float alpha);
	~Particle() = default;

	void integrate(float dt, IntegrationMethods method);

	Vector3D eulerUpdateVelocity(float dt);
	Vector3D eulerUpdatePosition(float dt);

	/* Compute the next position and velocity using the Euler integration formula.
	   Takes the frame length (in milliseconds) as parameter. */

	void integrateEuler(float dt);

	/* Compute the next position using the Verlet integration formula.
	   Takes the frame length (in milliseconds) as parameter. */

	void integrateVerlet(float dt);

	void draw();


};
