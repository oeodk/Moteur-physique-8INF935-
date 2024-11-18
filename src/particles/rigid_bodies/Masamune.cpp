#include "Masamune.h"

Masamune::Masamune(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 10, 10, init_orientation, init_angular_vel)
{
	initAngularAcceleration(base_force_for_rotation, application_point);
	if (!mesh_init_)
	{
		mesh_init_ = true;
		_masamune_mesh.load("Masamune.ply");
		_masamune_mesh.clearTexCoords();
	}
	_mesh = &_masamune_mesh;
}

void Masamune::getInertiaMoment(Matrix3& mat) const
{
}
