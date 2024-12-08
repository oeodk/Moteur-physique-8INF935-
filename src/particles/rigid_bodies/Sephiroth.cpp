#include "Sephiroth.h"

Sephiroth::Sephiroth(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 80, 60, init_orientation, init_angular_vel, "sephiroth.wav")
{
	_mesh = &_sephiroth_mesh;
	initAngularAcceleration(base_force_for_rotation, application_point);

	_hitbox = new BoxHitbox(&_position, &_orientation, _centroid, 7.5, 30, 7.5);
	_hitbox->update();
}

void Sephiroth::getInertiaMoment(Matrix3& mat) const
{
	constexpr float CYLINDER_RADIUS_SQUARED = 15 * 15;
	const float CYNLINDER_HEIGHT_SQUARED = _radius * _radius;
	mat = _mass * Matrix3(
		Vector3D(0.25 * CYLINDER_RADIUS_SQUARED + (1 / 12.f) * CYNLINDER_HEIGHT_SQUARED, 0, 0),
		Vector3D(0, 0.25 * CYLINDER_RADIUS_SQUARED + (1 / 12.f) * CYNLINDER_HEIGHT_SQUARED, 0),
		Vector3D(0, 0, 0.5 * CYLINDER_RADIUS_SQUARED)
	);
}

void Sephiroth::loadMesh()
{
	if (!mesh_init_)
	{
		mesh_init_ = true;
		_sephiroth_mesh.load("Sephiroth.ply");
		_sephiroth_mesh.clearTexCoords();
		_centroid = Vector3D(0, -25, 0);
	}
}

#ifdef DEBUG_RIGID_BODY
void Sephiroth::drawApproximation()
{
	ofSetColor(255, 0, 0, 120);
	ofDrawCylinder(_centroid, 15, _radius);
}
#endif
