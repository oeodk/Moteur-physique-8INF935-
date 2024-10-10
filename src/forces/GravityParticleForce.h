#pragma once
#include "ParticleForceGenerator.h"

class GravityParticleForce : public ParticleForceGenerator {
protected:
	Vector3D _gravity;

public:
	GravityParticleForce(Vector3D& gravity);
	void updateForce(Particle* particle, float dt) const override;
};

