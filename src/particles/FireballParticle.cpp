#include "FireballParticle.h"

FireballParticle::FireballParticle(const Vector3D& init_pos, const Vector3D& init_dir)
	: Particle(init_pos, 150 * init_dir, 1, 15, Vector3D(240, 60, 0), 175) {
};
