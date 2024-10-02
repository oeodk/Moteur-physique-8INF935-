#include "GravityParticleForce.h"

void GravityParticleForce::updateForce(Particle* particle, float dt) const {
	const float mass = particle->getMass();

	if (mass == 0) return;

	particle->addForce(gravity * mass);
}