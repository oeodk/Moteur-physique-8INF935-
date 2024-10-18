#include "CannonballParticle.h"

CannonballParticle::CannonballParticle(const Vector3D& init_pos, const Vector3D& init_dir)
	: Particle(init_pos, 30 * init_dir, 10, 20, Vector3D(60), 255) {
};