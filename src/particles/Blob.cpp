#include "Blob.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>
#include "constrain/CableConstrain.h"
#include "forces/SpringParticleForce.h"

Blob::Blob(std::vector<Particle*>* world_particles, ParticleForceRegistry* force_registry, const Vector3D& init_pos, const Vector3D& init_vel)
	:Particle(init_pos, init_vel, 1, 10, Vector3D(255, 255, 255), 255),
	_gravity_nullification(Vector3D(G_ACC * -1)),
	_world_particles(world_particles), _world_force_registry(force_registry)
{
	_joins.setMode(OF_PRIMITIVE_LINES);
	_friction_k1 = 0;
	_friction_k2 = 0;
}

void Blob::updateBlob()
{
	constexpr float SPRING_K = 0.5;
	constexpr float SPRING_LENGTH = _GRAB_RANGE;
	for (Particle* particle : *_world_particles)
	{
		Vector3D dist(_position - particle->getParticlePosition());
		if(particle != this)
		{
			bool added = false;

			ParticlePair p(this, particle);
			// Add particle to the blob if the particle is close enough
			// The particle is linked to the blob with a spring and the blob is not really linked to the particle
			if (std::find_if(_particles_links.begin(), _particles_links.end(), [&p](const ParticlePair& x) { return p.p1 == x.p1 && p.p2 == x.p2; }) == _particles_links.end())
			{
				if (dist.squareNorm() < _GRAB_RANGE * _GRAB_RANGE)
				{
					added = true;
					_blob_particles.push_back(particle);

					_forces[particle].emplace_back();
					_forces[particle].back().generator = new SpringParticleForce(SPRING_K, SPRING_LENGTH, &_position);
					_forces[particle].back().source = this;
					_particles_links.emplace_back(this, particle);
					particle->addConstrain(std::make_shared<CableConstrain>(_CABLE_RANGE, this));
				}
			}
			const size_t particles_count = _blob_particles.size();
			// Add particle to the blob if the particle is close of another blob's particle 
			// Both particle are linked with a spring
			for (size_t i = 0; i < particles_count; i++)
			{
				ParticlePair p(_blob_particles[i], particle);
				if(std::find_if(_particles_links.begin(), _particles_links.end(), [&p](const ParticlePair& x) { return p.p1 == x.p1 && p.p2 == x.p2; }) == _particles_links.end())
				{
					Vector3D d(_blob_particles[i]->getParticlePosition() - particle->getParticlePosition());
					if (d.squareNorm() < _GRAB_RANGE * _GRAB_RANGE)
					{
						if (_forces[particle].size() < 4)
						{
							if (!added)
							{
								_blob_particles.push_back(particle);
								added = true;
							}
							_forces[particle].emplace_back();
							_forces[particle].back().generator = new SpringParticleForce(SPRING_K, SPRING_LENGTH, _blob_particles[i]->getPosition());
							_forces[particle].back().source = _blob_particles[i];
							_blob_particles[i]->addConstrain(std::make_shared<CableConstrain>(_CABLE_RANGE, particle));

							_forces[_blob_particles[i]].emplace_back();
							_forces[_blob_particles[i]].back().generator = new SpringParticleForce(SPRING_K, SPRING_LENGTH, particle->getPosition());
							_forces[_blob_particles[i]].back().source = particle;

							particle->addConstrain(std::make_shared<CableConstrain>(_CABLE_RANGE, _blob_particles[i]));
							_particles_links.emplace_back(_blob_particles[i], particle);
						}
					}
				}

			}

		}
	}

	for (size_t i = 0; i < _blob_particles.size(); i++)
	{
		for (auto& force : _forces.at(_blob_particles[i]))
		{
			_world_force_registry->add(_blob_particles[i], force.generator);
		}
	}
	// Updating link render
	_joins.clear();
	for (auto& pair : _particles_links)
	{
		_joins.addVertex(pair.p1->getParticlePosition());
		_joins.addVertex(pair.p2->getParticlePosition());
	}
}

void Blob::drawNoLight()
{
	Particle::drawNoLight();
	ofSetColor(_color.x, _color.y, _color.z, 255);
	_joins.draw();
}

// SPlitting the blob remove the links between the blob core and the rest of the particles
void Blob::split()
{
	for (size_t i = 0; i < _particles_links.size(); i++)
	{
		if (_particles_links[i].p1 == this)
		{
			for (size_t k = 0; k < _forces.at(_particles_links[i].p2).size(); k++)
			{
				if (_forces.at(_particles_links[i].p2)[k].source == _particles_links[i].p1)
				{
					delete _forces.at(_particles_links[i].p2)[k].generator;
					_forces.at(_particles_links[i].p2).erase(_forces.at(_particles_links[i].p2).begin() + k);
					k--;
					break;
				}
			}
			_particles_links.erase(_particles_links.begin() + i);
		}
	}
}
