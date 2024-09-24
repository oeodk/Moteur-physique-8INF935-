#include "FireballParticle.h"

FireballParticle::FireballParticle(Vector3D init_pos, Vector3D init_dir, Vector3D init_acc) 
	: Particle(init_pos, 150 * init_dir, init_acc, 500, 35, Vector3D(240,60,0), 255) {
};
