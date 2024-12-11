#include "PhysicsEngine.h"
#include "particles/rigid_bodies/RigidBody.h"

void PhysicsEngine::updateParticles(float dt, const std::vector<class Particle*>& particles, Terrain* terrain) {
    for (auto& p : particles) {
        //std::cout << std::to_string(p->getAcceleration().x) << ", " << std::to_string(p->getAcceleration().y) << ", " << std::to_string(p->getAcceleration().z) << "\n";
        for (auto& p2 : particles)
        {
            p->checkCollisionTerrain(terrain, dt);
            if (p != p2)
            {
                p->checkCollision(p2, dt);
            }
        }
        
        p->updateConstrain(dt);
        p->computeForces();
        p->integrate(dt, _integration_method);
        p->clearAccum();
    }
}

void PhysicsEngine::updateRigidBody(float dt, const std::vector<class RigidBody*>& particles, Terrain* terrain)
{
    for (auto& p : particles)
    {
        //std::cout << std::to_string(p->getAcceleration().x) << ", " << std::to_string(p->getAcceleration().y) << ", " << std::to_string(p->getAcceleration().z) << "\n";
        for (auto& p2 : particles)
        {
            p->checkCollisionTerrain(terrain, dt);
            if (p != p2)
            {
                p->checkCollision(p2, dt);
            }
        }

        p->updateConstrain(dt);
        p->computeForces();
        p->integrate(dt, _integration_method);
        p->clearAccum();
    }
}
