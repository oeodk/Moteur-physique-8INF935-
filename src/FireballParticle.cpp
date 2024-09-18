#include "FireballParticle.h"

FireballParticle::FireballParticle(Vector3D init_pos, Vector3D init_dir, Vector3D init_acc) 
	: Particle(init_pos, 500 * init_dir, init_acc, 10, 100, Vector3D(240,40,0), 255) {
};
