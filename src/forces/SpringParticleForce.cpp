#include "SpringParticleForce.h"

void SpringParticleForce::updateForce(Particle* particle, float dt) const {
	constexpr float STRETCH_LIMIT = 200;
	Vector3D force(*_hookingPoint - particle->getParticlePosition());
	if (force.squareNorm() > STRETCH_LIMIT * STRETCH_LIMIT)
	{
		force /= force.getNorm();
		force *= STRETCH_LIMIT;
	}
	particle->addForce(force * _elasticity);
}