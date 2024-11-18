#pragma once
#include "RigidBody.h"
class Sephiroth :
    public RigidBody
{
private:
    inline static bool mesh_init_ = false;
    inline static ofVboMesh _sephiroth_mesh;
public:
    Sephiroth(const Vector3D& init_pos, const Vector3D& init_vel,
        const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point);
    void getInertiaMoment(Matrix3& mat) const override;

};

