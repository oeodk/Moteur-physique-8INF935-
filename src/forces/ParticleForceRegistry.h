#pragma once

#include "particles/Particle.h"
#include "ParticleForceGenerator.h"


class ParticleForceRegistry
{
public:
	ParticleForceRegistry();

	// Enregistrement d'une force et de son g�n�rateur
	struct ParticleForceRegistration
	{
		Particle* particle;
		ParticleForceGenerator* force_generator;
		ParticleForceRegistration(Particle* p, ParticleForceGenerator* generator) : particle(p), force_generator(generator){}
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

