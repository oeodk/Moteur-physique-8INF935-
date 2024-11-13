#include "RigidBody.h"
#include "Terrain.h"

void RigidBody::addForce(const Vector3D& force)
{
	_accum_force += force;
}

void RigidBody::clearAccum()
{
	_accum_force = 0;
}


void RigidBody::computeForces()
{
	_acceleration = _accum_force * _inverse_mass;
}

void RigidBody::integrate(float dt, IntegrationMethods method)
{
	switch (method)
	{
	case RigidBody::VERLET:
		integrateVerlet(dt); break;
	default:
		integrateEuler(dt); break;
	}
}

Vector3D RigidBody::eulerUpdateVelocity(float dt)
{
	//Vector3D v =  std::pow(_drag, dt) * _velocity + dt * _acceleration + _velocity_increment_delay;
	Vector3D v = _velocity + dt * _acceleration + _velocity_increment_delay;
	if (v.squareNorm() != 0)
	{
		if (_velocity.squareNorm() == 0)
		{
			if (v.squareNorm() < _mu_s * _mu_s)
			{
				return Vector3D(0);
			}
		}
	}
	_velocity_increment_delay.set(0);
	return v;
}

Vector3D RigidBody::eulerUpdatePosition(float dt)
{
	Vector3D v_dir(_velocity);
	if (v_dir.squareNorm() != 0)
	{
		v_dir.normalize();
		if (Vector3D::dotProduct(G_ACC * dt, v_dir) > Vector3D::dotProduct(_velocity, v_dir))
		{
			return _position;
		}
	}
	return _position + dt * _velocity;
}

void RigidBody::updateConstrain(float dt)
{
	for (auto& constrain : _constrains)
	{
		constrain->updateConstrain(this, dt);
	}
}

void RigidBody::removeConstrains(std::shared_ptr<Constrain> constrain)
{
	for (size_t i = 0; i < _constrains.size(); i++)
	{
		if (_constrains[i] == constrain)
		{
			_constrains.erase(_constrains.begin() + i);
			return;
		}
	}
}

void RigidBody::integrateEuler(float dt)
{
	_velocity.set(eulerUpdateVelocity(dt));
	_position.set(eulerUpdatePosition(dt));
}

void RigidBody::integrateVerlet(float dt)
{
	// can be done because particle are deleted below 80
	if (_previous_position != constants::EMPTY_VECTOR3D)
	{
		Vector3D copy_previous_position = _previous_position;
		_previous_position = _position;
		_position = 2 * _position - copy_previous_position + dt * dt * _acceleration;
	}
	else
	{
		_previous_position = _position;
		integrateEuler(dt);
	}
}

void RigidBody::checkCollision(RigidBody* other_particle, float dt)
{
	const Vector3D v_relative(_velocity - other_particle->_velocity);
	const float distance = (other_particle->_position - _position).squareNorm();
	Vector3D contact_normal = (other_particle->_position - _position);
	if (contact_normal.squareNorm() == 0)
	{
		return;
	}
	contact_normal.normalize();
	const float SUM_OF_RADIUS = _radius + other_particle->_radius;
	if (distance <= SUM_OF_RADIUS * SUM_OF_RADIUS)
	{
		const float chevauchement = SUM_OF_RADIUS - sqrt(distance);
		solveCollision(other_particle, v_relative, chevauchement, contact_normal);
	}
}

void RigidBody::solveCollision(RigidBody* other_particle, const Vector3D& v_relative, float chevauchement, const Vector3D& contact_normal)
{
	float K;
	const float e = 0.5; //elasticity
	K = ((e + 1) * Vector3D::dotProduct(contact_normal, v_relative)) / (_inverse_mass + other_particle->_inverse_mass);
	const float dep_1 = (chevauchement * other_particle->_mass) / (_mass + other_particle->_mass);
	const float dep_2 = (chevauchement * _mass) / (_mass + other_particle->_mass);
	_position = _position - dep_1 * contact_normal;
	other_particle->_position = other_particle->_position + dep_2 * contact_normal;

	incrementVelocityWithDelay(contact_normal * -K * _inverse_mass);
	other_particle->incrementVelocityWithDelay(contact_normal * K * other_particle->_inverse_mass);

}

void RigidBody::checkCollisionTerrain(Terrain* terrain, float dt)
{
	const float terrain_height = terrain->getHeight(_position.x, _position.z);
	if (terrain_height > _position.y - _radius)
	{
		solveCollisionTerrain(_velocity, terrain_height - (_position.y - _radius), terrain->getNormal(_position.x, _position.z));
	}
}

void RigidBody::solveCollisionTerrain(const Vector3D& v_relative, float chevauchement, const Vector3D& contact_normal)
{
	float K;
	const float e = 0.5; //elasticity
	K = ((e + 1) * Vector3D::dotProduct(contact_normal, v_relative)) / _inverse_mass;
	_position = _position + chevauchement * contact_normal;
	incrementVelocityWithDelay(contact_normal * -K * _inverse_mass);
}