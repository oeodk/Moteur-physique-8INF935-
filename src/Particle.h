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
	float _inverse_mass;
	float _radius;

	Vector3D _color;
	float _alpha;

	ofVboMesh _trail;
	const float _TRAIL_POINT_DELAY = 0.25;
	float _time_counter;

public:
	enum IntegrationMethods { EULER, VERLET };

	Particle();
	Particle(const Vector3D& init_pos, const Vector3D& init_vel, const Vector3D& init_acc, float mass, float radius, const Vector3D& color, float alpha);
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

	void draw() override;
	void drawNoLight() override;

	static void testParticle();
};
