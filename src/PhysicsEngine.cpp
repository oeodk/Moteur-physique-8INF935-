#include "PhysicsEngine.h"

void PhysicsEngine::updateParticles(float dt, const std::vector<class Particle*>& particles) {
	for (auto& p : particles) {
		//std::cout << std::to_string(p->getAcceleration().x) << ", " << std::to_string(p->getAcceleration().y) << ", " << std::to_string(p->getAcceleration().z) << "\n";
		p->computeForces();
		p->integrate(dt, _integration_method);
		p->clearAccum();
	}
}