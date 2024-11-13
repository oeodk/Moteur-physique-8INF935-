#pragma once
#include "../Drawable.h"
#include "ofVboMesh.h"
#include "ofNode.h"

#include "../RigidBody.h"
#include <ofSoundPlayer.h>

class Particle : public Drawable, public RigidBody
{
protected:
	

	Vector3D _color;
	float _alpha;

	ofVboMesh _trail;
	static constexpr float _TRAIL_POINT_DELAY = 0.25;
	float _time_counter;


	inline static bool mesh_init_ = false;
	inline static ofVboMesh _mesh;
	ofNode _transformation;
	
	ofSoundPlayer _shoot_sound;

public:
	inline static bool _draw_trail = true;

	Particle();
	Particle(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius, const Vector3D& color, float alpha);
	~Particle() = default;

	void integrate(float dt, IntegrationMethods method) override;

	void resetMovement();

	void draw() override;
	void drawNoLight() override;

	static void testParticle();
};
