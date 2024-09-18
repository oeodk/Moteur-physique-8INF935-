#include "PhysicsEngine.h"

void PhysicsEngine::updateParticles(float dt, std::vector<class Particle*> particles) {
	//ComputeForces();

	for (auto & p : particles) {
		p->integrate(dt, Particle::IntegrationMethods::EULER);
	}
}