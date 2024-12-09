#pragma once
#include "HitBoxPrimitive.h"

#ifdef HITBOX_DEBUG
#include <ofVboMesh.h>
#endif

class BoxHitbox 
	: public HitBoxPrimitive
{
	float _half_width, _half_height, _half_depth;
	std::array<Vector3D, 8> _hitbox_corner;
	std::array<Vector3D, 3> _faces_normals;

#ifdef HITBOX_DEBUG
	ofVboMesh _points;
#endif
public:
	/*
	w -> x
	h -> y
	d -> z
	*/
	BoxHitbox(Vector3D* position, Quaternion* orientation, const Vector3D& position_offset, float half_width, float half_height, float half_depth);
	bool doCollideWithBox(const class BoxHitbox* box_hitbox, std::array<CollisionData, 8>& collision_data) override;
	void update() override;
	bool doCollideWithTerrain(class Terrain* terrain, std::array<CollisionData, 8>& colision_data) override;
#ifdef HITBOX_DEBUG
	void drawBox() override;
	void drawCorner() override;
#endif // HITBOX_DEBUG
};

