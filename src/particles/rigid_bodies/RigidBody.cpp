#include "RigidBody.h"

#include "glm/mat3x3.hpp"
void RigidBody::initAngularAcceleration(const Vector3D& base_force_for_rotation, const Vector3D& application_point)
{
	getInertiaMoment(_inertia_moment);
	displaceInertiaMoment(_inertia_moment, getCentroid());
	_inertia_moment_inverted = _inertia_moment.getInverse();
	addForceToPoint(base_force_for_rotation, application_point);
}

void RigidBody::displaceInertiaMoment(Matrix3& inertia_moment, const Vector3D& displacement)
{
	Matrix3 dep(
		Vector3D( displacement.y * displacement.y + displacement.z * displacement.z, -displacement.x * displacement.y                                 , -displacement.x * displacement.z),
		Vector3D(-displacement.x * displacement.y                                  , displacement.x * displacement.x + displacement.z * displacement.z, -displacement.y * displacement.z),
		Vector3D(-displacement.x * displacement.z                                  , -displacement.y * displacement.z                                 , displacement.x * displacement.x + displacement.y * displacement.y)
	);
	inertia_moment = inertia_moment + (_mass * dep);
}

Vector3D RigidBody::calculateCentroid(const ofVboMesh& mesh)
{
	if (mesh.getVertices().size() == 0)
	{
		ofLogWarning("ofMesh") << "getCentroid(): mesh has no vertices, returning glm::vec3(0, 0, 0)";
		return glm::vec3(0, 0, 0);
	}

	Vector3D sum;
	for (ofIndexType i = 0; i < mesh.getVertices().size(); i++)
	{
		sum += mesh.getVertices()[i];
	}
	sum /= mesh.getVertices().size();
	return sum;
}

RigidBody::RigidBody(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius, const Quaternion& init_orientation, const Vector3D& init_angular_vel, const std::string& sound_path)
	: Particle(init_pos, init_vel * 100, mass, radius, Vector3D(255, 255, 255), 255)
	, _angular_velocity(init_angular_vel * 0), _orientation(init_orientation)
{
	_mass_center_mesh.addVertex(_position);
	_mass_center_mesh.disableColors();
	_mass_center_mesh.disableIndices();
	_mass_center_mesh.disableNormals();
	_mass_center_mesh.disableTextures();
	_mass_center_mesh.setMode(OF_PRIMITIVE_POINTS);

	_sound.load("sound/" + sound_path);
	_sound.play();

}

void RigidBody::checkCollision(RigidBody* other_rigid_body, float dt)
{
	std::array<CollisionData, 8> collision_data;
	std::vector<CollisionData*> relevant_collisions_data;
	relevant_collisions_data.reserve(4);
	for(HitBoxPrimitive* hitbox : _hitbox)
	{
		for(HitBoxPrimitive* other_hitbox : other_rigid_body->_hitbox)
		if (hitbox->doCollideWith(other_hitbox, collision_data))
		{
			// sorting collision data to get lower penetration distance in the front
			std::sort(collision_data.begin(), collision_data.end(), [](const CollisionData& c1, const CollisionData& c2) {
				return c1._penetration_distance < c2._penetration_distance;
				});

			const float min_penetration = collision_data[0]._penetration_distance;

			size_t i = 0;
			while (i < 4 && min_penetration == collision_data[i]._penetration_distance)
			{
				relevant_collisions_data.push_back(&collision_data[i]);
				i++;
			}
			solveCollision(other_rigid_body, relevant_collisions_data);
			return;
		}
	}
}

void RigidBody::solveCollision(RigidBody* other_rigid_body, const std::vector<CollisionData*>& collisions_data)
{
	float avr_penetration = 0;
	for (size_t i = 0; i < std::size(collisions_data); i++)
	{
		const Vector3D& contact_normal = collisions_data[i]->_contact_normal;
		const float penetration = collisions_data[i]->_penetration_distance;
		const Vector3D& impact_point = collisions_data[i]->_impact_point;

		avr_penetration += (penetration / std::size(collisions_data));

		solveCollisionTranslation(contact_normal, _velocity - other_rigid_body->_velocity, other_rigid_body, std::size(collisions_data));
		solveCollisionRotation(contact_normal, other_rigid_body, impact_point, std::size(collisions_data));
	}
	Vector3D dep_dir = (other_rigid_body->_position - _position);
	if (dep_dir.squareNorm() == 0)
	{
		return;
	}
	dep_dir.normalize();

	const float dep_1 = (avr_penetration * other_rigid_body->_mass) / (_mass + other_rigid_body->_mass);
	const float dep_2 = (avr_penetration * _mass) / (_mass + other_rigid_body->_mass);

	_position = _position - dep_1 * dep_dir;
	other_rigid_body->_position = other_rigid_body->_position + dep_2 * dep_dir;
}

void RigidBody::checkCollisionTerrain(Terrain* terrain, float dt)
{
	std::array<CollisionData, 8> collision_data;
	std::vector<CollisionData*> relevant_collisions_data;
	relevant_collisions_data.reserve(4);
	for(HitBoxPrimitive*  hitbox : _hitbox)
	{
		if (hitbox->doCollideWithTerrain(terrain, collision_data))
		{
			// sorting collision data to get lower penetration distance in the front
			std::sort(collision_data.begin(), collision_data.end(), [](const CollisionData& c1, const CollisionData& c2) {
				return c1._penetration_distance < c2._penetration_distance;
				});

			const float min_penetration = collision_data[0]._penetration_distance;

			size_t i = 0;
			while (i < 4 && min_penetration > collision_data[i]._penetration_distance * 0.95 && min_penetration < collision_data[i]._penetration_distance * 1.05)
			{
				relevant_collisions_data.push_back(&collision_data[i]);
				i++;
			}
			solveCollisionTerrain(terrain, relevant_collisions_data);
			return;
		}
	}
}

void RigidBody::solveCollisionTerrain(Terrain* terrain, const std::vector<CollisionData*>& collisions_data)
{
	float avr_penetration = 0;
	Vector3D avr_normal;
	for (size_t i = 0; i < std::size(collisions_data); i++)
	{
		const Vector3D& contact_normal = collisions_data[i]->_contact_normal;
		const float penetration = collisions_data[i]->_penetration_distance;
		const Vector3D& impact_point = collisions_data[i]->_impact_point;

		avr_normal+= (contact_normal / std::size(collisions_data));
		avr_penetration += (penetration / std::size(collisions_data));
		
		float K;
		const float e_translation = 0.05; //elasticity
		const float e_rotation = 0.5; //elasticity
		// solving translation
		K = ((e_translation + 1) * Vector3D::dotProduct(contact_normal, _velocity)) / _inverse_mass;
		incrementVelocityWithDelay(contact_normal * -K * _inverse_mass);

		// solving rotation
		
		// Relative position from center of mass to collision point
		Vector3D c1 = impact_point - _position;

		// Relative velocity at the collision point
		glm::vec3 relative_velocity = -1 * (_velocity + Vector3D::crossProduct(_angular_velocity, c1));

		// Calculate impulse scalar
		float relative_velocity_projection = Vector3D::dotProduct(relative_velocity, contact_normal);
		//if (relativeVelocityAlongNormal > 0.0f) return; // Bodies are separating

		const Matrix3& J1_inv = _inertia_moment_inverted;

		// Compute impulse denominator
		const Vector3D c1_cross_normal = Vector3D::crossProduct(c1, contact_normal);

		float angular_term = Vector3D::dotProduct(contact_normal, Vector3D::crossProduct(J1_inv * c1_cross_normal, c1));

		K = -(1.0f + e_rotation) * relative_velocity_projection / (_inverse_mass + angular_term);
		K /= std::size(collisions_data);

		// Impulse vector
		Vector3D impulse = K * contact_normal;

		// Apply angular impulse
		incrementAngularVelocityWithDelay((-1) * J1_inv * Vector3D::crossProduct(c1, impulse));
		
	}
	_position = _position + avr_penetration * avr_normal;
}

void RigidBody::solveCollisionTranslation(const Vector3D& contact_normal, const Vector3D& v_relative, RigidBody* other_rigid_body, int impact_count)
{
	float K;
	constexpr float e = 0.75; //elasticity

	K = ((e + 1) * Vector3D::dotProduct(contact_normal, v_relative)) / (_inverse_mass + other_rigid_body->_inverse_mass);
	K /= impact_count;

	incrementVelocityWithDelay(contact_normal * -K * _inverse_mass);
	other_rigid_body->incrementVelocityWithDelay(contact_normal * K * other_rigid_body->_inverse_mass);
}

void RigidBody::solveCollisionRotation(const Vector3D& contact_normal, RigidBody* other_rigid_body, const Vector3D& contact_point, int impact_count)
{
	constexpr float e = 0.5; //elasticity

	// Relative position from center of mass to collision point
	Vector3D c1 = contact_point - _position;
	Vector3D c2 = contact_point - other_rigid_body->_position;

	// Relative velocity at the collision point
	glm::vec3 relative_velocity = (other_rigid_body->_velocity + Vector3D::crossProduct(other_rigid_body->_angular_velocity, c2)) -
		(_velocity + Vector3D::crossProduct(_angular_velocity, c1));

	// Calculate impulse scalar
	float relative_velocity_projection = Vector3D::dotProduct(relative_velocity, contact_normal);
	//if (relativeVelocityAlongNormal > 0.0f) return; // Bodies are separating

	const Matrix3& J1_inv = _inertia_moment_inverted;
	const Matrix3& J2_inv = other_rigid_body->_inertia_moment_inverted;

	// Compute impulse denominator
	const float mass_term = (_inverse_mass) +other_rigid_body->_inverse_mass;
	const Vector3D c1_cross_normal = Vector3D::crossProduct(c1, contact_normal);
	const Vector3D c2_cross_normal = Vector3D::crossProduct(c2, contact_normal);

	float angular_term = Vector3D::dotProduct(contact_normal,
		Vector3D::crossProduct(J1_inv * c1_cross_normal, c1) + Vector3D::crossProduct(J2_inv * c2_cross_normal, c2));

	float K = -(1.0f + e) * relative_velocity_projection / (mass_term + angular_term);
	K /= impact_count;

	// Impulse vector
	Vector3D impulse = K * contact_normal;

	// Apply angular impulse
	incrementAngularVelocityWithDelay((-1) * J1_inv * Vector3D::crossProduct(c1, impulse));
	other_rigid_body->incrementAngularVelocityWithDelay(J2_inv * Vector3D::crossProduct(c2, impulse));
}

void RigidBody::integrate(float dt, IntegrationMethods method)
{
	Particle::integrate(dt, method);
	
	Matrix3 orientation_matrix(_orientation.toMatrix3());
	Matrix3 inv_orientation_matrix(orientation_matrix.getInverse());
	

	_inertia_moment_inverted = orientation_matrix * (_inertia_moment_inverted * inv_orientation_matrix);
	_inertia_moment = _inertia_moment_inverted.getInverse();

	_angular_acceleration = _inertia_moment_inverted * _torque;
	_torque.set(0, 0, 0);

	_angular_velocity = _angular_velocity + _angular_acceleration * dt + _angular_velocity_increment_delay;
	_angular_velocity_increment_delay.set(0);

	_orientation += 0.5f * Quaternion(0,_angular_velocity) * _orientation * dt;

	_orientation.normalize();
	for (HitBoxPrimitive* hitbox : _hitbox)
	{
		hitbox->update();
	}
}

void RigidBody::draw()
{
	_transformation.setPosition(_position);

	_transformation.setOrientation(_orientation);

	_transformation.transformGL();

	
	ofSetColor(_color.x, _color.y, _color.z, _alpha);
	if (_texture.isAllocated())
	{
		_texture.bind();
	}
	_mesh->draw();

	if (_texture.isAllocated())
	{
		_texture.unbind();
	}

#ifdef HITBOX_DEBUG
	for (HitBoxPrimitive* hitbox : _hitbox)
	{
		hitbox->drawBox();
	}
#endif // HITBOX_DEBUG

#ifdef DEBUG_RIGID_BODY
	drawApproximation();
#endif // DEBUG_RIGID_BODY
	_transformation.restoreTransformGL();

#ifdef HITBOX_DEBUG
	for (HitBoxPrimitive* hitbox : _hitbox)
	{
		hitbox->drawCorner();
	}
#endif // HITBOX_DEBUG
	for (HitBoxPrimitive* hitbox : _hitbox)
	{
		hitbox->update();
	}

	_mass_center_mesh.getVertices().back() = _position;
	ofDisableDepthTest();
	ofSetColor(255, 0, 0, 255);
	_mass_center_mesh.draw();
	ofEnableDepthTest();
}

void RigidBody::addForceToPoint(const Vector3D& base_force_for_rotation, const Vector3D& application_point)
{
	_torque += Vector3D::crossProduct(application_point, base_force_for_rotation);
}
