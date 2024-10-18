#include "GravityParticleForce.h"

GravityParticleForce::GravityParticleForce(const Vector3D & gravity) : _gravity(gravity) {}

void GravityParticleForce::updateForce(Particle* particle, float dt) const {
	const float mass = particle->getMass();

	if (mass == 0) return;

	particle->addForce(_gravity * mass);
}