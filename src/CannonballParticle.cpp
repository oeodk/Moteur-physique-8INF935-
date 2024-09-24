#include "CannonballParticle.h"

CannonballParticle::CannonballParticle(const Vector3D& init_pos, const Vector3D& init_dir, const Vector3D& init_acc)
	: Particle(init_pos, 30 * init_dir, 10 * init_acc, 2000, 20, Vector3D(60), 255) {
};