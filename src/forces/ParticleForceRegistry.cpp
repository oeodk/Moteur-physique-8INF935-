#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry() {}

void ParticleForceRegistry::add(Particle *particle, ParticleForceGenerator *force_generator) {
	ParticleForceRegistration pfr;
	pfr.particle = particle;
	pfr.force_generator = force_generator;
	_registry.push_back(pfr);
}

void ParticleForceRegistry::remove(Particle* particle, ParticleForceGenerator* force_generator) {
	for (int i = 0; i < _registry.size(); i++)
		if (_registry[i].particle == particle && _registry[i].force_generator == force_generator) {
			_registry.erase(_registry.begin() + i);
			return;
		}
}

void ParticleForceRegistry::clear() {
	_registry.clear();
}

void ParticleForceRegistry::updateForces(float dt) {
	for (ParticleForceRegistration pfr : _registry)
		pfr.force_generator->updateForce(pfr.particle, dt);
}