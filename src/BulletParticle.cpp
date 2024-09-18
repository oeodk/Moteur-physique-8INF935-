#include "BulletParticle.h"

BulletParticle::BulletParticle(Vector3D init_pos, Vector3D init_acc) : Particle(init_pos, 1000, init_acc, 0.1, Vector3D(80), 255) {
};