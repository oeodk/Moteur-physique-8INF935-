#include "SpringParticleForce.h"

void SpringParticleForce::updateForce(Particle* particle, float dt) const {

	float length = particle->getPosition() - _hookingPoint;

	particle->addForce(_elasticity * length);

}