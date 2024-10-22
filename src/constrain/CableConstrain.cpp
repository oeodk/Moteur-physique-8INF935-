#include "CableConstrain.h"
#include "particles/Particle.h"

void CableConstrain::updateConstrain(Particle* particle, float dt)
{
	Vector3D dist(_anchor_point->getParticlePosition() - particle->getParticlePosition());
	// If particles are too far the spring became a cable
	if (dist.squareNorm() > _length * _length)
	{
		const float DELTA_DIST = dist.getNorm() - _length;
		dist.normalize();
		dist.invert();
		particle->solveCollision(_anchor_point, particle->getVelocity() - _anchor_point->getVelocity(), DELTA_DIST, dist);
	}
}
