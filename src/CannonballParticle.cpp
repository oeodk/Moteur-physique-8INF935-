#include "CannonballParticle.h"

CannonballParticle::CannonballParticle(Vector3D init_pos, Vector3D init_dir, Vector3D init_acc) 
	: Particle(init_pos, 30 * init_dir, init_acc, 2000, 65, Vector3D(30), 255) {
};