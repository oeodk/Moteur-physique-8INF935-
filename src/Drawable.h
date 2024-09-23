#pragma once
#include "Vector3D.h"

// Interface used to render objets

class Drawable
{
protected :
	Vector3D* _world_position;
public :
	virtual void draw() {}
	virtual void drawNoLight() {}
	const Vector3D* getPosition() const { return _world_position; }
};

