#pragma once
#include "RigidBody.h"
class ImmovableBlock :
    public RigidBody
{
private:
    inline static bool mesh_init_ = false;
    inline static ofVboMesh _cube_mesh;
    inline static Vector3D _centroid;

    Vector3D getCentroid() const override { return _centroid; }
public:
    ImmovableBlock(const Vector3D& init_pos, const Vector3D& init_vel,
        const Quaternion& init_orientation, const Vector3D& init_angular_vel, const Vector3D& base_force_for_rotation, const Vector3D& application_point);
    void getInertiaMoment(Matrix3& mat) const override;
    void integrate(float dt, IntegrationMethods method = EULER) {}
#ifdef DEBUG_RIGID_BODY
    void drawApproximation() override;
#endif
};

