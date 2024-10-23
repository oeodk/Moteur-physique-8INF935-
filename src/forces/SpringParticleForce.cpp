#include "SpringParticleForce.h"

void SpringParticleForce::updateForce(Particle* particle, float dt) const {
	const Vector3D force(particle->getParticlePosition()  - *_hooking_point);
	if(force.squareNorm()!= 0)
	{
		Vector3D dir(force);
		dir.normalize();
		particle->addForce(((_resting_length - force.getNorm()) * _elasticity) * dir);
	}
}