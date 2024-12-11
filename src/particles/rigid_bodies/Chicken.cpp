#include "Chicken.h"

Chicken::Chicken(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 10, 15, init_orientation, init_angular_vel, "chicken.wav")
{
	if (!mesh_init_)
	{
		mesh_init_ = true;
		_chicken_mesh.load("chicken.ply");
		_chicken_mesh.clearTexCoords();
		_centroid = Vector3D();
	}
	_mesh = &_chicken_mesh;
	initAngularAcceleration(base_force_for_rotation, application_point);

	_hitbox.push_back(new BoxHitbox(&_position, &_orientation, constants::EMPTY_VECTOR3D, 7.5, 7.5, 7.5));
	_hitbox.back()->update();
}

void Chicken::getInertiaMoment(Matrix3& mat) const
{
	mat = constants::IDENTITY_MATRIX3 * (0.4 * _mass * _radius * _radius);
}
#ifdef DEBUG_RIGID_BODY
void Chicken::drawApproximation()
{
	ofSetColor(255, 0, 0, 120);
	ofDrawSphere(_radius);
}
#endif
