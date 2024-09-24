#include "PhysicsEngine.h"

void PhysicsEngine::updateParticles(float dt, const std::vector<class Particle*>& particles) {
	//ComputeForces();
	for (auto& p : particles) {
		p->integrate(dt, _integration_method);
	}
}