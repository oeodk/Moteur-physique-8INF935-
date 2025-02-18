#pragma once
#include "Vector3D.h"

enum BulletType {
	BULLET,
	CANNONBALL,
	FIREBALL,
	BUBBLE,
	BLOB,
	CHICKEN,
	SEPHIROTH,
	MASAMUNE,
	GOAT,
	STATIC_CUBE,
	MOVING_CUBE,
	IMMOBILE_BLOCK,
	E_END
};

constexpr float g = 9.81;

inline static const Vector3D G_ACC = Vector3D(0, -9.81, 0);

constexpr int WINDOW_BASE_HEIGHT = 1024;
constexpr int WINDOW_BASE_WIDTH = 1024;

enum ChunkLod {
	HIGH, MEDUIM, LOW, NONE
};