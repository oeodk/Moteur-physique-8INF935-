#pragma once
#include "Constrain.h"
class Particle;

class CableConstrain :
    public Constrain
{
private :
    float _length;
    RigidBody* _anchor_point;
public:
    CableConstrain(float length, RigidBody* anchor_point):_length(length), _anchor_point(anchor_point){}
    void updateConstrain(RigidBody* particle, float dt) override;
    const RigidBody* getAnchorPoint()const { return _anchor_point; }
};

