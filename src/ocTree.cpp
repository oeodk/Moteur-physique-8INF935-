#include "ocTree.h"

ocTree::ocTree():
	_nodeVolume(new Cube()) {}

ocTree::ocTree(const Vector3D& origin, const Vector3D& size, int depth) :
	_nodeVolume(new Cube(origin, size)),
	_depth(depth)
{
	_children.assign(8, nullptr);
	_octree_mesh.setMode(OF_PRIMITIVE_LINES);

	for (int i = 0; i < 7; i++)
	{
		_octree_mesh.addIndex(i);
		_octree_mesh.addIndex(i + 1);
	}
	_octree_mesh.addIndex(7);
	_octree_mesh.addIndex(0);

	_octree_mesh.addIndex(7);
	_octree_mesh.addIndex(2);

	_octree_mesh.addIndex(6);
	_octree_mesh.addIndex(3);

	_octree_mesh.addIndex(0);
	_octree_mesh.addIndex(5);

	_octree_mesh.addIndex(1);
	_octree_mesh.addIndex(4);

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

	// If there is not enough particles to fill the node or if the max depth is reached, it's a leaf
	if (relevantParticles.size() <= PARTICLE_PER_NODE || _depth >= MAX_DEPTH) {
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
		_children[i] = new ocTree(origin, size, _depth + 1);
		_children[i]->build(childParticles[i]);
	}
}

void ocTree::checkCollisions(float dt) {
	if (_particles.size() > 1) {
		for (auto& p : _particles)
		{
			for (auto& p2 : _particles)
			{
				if (p != p2)
					p->checkCollision(p2, dt);
			}
		}
	}

	for (auto child : _children)
		if (child != nullptr)
			child->checkCollisions(dt);
}

void ocTree::drawNoLight() {
	_octree_mesh.clearVertices();
	ofVboMesh tmp_mesh;
	tmp_mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	const Vector3D& size = _nodeVolume->getSize();
	
	_octree_mesh.addVertex({ -size.x / 2.f, -size.y / 2.f, -size.z / 2.f});
	_octree_mesh.addVertex({ -size.x / 2.f, -size.y / 2.f,  size.z / 2.f});
	_octree_mesh.addVertex({ -size.x / 2.f,  size.y / 2.f,  size.z / 2.f});
	_octree_mesh.addVertex({  size.x / 2.f,  size.y / 2.f,  size.z / 2.f});
	_octree_mesh.addVertex({  size.x / 2.f, -size.y / 2.f,  size.z / 2.f});
	_octree_mesh.addVertex({  size.x / 2.f, -size.y / 2.f, -size.z / 2.f});
	_octree_mesh.addVertex({  size.x / 2.f,  size.y / 2.f, -size.z / 2.f});
	_octree_mesh.addVertex({ -size.x / 2.f,  size.y / 2.f, -size.z / 2.f});
	for (auto& v : _octree_mesh.getVertices())
		v += glm::vec3(_nodeVolume->getOrigin());
	
	ofSetColor(255.0 / (_depth + 1), 255.0 / (_depth + 1), 255.0 / (_depth + 1), 255);

	// Draw the box
	_octree_mesh.drawWireframe();

	// Draw every non-null child
	for (auto& oT : _children)
		if (oT != nullptr)
			oT->drawNoLight();
}