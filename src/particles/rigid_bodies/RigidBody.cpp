#include "RigidBody.h"
#include "ofGraphics.h"
#include "of3DGraphics.h"
#include "glm/mat3x3.hpp"
void RigidBody::initAngularAcceleration(const Vector3D& base_force_for_rotation, const Vector3D& application_point)
{
	getInertiaMoment(_inertia_moment_inverted);
	_inertia_moment_inverted = _inertia_moment_inverted.getInverse();
	addForceToPoint(base_force_for_rotation, application_point);
}

RigidBody::RigidBody(const Vector3D& init_pos, const Vector3D& init_vel, float mass, float radius, const Quaternion& init_orientation, const Vector3D& init_angular_vel)
	: Particle(init_pos, init_vel * 200, mass, radius, Vector3D(255, 255, 255), 255)
	, _angular_velocity(init_angular_vel), _orientation(init_orientation)
{
	_mass_center_mesh.addVertex(_position);
	_mass_center_mesh.disableColors();
	_mass_center_mesh.disableIndices();
	_mass_center_mesh.disableNormals();
	_mass_center_mesh.disableTextures();
	_mass_center_mesh.setMode(OF_PRIMITIVE_POINTS);
}

void RigidBody::integrate(float dt, IntegrationMethods method)
{
	Particle::integrate(dt, method);

	glm::mat3x3 tmp_mat;
	tmp_mat = glm::inverse(glm::identity<glm::mat3x3>() * (0.4 * _mass * _radius * _radius));
	
	_angular_acceleration = tmp_mat * _torque;
	_torque.set(0, 0, 0);

	_angular_velocity = _angular_velocity + _angular_acceleration * dt;
	glm::quat tmp(_orientation);

	//_orientation += 0.5f * Quaternion(0,_angular_velocity) * _orientation * dt;
	// TO DO : uncomment the line above and remove the two line bellow
	tmp += 0.5f * glm::quat(0,_angular_velocity) * tmp * dt;
	_orientation = Quaternion(tmp.w, tmp.x, tmp.y, tmp.z);

	_orientation.normalize();
}

void RigidBody::draw()
{
	_transformation.setPosition(_position);

	_transformation.setOrientation(_orientation);

	_transformation.transformGL();
	
	ofSetColor(_color.x, _color.y, _color.z, _alpha);
	_mesh->draw();

	_transformation.restoreTransformGL();

	_mass_center_mesh.getVertices().back() = _position;
	ofDisableDepthTest();
	ofSetColor(255, 0, 0, 255);
	_mass_center_mesh.draw();
	ofEnableDepthTest();
}

void RigidBody::addForceToPoint(const Vector3D& base_force_for_rotation, const Vector3D& application_point)
{
	_torque += Vector3D::crossProduct(application_point, base_force_for_rotation);
	std::cout << "x : " << _torque.x << "y : " << _torque.y << "z : " << _torque.z << std::endl;
}
