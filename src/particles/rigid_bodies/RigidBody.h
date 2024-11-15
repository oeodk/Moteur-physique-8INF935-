#pragma once
#include "particles/Particle.h"
#include "Quaternion.h"
#include "Matrix3.h"
#include <vector>
#include <memory>

class RigidBody : public Particle
{
protected :
	Vector3D _angular_acceleration;
	Vector3D _angular_velocity;

	Quaternion  _orientation;

	ofVboMesh* _mesh;
	ofVboMesh _mass_center_mesh;
	ofNode _transformation;

	Matrix3 _inertia_moment;

	void initAngularAcceleration(const Vector3D& base_force_for_rotation, const Vector3D& application_point);

public :
	RigidBody(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius,
			  const Quaternion& init_orientation, const Vector3D& init_angular_vel);
	void integrate(float dt, IntegrationMethods method = EULER) override;
	void draw() override;
	virtual void getInertiaMoment(Matrix3& mat) const {};
};

