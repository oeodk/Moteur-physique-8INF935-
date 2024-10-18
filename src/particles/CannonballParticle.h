#pragma once
#include "Particle.h"


class CannonballParticle : public Particle {
public:
	CannonballParticle() {};
	~CannonballParticle() = default;

	CannonballParticle(const Vector3D& init_pos, const Vector3D& init_dir);

private:

};