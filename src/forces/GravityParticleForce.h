#pragma once
#include "ParticleForceGenerator.h"

class GravityParticleForce : public ParticleForceGenerator {
	Vector3D gravity;
	void updateForce(Particle* particle, float dt) const override;
};

