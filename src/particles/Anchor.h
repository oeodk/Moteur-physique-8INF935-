#pragma once
#include "Particle.h"
class Anchor :
    public Particle
{
public:
	Anchor(const Vector3D& init_pos, const Vector3D& color);
	Vector3D eulerUpdateVelocity(float dt) override { return Vector3D(); }

};

