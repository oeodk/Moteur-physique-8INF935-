#include "BarConstrain.h"
#include "RigidBody.h"

void BarConstrain::updateConstrain(RigidBody* particle, float dt)
{
	Vector3D dist(_anchor_point->getParticlePosition() - particle->getParticlePosition());
	const float DELTA_DIST = dist.getNorm() - _length;
	dist.normalize();
	dist.invert();
	particle->solveCollision(_anchor_point, particle->getVelocity() - _anchor_point->getVelocity(), DELTA_DIST, dist);
	
}
