#include "Masamune.h"

Masamune::Masamune(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, init_vel, 100, 75, init_orientation, init_angular_vel, "masamune.wav")
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
}

void Masamune::getInertiaMoment(Matrix3& mat) const
{
	const float ROD_LENGTH_SQUARED = _radius * _radius;
	mat = _mass * Matrix3(
		Vector3D(0.5 * ROD_LENGTH_SQUARED, 0, 0),
		Vector3D(0, 0.5 * ROD_LENGTH_SQUARED, 0),
		Vector3D()
	);
}
#ifdef DEBUG_RIGID_BODY
void Masamune::drawApproximation()
{
	ofSetColor(255, 0, 0, 120);
	ofDrawCylinder(getCentroid(), 1, _radius);
}
#endif
