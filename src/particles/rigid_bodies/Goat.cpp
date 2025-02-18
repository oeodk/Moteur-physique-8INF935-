#include "Goat.h"

inline static constexpr const char* GOAT_SCREAM[18] = {"Goat_pre_ram1.wav", "Goat_pre_ram2.wav", "Goat_pre_ram3.wav", "Goat_pre_ram4.wav", "Goat_scream1.wav", "Goat_scream2.wav", "Goat_scream3.wav", "Goat_scream4.wav", "Goat_scream5.wav", "Goat_scream6.wav", "Goat_scream7.wav", "Goat_scream8.wav", "Goat_scream9.wav", "Goat_screaming_pre_ram1.wav", "Goat_screaming_pre_ram2.wav", "Goat_screaming_pre_ram3.wav", "Goat_screaming_pre_ram4.wav", "Goat_screaming_pre_ram5.wav"};

Goat::Goat(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 50, 28, init_orientation, init_angular_vel, std::string("Goat_sound/") + GOAT_SCREAM[rand() % 18])
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

	_hitbox.push_back(new BoxHitbox(&_position, &_orientation, constants::EMPTY_VECTOR3D, 12.5, 20, 12.5));
	_hitbox.back()->update();
}

void Goat::getInertiaMoment(Matrix3& mat) const
{
	mat = constants::IDENTITY_MATRIX3 * ((1/6.f) * _mass * 25 * 25);
}

#ifdef DEBUG_RIGID_BODY
void Goat::drawApproximation()
{
	ofSetColor(255, 0, 0, 120);
	ofDrawBox(25);
}
#endif
