#pragma once
#include "Vector3D.h"
#include "GlobalConstants.h"
#include "constrain/Constrain.h"

#include <vector>
#include <memory>

class RigidBody
{
protected :
	Vector3D _position;
	Vector3D _previous_position;
	Vector3D _velocity;
	Vector3D _velocity_increment_delay;

	float _drag = 0.7;

	Vector3D _acceleration;
	Vector3D _accum_force;

	float _mass;
	float _inverse_mass;
	float _radius;

	float _friction_k1, _friction_k2, _mu_s;
	std::vector<std::shared_ptr<Constrain>> _constrains;

public :
	enum IntegrationMethods { EULER, VERLET };

	void addForce(const Vector3D& force);
	void clearAccum();

	float getRadius() const { return _radius; }
	const Vector3D& getAcceleration() const { return _acceleration; }
	float getMass() const { return _mass; }
	float getInverseMass() const { return _inverse_mass; }
	const Vector3D& getParticlePosition() const { return _position; }
	const Vector3D& getVelocity() const { return _velocity; }
	float getFrictionK1() const { return _friction_k1; }
	float getFrictionK2() const { return _friction_k2; }

	virtual void computeForces();
	virtual void integrate(float dt, IntegrationMethods method);

	virtual Vector3D eulerUpdateVelocity(float dt);
	Vector3D eulerUpdatePosition(float dt);
	void updateConstrain(float dt);

	void addConstrain(std::shared_ptr<Constrain> constrain) { _constrains.push_back(constrain); }
	void removeConstrains(std::shared_ptr<Constrain> constrain);
	const std::vector< std::shared_ptr<Constrain>>& getConstrains() const { return _constrains; }
	/* Compute the next position and velocity using the Euler integration formula.
	   Takes the frame length (in milliseconds) as parameter. */

	void integrateEuler(float dt);

	/* Compute the next position using the Verlet integration formula.
	   Takes the frame length (in milliseconds) as parameter. */

	void integrateVerlet(float dt);

	void incrementVelocityWithDelay(const Vector3D& increment) { _velocity_increment_delay += increment; }

	void checkCollision(RigidBody* otherParticle, float dt);

	void solveCollision(RigidBody* otherParticle, const Vector3D& v_relative, float chevauchement, const Vector3D& contact_normal);

	void checkCollisionTerrain(class Terrain* terrain, float dt);

	void solveCollisionTerrain(const Vector3D& v_relative, float chevauchement, const Vector3D& contact_normal);

};

