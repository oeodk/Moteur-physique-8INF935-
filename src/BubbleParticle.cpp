#include "BubbleParticle.h"

BubbleParticle::BubbleParticle(Vector3D init_pos, Vector3D init_dir, Vector3D init_acc) 
	: Particle(init_pos, 50 * init_dir, init_acc, 0.01, 25, Vector3D(30,255,255), 25) {
};