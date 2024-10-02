#pragma once
#include "Particle.h"


class FireballParticle : public Particle {
public:
	FireballParticle() {};
	~FireballParticle() = default;

	FireballParticle(const Vector3D& init_pos, const Vector3D& init_dir, const Vector3D& init_acc);

private:

};
