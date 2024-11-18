#include "RigidBody.h"

#include "glm/mat3x3.hpp"
void RigidBody::initAngularAcceleration(const Vector3D& base_force_for_rotation, const Vector3D& application_point)
{
	getInertiaMoment(_inertia_moment_inverted);
	displaceInertiaMoment(_inertia_moment_inverted, getCentroid());
	_inertia_moment_inverted = _inertia_moment_inverted.getInverse();
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
	: Particle(init_pos, init_vel * 200, mass, radius, Vector3D(255, 255, 255), 255)
	, _angular_velocity(init_angular_vel), _orientation(init_orientation)
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

void RigidBody::integrate(float dt, IntegrationMethods method)
{
	Particle::integrate(dt, method);
	
	_angular_acceleration = _inertia_moment_inverted * _torque;
	_torque.set(0, 0, 0);

	_angular_velocity = _angular_velocity + _angular_acceleration * dt;

	_orientation += 0.5f * Quaternion(0,_angular_velocity) * _orientation * dt;

	_orientation.normalize();
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

#ifdef DEBUG_RIGID_BODY
	drawApproximation();
#endif // DEBUG_RIGID_BODY
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
}
