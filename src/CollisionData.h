#pragma once
#include "Vector3D.h"

struct CollisionData
{
	bool _collision_detected = false;
	Vector3D _impact_point; //nul constructor exist
	Vector3D _contact_normal; //nul constructor exist
	float _penetration_distance = FLT_MAX;
};

