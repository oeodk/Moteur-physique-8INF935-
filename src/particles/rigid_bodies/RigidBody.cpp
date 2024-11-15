#include "RigidBody.h"
#include "ofGraphics.h"
#include "of3DGraphics.h"

RigidBody::RigidBody(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: Particle(init_pos, init_vel * 200, mass, radius, Vector3D(255, 255, 255), 255)
	, _angular_velocity(init_angular_vel), _orientation(init_orientation)
{
	Matrix3 inertia_moment;
	getInertiaMoment(inertia_moment);
	_angular_acceleration = inertia_moment * Vector3D::crossProduct(application_point - _position, base_force_for_rotation);
}

void RigidBody::integrate(float dt, IntegrationMethods method)
{
	Particle::integrate(dt, method);
	_angular_velocity = _angular_velocity + _angular_acceleration * dt;
	glm::quat tmp(_orientation);

	//_orientation += 0.5f * Quaternion(0,_angular_velocity) * _orientation * dt;
	// TO DO : uncomment the line above and remove the two line bellow
	tmp += 0.5f * glm::quat(0,_angular_velocity) * tmp * dt;
	_orientation = Quaternion(tmp.w, tmp.x, tmp.y, tmp.z);

	_orientation.normalize();
}

void RigidBody::draw()
{
	_transformation.setPosition(_position);

	_transformation.setOrientation(_orientation);

	_transformation.transformGL();
	
	ofSetColor(_color.x, _color.y, _color.z, _alpha);
	_mesh->draw();

	_transformation.restoreTransformGL();
}
