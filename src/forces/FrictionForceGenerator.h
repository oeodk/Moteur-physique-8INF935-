#pragma once
#include "ParticleForceGenerator.h"
class FrictionForceGenerator :
    public ParticleForceGenerator
{
public :
    void updateForce(Particle* particle, float dt) const override;
};

