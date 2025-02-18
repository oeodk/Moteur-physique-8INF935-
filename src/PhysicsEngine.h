#pragma once

#include <vector>
#include "particles/Particle.h"
#include "ocTree.h"

class PhysicsEngine {
private:
	Particle::IntegrationMethods _integration_method = Particle::IntegrationMethods::EULER;
public:
	void updateParticles(float dt, const std::vector<class Particle*>& particles, class Terrain* terrain);
	void updateRigidBody(float dt, const std::vector<class RigidBody*>& particles, class Terrain* terrain, ocTree& octree);
	void changeIntegrationMethod() { _integration_method = static_cast<Particle::IntegrationMethods>(1 - static_cast<int>(_integration_method)); }
};

