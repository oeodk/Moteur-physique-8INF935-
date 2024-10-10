#pragma once

#include "../Particle.h"
#include "ParticleForceGenerator.h"


class ParticleForceRegistry
{
public:
	ParticleForceRegistry();

	// Enregistrement d'une force et de son générateur
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* force_generator;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	
private:
	Registry _registry;

public:
	void add(Particle* particle, ParticleForceGenerator* force_generator);
	void remove(Particle* particle, ParticleForceGenerator* force_generator);
	void clear();

	void updateForces(float dt);
};

