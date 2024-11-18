#include "Goat.h"

Goat::Goat(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 50, 25, init_orientation, init_angular_vel, "goat.wav")
{
	if (!mesh_init_)
	{
		mesh_init_ = true;
		_goat_mesh.load("goatTpose.ply");
		//_mesh.clearTexCoords();
		_goat_mesh.clearColors();
		_texture_image.load("Goat_texture.png");
		for (auto& v : _goat_mesh.getTexCoords())
		{
			v.x *= 128;
			v.y *= 128;
		}
		_centroid = Vector3D();
	}
	_texture.loadData(_texture_image);
	_mesh = &_goat_mesh;
	initAngularAcceleration(base_force_for_rotation, application_point);
}

void Goat::getInertiaMoment(Matrix3& mat) const
{
	mat = constants::IDENTITY_MATRIX3 * ((1/6.f) * _mass * _radius * _radius);
}

#ifdef DEBUG_RIGID_BODY
void Goat::drawApproximation()
{
	ofSetColor(255, 0, 0, 120);
	ofDrawBox(_radius);
}
#endif
