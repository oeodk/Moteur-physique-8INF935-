#pragma once
#include "particles/Particle.h"

class ParticleForceGenerator {
public:
	virtual void updateForce(Particle* particle, float dt) const = 0;
};

