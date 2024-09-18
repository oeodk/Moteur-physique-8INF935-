#pragma once
#include "Particle.h"


class BulletParticle: public Particle
{
public:
	BulletParticle() {};
	~BulletParticle () = default;

	BulletParticle(Vector3D init_pos, Vector3D init_acc);

private:

};

