#include "BulletParticle.h"

BulletParticle::BulletParticle(const Vector3D& init_pos, const Vector3D& init_dir, const Vector3D& init_acc) :
	Particle(init_pos, 500 * init_dir, 0.1, 4, Vector3D(204, 153, 0), 255) {
};