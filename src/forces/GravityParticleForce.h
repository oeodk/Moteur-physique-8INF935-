#pragma once
#include "ParticleForceGenerator.h"

class GravityParticleForce : public ParticleForceGenerator {
public:
	Vector3D _gravity;

	GravityParticleForce() : _gravity(0) {};
	GravityParticleForce(Vector3D gravity) : _gravity(gravity) {};

	void updateForce(Particle* particle, float dt) const override;
};

