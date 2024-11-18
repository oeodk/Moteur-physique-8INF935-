#include "Goat.h"

Goat::Goat(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 10, 10, init_orientation, init_angular_vel)
{
	initAngularAcceleration(base_force_for_rotation, application_point);
	if (!mesh_init_)
	{
		mesh_init_ = true;
		_goat_mesh.load("goatTpose.ply");
		//_mesh.clearTexCoords();
		_goat_mesh.clearColors();
		_texture_image.load("goat_texture.png");
		for (auto& v : _goat_mesh.getTexCoords())
		{
			v.x *= 128;
			v.y *= 128;
		}
	}
	ofLoadImage(_texture, "goat_texture.png");
	//_texture.loadData(_texture_image);
	_mesh = &_goat_mesh;
}

void Goat::getInertiaMoment(Matrix3& mat) const
{
}
