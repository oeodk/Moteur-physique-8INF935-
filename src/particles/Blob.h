#pragma once
#include "Particle.h"
#include "forces/ParticleForceRegistry.h"
#include "forces/GravityParticleForce.h"
#include <of3dPrimitives.h>

class Blob :
    public Particle
{
private:
    static constexpr float _GRAB_RANGE = 50;
    static constexpr float _LOSE_RANGE = 500;
    
    std::vector<Particle*> _blob_particles;
    std::vector<Particle*>* _world_particles;
    ParticleForceRegistry* _world_force_registry;

    GravityParticleForce _gravity_nullification;
    struct GeneratorSource
    {
        ParticleForceGenerator* generator;
        Particle* source;
    };
    std::unordered_map<Particle*, std::vector<GeneratorSource>> _forces;
    
    struct ParticlePair
    {
        Particle* p1, * p2;
        ParticlePair(Particle* p, Particle* pp):p1(p), p2(pp){}
    };
    std::vector<ParticlePair> _particles_links;

    ofVboMesh _joins;
public :
    Blob(std::vector<Particle*>* world_particles, ParticleForceRegistry* force_registry, const Vector3D& init_pos, const Vector3D& init_vel);
    void updateBlob();
    void drawNoLight() override;
};

