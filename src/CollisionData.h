#pragma once
#include "Vector3D.h"

struct CollisionData
{
	bool _collision_detected;
	Vector3D _impact_point;
	float _penetration_distance;
};

