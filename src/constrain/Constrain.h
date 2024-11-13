#pragma once

class Constrain
{
public:
	virtual void updateConstrain(class RigidBody* particle, float dt) = 0;
};

