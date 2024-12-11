#include "Masamune.h"

Masamune::Masamune(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 100, 70, init_orientation, init_angular_vel, "masamune.wav")
{
	if (!mesh_init_)
	{
		mesh_init_ = true;
		_masamune_mesh.load("Masamune.ply");
		_masamune_mesh.clearTexCoords();
		_centroid = Vector3D(0, 25, 0);
	}
	_mesh = &_masamune_mesh;
	initAngularAcceleration(base_force_for_rotation, application_point);

	_hitbox.push_back(new BoxHitbox(&_position, &_orientation, getCentroid(), 0.5, 10, 0.5));//17.5 can replace 10 for full hitbox
	_hitbox.back()->update();
	_hitbox.push_back(new BoxHitbox(&_position, &_orientation, getCentroid() + Vector3D(0, 27.5, 0), 0.5, 10, 0.5));
	_hitbox.back()->update();
	_hitbox.push_back(new BoxHitbox(&_position, &_orientation, getCentroid() - Vector3D(0, 27.5, 0), 0.5, 10, 0.5));
	_hitbox.back()->update();
}

void Masamune::getInertiaMoment(Matrix3& mat) const
{
	//const float ROD_LENGTH_SQUARED = _radius * _radius;
	//mat = _mass * Matrix3(
	//	Vector3D(0.5 * ROD_LENGTH_SQUARED, 0, 0),
	//	Vector3D(0, 0.5 * ROD_LENGTH_SQUARED, 0),
	//	Vector3D()
	//);

	constexpr float CYLINDER_RADIUS_SQUARED = 0.5 * 0.5;
	const float CYNLINDER_HEIGHT_SQUARED = 75 * 75;
	mat = _mass * Matrix3(
		Vector3D(0.25 * CYLINDER_RADIUS_SQUARED + (1 / 12.f) * CYNLINDER_HEIGHT_SQUARED, 0, 0),
		Vector3D(0, 0.25 * CYLINDER_RADIUS_SQUARED + (1 / 12.f) * CYNLINDER_HEIGHT_SQUARED, 0),
		Vector3D(0, 0, 0.5 * CYLINDER_RADIUS_SQUARED)
	);
}
#ifdef DEBUG_RIGID_BODY
void Masamune::drawApproximation()
{
	ofSetColor(255, 0, 0, 120);
	ofDrawCylinder(getCentroid(), 1, 75);
}
#endif
