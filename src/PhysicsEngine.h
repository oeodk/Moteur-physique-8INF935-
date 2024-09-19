#pragma once

#include <vector>
using namespace std;

#include "Particle.h"

class PhysicsEngine
{
public:
	void updateParticles(float dt, const std::vector<class Particle*>& particles);
};

