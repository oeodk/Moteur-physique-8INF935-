#include "FrictionForceGenerator.h"

void FrictionForceGenerator::updateForce(Particle* particle, float dt) const
{
	const Vector3D& particle_velocity = particle->getVelocity();
	if(particle_velocity.squareNorm() != 0)
	{
		Vector3D velocity_direction(particle_velocity);
		velocity_direction.normalize();
		velocity_direction.invert();
		particle->addForce(velocity_direction * (particle->getFrictionK1() * particle_velocity.getNorm() + particle->getFrictionK2() * particle_velocity.squareNorm()));
	}
}
