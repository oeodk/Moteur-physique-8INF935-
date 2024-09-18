#pragma once
#include "Vector3D.h"

class Drawable
{
protected :
	Vector3D* _world_position;
public :
	virtual void draw() = 0;
	const Vector3D* getPosition() const { return _world_position; }
};

