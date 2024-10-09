#pragma once
#include "Vector3D.h"
#include "Drawable.h"
#include "ofVboMesh.h"
#include "GlobalConstants.h"

class Particle : public Drawable {
private:
	Vector3D _position;
	Vector3D _previous_position;
	Vector3D _velocity;

	Vector3D _acceleration;
	Vector3D _accum_force;
	
	float _mass;
	float _inverse_mass;
	float _radius;

	Vector3D _color;
	float _alpha;

	ofVboMesh _trail;
	static constexpr float _TRAIL_POINT_DELAY = 0.25;
	float _time_counter;

public:
	enum IntegrationMethods { EULER, VERLET };

	Particle();
	Particle(const Vector3D& init_pos, const Vector3D& init_vel, const Vector3D& init_acc, float mass, float radius, const Vector3D& color, float alpha);
	~Particle() = default;

	void addForce(const Vector3D& force);
	void clearAccum();

	float getMass() const { return _mass; }
	float getInverseMass() const { return _inverse_mass; }
	float getRadius() const { return _radius; }

	void integrate(float dt, IntegrationMethods method);

	Vector3D eulerUpdateVelocity(float dt);
	Vector3D eulerUpdatePosition(float dt);

	/* Compute the next position and velocity using the Euler integration formula.
	   Takes the frame length (in milliseconds) as parameter. */

	void integrateEuler(float dt);

	/* Compute the next position using the Verlet integration formula.
	   Takes the frame length (in milliseconds) as parameter. */

	void integrateVerlet(float dt);

	void draw() override;
	void drawNoLight() override;

	void checkCollision(Particle* otherParticle, float dt);

	void solveCollision(Particle* otherParticle, Vector3D contactNormal, Vector3D contactPoint);

	static void testParticle();
};
