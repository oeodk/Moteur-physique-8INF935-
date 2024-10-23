#pragma once
#include "Particle.h"
#include "../forces/GravityParticleForce.h"


class BubbleParticle : public Particle {
public:
	BubbleParticle() {};
	~BubbleParticle() = default;
	BubbleParticle(const Vector3D& init_pos, const Vector3D& init_dir);
private:

};
