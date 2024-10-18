#pragma once
#include "ParticleForceGenerator.h"

class SpringParticleForce : public ParticleForceGenerator {
public:

	float _elasticity;
	float _restingLength;
	const Vector3D* _hookingPoint;

	SpringParticleForce() : _elasticity(0), _restingLength(0), _hookingPoint(0) {};
	SpringParticleForce(float elasticity, float restingLength, const Vector3D* hookingPoint) :
		_elasticity(elasticity), _restingLength(restingLength), _hookingPoint(hookingPoint) {};

	void updateForce(Particle* particle, float dt) const override;

};