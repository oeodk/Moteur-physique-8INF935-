#pragma once
#include "Particle.h"


class BulletParticle : public Particle {
public:
	BulletParticle() {};
	~BulletParticle() = default;

	BulletParticle(const Vector3D& init_pos, const Vector3D& init_dir);

private:

};

