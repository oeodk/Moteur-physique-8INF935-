#pragma once
#include "Particle.h"


class CannonballParticle : public Particle
{
public:
	CannonballParticle() {};
	~CannonballParticle() = default;

	CannonballParticle(Vector3D init_pos, Vector3D init_acc);

private:

};