#include "ocTree.h"

ocTree::ocTree():
	_nodeVolume(new Cube()) {}

ocTree::ocTree(const Vector3D& origin, const Vector3D& size) :
	_nodeVolume(new Cube(origin, size))
{
	_children.assign(8, nullptr);
}

ocTree::~ocTree()
{
	for (auto& child : _children)
	{
		if(child)
		{
			delete child;
		}
	}
}

vector<int> ocTree::getAffectedChildIndices(Particle* p) {
	const Vector3D& particlePos = p->getParticlePosition();
	const Vector3D& origin = _nodeVolume->getOrigin();

	vector<int> r;

	// Affect child nodes to that Particle based on its relative position to the current node
	if (particlePos.x - p->getRadius() <= origin.x) {
		if (particlePos.y - p->getRadius() <= origin.y) {
			if (particlePos.z - p->getRadius() <= origin.z)
				r.push_back(TopLeftFront);
			if (particlePos.z + p->getRadius() > origin.z)
				r.push_back(TopLeftBack);
		}

		if (particlePos.y + p->getRadius() >= origin.y) {
			if (particlePos.z - p->getRadius() <= origin.z)
				r.push_back(BottomLeftFront);
			if (particlePos.z + p->getRadius() > origin.z)
				r.push_back(BottomLeftBack);
		}
	}

	if (particlePos.x + p->getRadius() > origin.x) {
		if (particlePos.y - p->getRadius() <= origin.y) {
			if (particlePos.z - p->getRadius() <= origin.z)
				r.push_back(TopRightFront);
			if (particlePos.z + p->getRadius() > origin.z)
				r.push_back(TopRightBack);
		}

		if (particlePos.y + p->getRadius() > origin.y) {
			if (particlePos.z - p->getRadius() <= origin.z)
				r.push_back(BottomRightFront);
			if (particlePos.z + p->getRadius() > origin.z)
				r.push_back(BottomRightBack);
		}
	}
	return r;
}

void ocTree::build(const vector<RigidBody*>& particles) {
	vector<RigidBody*> relevantParticles;
	for (RigidBody* p : particles) {
		// If for some reason, the particle is not in the bounds of the octree, ignore it
		if (abs(p->getPosition()->x - _nodeVolume->getOrigin().x) > _nodeVolume->getSize().x / 2 + p->getRadius()
			|| abs(p->getPosition()->y - _nodeVolume->getOrigin().y) > _nodeVolume->getSize().y / 2 + p->getRadius()
			|| abs(p->getPosition()->z - _nodeVolume->getOrigin().z) > _nodeVolume->getSize().z / 2 + p->getRadius()
			) continue;
		relevantParticles.push_back(p);
	}

	// If there is not enough particles to fill the node, it's a leaf
	if (relevantParticles.size() <= PARTICLE_PER_NODE) {
		for (RigidBody* p : relevantParticles)
			_particles.push_back(p);

		return;
	}

	// If not, divide the particles in 8 groups (one for each node)
	vector<RigidBody*> childParticles[CHILD_PER_NODE];
	for (RigidBody* p : relevantParticles) {
		const vector<int> childIndices = getAffectedChildIndices(p);
		for (int i : childIndices) childParticles[i].push_back(p);
	}

	// Build the child octrees if they contain at least one particle
	for (int i = 0; i < _children.size(); i++) {
		if (childParticles[i].size() == 0) continue;

		Vector3D size = _nodeVolume->getSize() / 2;
		Vector3D origin = _nodeVolume->getOrigin();
		switch (i) {
			case TopLeftFront:		origin += Vector3D(-size.x,	-size.y, -size.z) / 2; break;
			case TopLeftBack:		origin += Vector3D(-size.x,	-size.y, size.z ) / 2; break;
			case BottomLeftFront:	origin += Vector3D(-size.x,	size.y,	 -size.z) / 2; break;
			case BottomLeftBack:	origin += Vector3D(-size.x,	size.y,	 size.z ) / 2; break;
			case TopRightFront:		origin += Vector3D(size.x,	-size.y, -size.z) / 2; break;
			case TopRightBack:		origin += Vector3D(size.x,	-size.y, size.z ) / 2; break;
			case BottomRightFront:	origin += Vector3D(size.x,	size.y,	 -size.z) / 2; break;
			case BottomRightBack:	origin += Vector3D(size.x,	size.y,	 size.z ) / 2; break;
			default: break;
		}
		_children[i] = new ocTree(origin, size);
		_children[i]->build(childParticles[i]);
	}
}

void ocTree::drawNoLight() {
	ofVboMesh tmp_mesh;
	tmp_mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	const Vector3D& size = _nodeVolume->getSize();
	
	// Create a box mesh
	tmp_mesh.append(ofVboMesh::box(size.x, size.y, size.z));
	for (auto& v : tmp_mesh.getVertices())
		v += glm::vec3(_nodeVolume->getOrigin());

	_octree_mesh.addVertices(tmp_mesh.getVertices());
	_octree_mesh.addNormals(tmp_mesh.getNormals());
	_octree_mesh.addIndices(tmp_mesh.getIndices());

	ofSetColor(255, 255, 255, 255);

	// Draw the box
	_octree_mesh.drawWireframe();

	// Draw every non-null child
	for (auto& oT : _children)
		if (oT != nullptr)
			oT->drawNoLight();
}