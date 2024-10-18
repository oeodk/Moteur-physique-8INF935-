#include "BubbleParticle.h"

BubbleParticle::BubbleParticle(const Vector3D& init_pos, const Vector3D& init_dir)
	: Particle(init_pos, 50 * init_dir, 0.001, 8, Vector3D(30, 255, 255), 40) {
};