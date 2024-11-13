#pragma once
#include "Constrain.h"
class BarConstrain :
    public Constrain
{
private:
    float _length;
    RigidBody* _anchor_point;
public:
    BarConstrain(float length, RigidBody* anchor_point) :_length(length), _anchor_point(anchor_point) {}
    void updateConstrain(RigidBody* particle, float dt) override;
};
