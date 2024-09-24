#pragma once
#include "Vector3D.h"

enum BulletType
{
	BULLET,
	CANNONBALL,
	FIREBALL,
	BUBBLE
};

const Vector3D G_ACC = Vector3D(0, -9.81, 0);

enum ChunkLod
{
	HIGH, MEDUIM, LOW, NONE
};