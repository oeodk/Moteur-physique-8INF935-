#pragma once
#include "Constrain.h"
class Particle;

class CableConstrain :
    public Constrain
{
private :
    float _length;
    Particle* _anchor_point;
public:
    CableConstrain(float length, Particle* anchor_point):_length(length), _anchor_point(anchor_point){}
    void updateConstrain(Particle* particle, float dt) override;
    const Particle* getAnchorPoint()const { return _anchor_point; }
};

