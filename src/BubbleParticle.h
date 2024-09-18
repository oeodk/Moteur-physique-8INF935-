#pragma once
#include "Particle.h"


class BubbleParticle : public Particle
{
public:
	BubbleParticle() {};
	~BubbleParticle() = default;

	BubbleParticle(Vector3D init_pos, Vector3D init_dir, Vector3D init_acc);

private:

};
