#pragma once
#include "Particle.h"


class FireballParticle : public Particle
{
public:
	FireballParticle() {};
	~FireballParticle() = default;

	FireballParticle(Vector3D init_pos, Vector3D init_acc);

private:

};
