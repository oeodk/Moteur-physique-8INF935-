#include "Blob.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>

#include "forces/SpringParticleForce.h"

Blob::Blob(std::vector<Particle*>* world_particles, ParticleForceRegistry* force_registry, const Vector3D& init_pos, const Vector3D& init_vel)
	:Particle(init_pos, init_vel, 10, 10, Vector3D(255, 255, 255), 255),
	_gravity_nullification(Vector3D(G_ACC * -1)),
	_world_particles(world_particles), _world_force_registry(force_registry)
{
	_joins.setMode(OF_PRIMITIVE_LINES);
}

void Blob::updateBlob()
{
	constexpr float SPRING_K = 1;
	constexpr float SPRING_LENGTH = 200;
	for (Particle* particle : *_world_particles)
	{
		Vector3D dist(_position - particle->getParticlePosition());
		if(particle != this)
		{
			bool added = false;

			ParticlePair p(this, particle);
			if (std::find_if(_particles_links.begin(), _particles_links.end(), [&p](const ParticlePair& x) { return p.p1 == x.p1 && p.p2 == x.p2; }) == _particles_links.end())
			{
				if (dist.squareNorm() < _GRAB_RANGE * _GRAB_RANGE)
				{
					added = true;
					_blob_particles.push_back(particle);
					//_forces.emplace(particle, std::vector<GeneratorSource>());
					if(_forces[particle].size() < 4)
					{
						_forces[particle].emplace_back();
						_forces[particle].back().generator = new SpringParticleForce(SPRING_K, SPRING_LENGTH, &_position);
						_forces[particle].back().source = this;
						_particles_links.emplace_back(this, particle);
					}
				}
			}
			const size_t particles_count = _blob_particles.size();
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
								//_forces.emplace(particle, std::vector<GeneratorSource>());
							}
							_forces[particle].emplace_back();
							_forces[particle].back().generator = new SpringParticleForce(SPRING_K, SPRING_LENGTH, _blob_particles[i]->getPosition());
							_forces[particle].back().source = _blob_particles[i];

							_forces[_blob_particles[i]].emplace_back();
							_forces[_blob_particles[i]].back().generator = new SpringParticleForce(SPRING_K, SPRING_LENGTH, particle->getPosition());
							_forces[_blob_particles[i]].back().source = particle;

							_particles_links.emplace_back(_blob_particles[i], particle);
						}
					}
				}

			}

		}
	}

	_world_force_registry->add(this, &_gravity_nullification);
	for (size_t j = 0; j < _particles_links.size(); j++)
	{
		Vector3D dist(_particles_links[j].p2->getParticlePosition() - _particles_links[j].p1->getParticlePosition());
		if (dist.squareNorm() > _LOSE_RANGE * _LOSE_RANGE)
		{
			std::vector<Particle*> particles;
			std::vector<Particle*> particles_link;

			particles.reserve(2);
			particles_link.reserve(2);

			particles.push_back(_particles_links[j].p2);
			particles_link.push_back(_particles_links[j].p1);

			if (_particles_links[j].p1 != this)
			{
				particles.push_back(_particles_links[j].p1);
				particles_link.push_back(_particles_links[j].p2);
			}

			for (size_t i = 0; i < particles.size(); i++)
			{
				for (size_t k = 0; k < _forces.at(particles[i]).size(); k++)
				{
					if (_forces.at(particles[i])[k].source == particles_link[i])
					{
						delete _forces.at(particles[i])[k].generator;
						_forces.at(particles[i]).erase(_forces.at(particles[i]).begin() + k);
						k--;
						break;
					}
				}
			}
			_particles_links.erase(_particles_links.begin() + j);
		}
	}
	for (size_t i = 0; i < _blob_particles.size(); i++)
	{
		for (auto& force : _forces.at(_blob_particles[i]))
		{
			_world_force_registry->add(_blob_particles[i], force.generator);
		}
	}
	_joins.clear();
	for (auto& pair : _particles_links)
	{
		_joins.addVertex(pair.p1->getParticlePosition());
		_joins.addVertex(pair.p2->getParticlePosition());
	}
}

void Blob::drawNoLight()
{
	ofSetColor(_color.x, _color.y, _color.z, 255);
	_joins.draw();
}
