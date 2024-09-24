#pragma once

#include <vector>
using namespace std;

#include "Particle.h"

class PhysicsEngine
{
private :
	Particle::IntegrationMethods _integration_method = Particle::IntegrationMethods::VERLET;
public:
	void updateParticles(float dt, const std::vector<class Particle*>& particles);
	void changeIntegrationMethod() { _integration_method = static_cast<Particle::IntegrationMethods>(1 - static_cast<int>(_integration_method)); }
};

