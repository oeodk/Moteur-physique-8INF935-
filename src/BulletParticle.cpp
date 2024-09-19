#include "BulletParticle.h"

BulletParticle::BulletParticle(Vector3D init_pos,Vector3D init_dir, Vector3D init_acc) : 
	Particle(init_pos, 100 * init_dir, init_acc, 0.1, 25, Vector3D(80), 255) {
};