#pragma once
//#define DEBUG_RIGID_BODY
#include "../Particle.h"
#include "../../Quaternion.h"
#include "../../Matrix3.h"

#include <ofPixels.h>
#include <ofTexture.h>
#include <ofImage.h>
#include <ofGraphics.h>
#include <of3DGraphics.h>
#include <ofSoundPlayer.h>
#include <vector>
#include <memory>

class RigidBody : public Particle
{
protected :
	Vector3D _angular_acceleration;
	Vector3D _angular_velocity;

	Quaternion  _orientation;

	ofVboMesh* _mesh;
	ofVboMesh _mass_center_mesh;
	ofNode _transformation;
	ofTexture _texture;

	ofSoundPlayer _sound;
	ofSoundPlayer _hit_sound;

	Vector3D _torque;
	Matrix3 _inertia_moment;
	Matrix3 _inertia_moment_inverted;

	Vector3D _angular_velocity_increment_delay;

	std::vector<BoxHitbox*> _hitbox;

	void initAngularAcceleration(const Vector3D& base_force_for_rotation, const Vector3D& application_point);
	void displaceInertiaMoment(Matrix3& inertia_moment, const Vector3D& displacement);
	static Vector3D calculateCentroid(const ofVboMesh& mesh);
	virtual Vector3D getCentroid() const = 0;

	void solveCollisionTranslation(const Vector3D& contact_normal, const Vector3D& v_relative, RigidBody* other_rigid_body, int impact_count);
	void solveCollisionRotation(const Vector3D& contact_normal, RigidBody* other_rigid_body, const Vector3D& contact_point, int impact_count);

	
public :
	RigidBody(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius,
			  const Quaternion& init_orientation, const Vector3D& init_angular_vel,
			  const std::string& sound_path);
	~RigidBody() { for(auto& hitbox : _hitbox) delete hitbox; }

	void checkCollision(RigidBody* other_rigid_body, float dt);
	void solveCollision(RigidBody* other_rigid_body, const std::vector<CollisionData*>& collisions_data);

	void checkCollisionTerrain(class Terrain* terrain, float dt) override;
	void solveCollisionTerrain(class Terrain* terrain, const std::vector<CollisionData*>& collisions_data);


	void incrementAngularVelocityWithDelay(const Vector3D& increment) { _angular_velocity_increment_delay += increment; }

	void integrate(float dt, IntegrationMethods method = EULER) override;
	void draw() override;
	virtual void getInertiaMoment(Matrix3& mat) const {};
	void addForceToPoint(const Vector3D& base_force_for_rotation, const Vector3D& application_point);
#ifdef DEBUG_RIGID_BODY
	virtual void drawApproximation() = 0;
#endif
};

