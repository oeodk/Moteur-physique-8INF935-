#include "StaticTestCube.h"

StaticTestCube::StaticTestCube(const Vector3D& init_pos, const Vector3D& init_vel, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point)
	: RigidBody(init_pos, constants::EMPTY_VECTOR3D, 10, 10, init_orientation, init_angular_vel, "placing_block.mp3")
{
	if (!mesh_init_)
	{
		mesh_init_ = true;
		_cube_mesh.load("cube.ply");
		for (auto& v : _cube_mesh.getVertices())
		{
			v *= 5;
		}
		_cube_mesh.clearTexCoords();
		_centroid = Vector3D();
	}
	_mesh = &_cube_mesh;
	initAngularAcceleration(constants::EMPTY_VECTOR3D, constants::EMPTY_VECTOR3D);

	_hitbox = new BoxHitbox(&_position, &_orientation, constants::EMPTY_VECTOR3D, 50, 50, 50);
	_hitbox->update();
}

void StaticTestCube::getInertiaMoment(Matrix3& mat) const
{
	mat = constants::IDENTITY_MATRIX3 * ((1 / 6.f) * _mass * _radius * _radius);
}

#ifdef DEBUG_RIGID_BODY
void StaticTestCube::drawApproximation()
{
	ofSetColor(255, 0, 0, 120);
	ofDrawBox(_radius);
}
#endif
