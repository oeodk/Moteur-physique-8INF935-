#include "Particle.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>
#include <stdexcept>
#include "../Terrain.h"

Particle::Particle() :_position(), _previous_position(), _velocity(), _acceleration(0, 0, 0), _inverse_mass(0), _mass(FLT_MAX), _radius(), _color(), _alpha(), _time_counter(0) {
	_world_position = &_position;
	_trail.setMode(OF_PRIMITIVE_POINTS);
	_trail.addVertex(_position);
	_trail.addVertex(_position);
}

Particle::Particle(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius, const Vector3D& color, float alpha)
	: _position(init_pos), _velocity(init_vel), _acceleration(0), _accum_force(0), _radius(radius), _color(color), _alpha(alpha), _time_counter(0) {
	_mass = mass;
	_inverse_mass = mass == 0 ? FLT_MAX : 1 / mass;
	_world_position = &_position;
	_trail.setMode(OF_PRIMITIVE_LINE_STRIP);
	_trail.addVertex(_position);
	_friction_k1 = 0.1;
	_friction_k2 = 0.0001;
	_mu_s = 0.15;

	if (!mesh_init_)
	{
		mesh_init_ = true;
		_mesh.load("test.ply");
		_mesh.clearTexCoords();
		_mesh.clearColors();
	}
	//_shoot_sound.load("chicken.wav");
	//_shoot_sound.setSpeed(pow(1 / (_radius), 0.25));
	//_shoot_sound.play();
}

void Particle::addForce(const Vector3D& force) {
	_accum_force += force;
}

void Particle::clearAccum() {
	_accum_force = 0;
}


void Particle::computeForces() {
	_acceleration = _accum_force * _inverse_mass;
}

void Particle::integrate(float dt, IntegrationMethods method) {
	switch (method) {
	case Particle::VERLET:
		integrateVerlet(dt); break;
	default:
		integrateEuler(dt); break;
	}


	// Generate the trail effect
	_time_counter += dt;
	if (_time_counter > _TRAIL_POINT_DELAY) {
		_trail.addVertex(_position);
		_time_counter -= _TRAIL_POINT_DELAY;
	}
	_trail.getVertices().back() = _position;
}

Vector3D Particle::eulerUpdateVelocity(float dt) {
	//Vector3D v =  std::pow(_drag, dt) * _velocity + dt * _acceleration + _velocity_increment_delay;
	Vector3D v =  _velocity + dt * _acceleration + _velocity_increment_delay;
	if(v.squareNorm() != 0)
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

Vector3D Particle::eulerUpdatePosition(float dt) {
	Vector3D v_dir(_velocity);
	if(v_dir.squareNorm() != 0)
	{
		v_dir.normalize();
		if (Vector3D::dotProduct(G_ACC * dt, v_dir) > Vector3D::dotProduct(_velocity, v_dir))
		{
			return _position;
		}
	}
	return _position + dt * _velocity;
}

void Particle::updateConstrain(float dt)
{
	for (auto& constrain : _constrains)
	{
		constrain->updateConstrain(this, dt);
	}
}

void Particle::removeConstrains(std::shared_ptr<Constrain> constrain)
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

void Particle::integrateEuler(float dt) {
	_velocity.set(eulerUpdateVelocity(dt));
	_position.set(eulerUpdatePosition(dt));
}

void Particle::integrateVerlet(float dt) {
	// can be done because particle are deleted below 80
	if (_previous_position != constants::EMPTY_VECTOR3D) {
		Vector3D copy_previous_position = _previous_position;
		_previous_position = _position;
		_position = 2 * _position - copy_previous_position + dt * dt * _acceleration;
	}
	else {
		_previous_position = _position;
		integrateEuler(dt);
	}
}

void Particle::draw() {
	_transformation.setPosition(_position);
	_transformation.setScale(_radius);
	_transformation.lookAt(_position + _velocity);
	_transformation.transformGL();
	ofSetColor(_color.x, _color.y, _color.z, _alpha);
	_mesh.draw();
	_transformation.restoreTransformGL();

	//ofSetColor(255,255,255, 120);
	//ofDrawSphere(_position, _radius);
	//ofDrawSphere(_position, _radius);
}

void Particle::drawNoLight() {
	if(_draw_trail)
	{
		ofSetColor(_color.x, _color.y, _color.z, 255);
		_trail.draw();
	}
}

void Particle::checkCollision(Particle* other_particle, float dt) {
	const Vector3D v_relative(_velocity - other_particle->_velocity);
	const float distance = (other_particle->_position - _position).squareNorm();
	Vector3D contact_normal = (other_particle->_position - _position);
	if (contact_normal.squareNorm() == 0)
	{
		return;
	}
	contact_normal.normalize();
	const float SUM_OF_RADIUS = _radius + other_particle->_radius;
	if (distance <= SUM_OF_RADIUS * SUM_OF_RADIUS){
		const float chevauchement = SUM_OF_RADIUS - sqrt(distance);
		solveCollision(other_particle, v_relative, chevauchement, contact_normal);
	}
}

void Particle::solveCollision(Particle* other_particle, const Vector3D& v_relative, float chevauchement, const Vector3D& contact_normal)
{
	float K;
	const float e = 0.5; //elasticity
	K = ((e + 1) * Vector3D::dotProduct(contact_normal, v_relative)) / (_inverse_mass + other_particle->_inverse_mass);
	const float dep_1 = (chevauchement * other_particle->_mass) / (_mass + other_particle->_mass);
	const float dep_2 = (chevauchement * _mass) / (_mass + other_particle->_mass);
	_position = _position - dep_1 * contact_normal;
	other_particle->_position = other_particle->_position + dep_2 * contact_normal;

	incrementVelocityWithDelay( contact_normal * -K * _inverse_mass);
	other_particle->incrementVelocityWithDelay( contact_normal * K * other_particle->_inverse_mass);

}

void Particle::checkCollisionTerrain(Terrain* terrain, float dt)
{
	const float terrain_height = terrain->getHeight(_position.x, _position.z);
	if (terrain_height > _position.y - _radius)
	{
		solveCollisionTerrain(_velocity, terrain_height - (_position.y - _radius), terrain->getNormal(_position.x, _position.z));
	}
}

void Particle::solveCollisionTerrain(const Vector3D& v_relative, float chevauchement, const Vector3D& contact_normal)
{
	float K;
	const float e = 0.5; //elasticity
	K = ((e + 1) * Vector3D::dotProduct(contact_normal, v_relative)) / _inverse_mass;
	_position = _position + chevauchement * contact_normal;
	incrementVelocityWithDelay(contact_normal * -K * _inverse_mass);
}

void Particle::resetMovement()
{
	_acceleration.set(0);
	_velocity.set(0);
}


void Particle::testParticle() {
	Particle particle1;

	_ASSERT(particle1._position == Vector3D(0, 0, 0));
	_ASSERT(particle1._velocity == Vector3D(0, 0, 0));
	//_ASSERT(particle1._acceleration == Vector3D(0, -g, 0));
	_ASSERT(particle1._inverse_mass == 0);

	Vector3D init_position(1, 2, 3);
	Vector3D init_velocity(0, 0, 0);
	Vector3D init_acceleration(0, -g, 0);
	Particle particle2(init_position, init_velocity, 1., 1., Vector3D(1), 1.);
	_ASSERT(particle2._position == init_position);
	_ASSERT(particle2._velocity == init_velocity);
	//_ASSERT(particle2._acceleration == 0);
	_ASSERT(particle2._inverse_mass == 1.0f);

	particle2.integrate(1.0f, Particle::EULER);
	_ASSERT(particle2._velocity == Vector3D(0, 0, 0));
	_ASSERT(particle2._position == Vector3D(1, 2, 3));

	Particle particle4(Vector3D(1.0f, 1.0f, 1.0f), Vector3D(0.0f, 0.0f, 0.0f), 1., 1., Vector3D(1), 1.);

	// 1 st integration used to initialized p-1 with euler integration
	particle4.integrate(1.0f, Particle::VERLET);
	particle4.integrate(1.0f, Particle::VERLET);
	_ASSERT(particle4._position == Vector3D(1, 1, 1));
}
