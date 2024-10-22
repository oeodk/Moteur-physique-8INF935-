#pragma once
#include "ParticleForceGenerator.h"

class SpringParticleForce : public ParticleForceGenerator {
public:

	float _elasticity;
	float _resting_length;
	const Vector3D* _hooking_point;

	SpringParticleForce() : _elasticity(0), _resting_length(0), _hooking_point(0) {};
	SpringParticleForce(float elasticity, float resting_length, const Vector3D * hooking_point) :
		_elasticity(elasticity), _resting_length(resting_length), _hooking_point(hooking_point) {};

	void updateForce(Particle* particle, float dt) const override;

};