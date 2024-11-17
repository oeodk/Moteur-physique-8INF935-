#include "FireballParticle.h"

FireballParticle::FireballParticle(const Vector3D& init_pos, const Vector3D& init_dir)
	: Particle(init_pos, 150 * init_dir, 1, 15, Vector3D(240, 60, 0), 175) {
	_friction_k1 * 0.5;
	_friction_k2 * 0.5;
}
void FireballParticle::drawNoLight()
{
	Particle::draw();
	Particle::drawNoLight();
}
;
