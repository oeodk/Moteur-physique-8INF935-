#pragma once
#include "Constrain.h"
class BarConstrain :
    public Constrain
{
private:
    float _length;
    Particle* _anchor_point;
public:
    BarConstrain(float length, Particle* anchor_point) :_length(length), _anchor_point(anchor_point) {}
    void updateConstrain(Particle* particle, float dt) override;
};
