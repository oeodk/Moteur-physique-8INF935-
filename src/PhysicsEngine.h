#pragma once

#include <vector>
using namespace std;

#include "Particle.h"

class PhysicsEngine
{
public:
	void updateParticles(float dt, std::vector<class Particle*> particles);
};

