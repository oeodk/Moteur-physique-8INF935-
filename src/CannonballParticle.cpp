#include "CannonballParticle.h"

CannonballParticle::CannonballParticle(Vector3D init_pos, Vector3D init_dir, Vector3D init_acc) 
	: Particle(init_pos, 250 * init_dir, init_acc, 5, 200, Vector3D(0), 255) {
};