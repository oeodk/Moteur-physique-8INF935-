#include "Chicken.h"

Chicken::Chicken(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 10, 10, init_orientation, init_angular_vel, base_force_for_rotation, application_point)
{
	if (!mesh_init_)
	{
		mesh_init_ = true;
		_chicken_mesh.load("chicken.ply");
		//_mesh.clearTexCoords();
		//_mesh.clearColors();
	}
	_mesh = &_chicken_mesh;
}

void Chicken::getInertiaMoment(Matrix3& mat) const
{
	mat = constants::IDENTITY_MATRIX3 * (0.4 * _mass * _radius * _radius);
	
}
