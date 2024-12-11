#pragma once

// Implémentation inspirée (mais pas copiée!) du site GeeksForGeeks : https://www.geeksforgeeks.org/octree-insertion-and-searching/

#include <vector>
#include "Vector3D.h"
#include "particles/rigid_bodies/RigidBody.h"
using namespace std;

constexpr int PARTICLE_PER_NODE = 2;
constexpr int CHILD_PER_NODE = 8;
constexpr int MAX_DEPTH = 8;

constexpr int TopLeftFront = 0;
constexpr int TopRightFront = 1;
constexpr int BottomRightFront = 2;
constexpr int BottomLeftFront = 3;
constexpr int TopLeftBack = 4;
constexpr int TopRightBack = 5;
constexpr int BottomRightBack = 6;
constexpr int BottomLeftBack = 7;

class Cube {
private:
	Vector3D _origin;
	Vector3D _size;

public:
	const Vector3D& getOrigin() { return _origin; }
	const Vector3D& getSize() { return _size; }
	Cube(const Vector3D& origin = Vector3D(0., 0., 0.), const Vector3D& size = Vector3D(1., 1., 1.)) :
		_origin(origin), _size(size) {}
};


class ocTree : public Drawable {
protected:
	Cube* _nodeVolume;
	vector<RigidBody*> _particles;
	vector<ocTree*> _children;
	int _depth;

	ofVboMesh _octree_mesh;

public:
	ocTree();
	ocTree(const Vector3D& origin, const Vector3D& size, int depth = 0);
	~ocTree();

	void build(const vector<RigidBody*>& particles);
	vector<int> getAffectedChildIndices(Particle* p);

	void checkCollisions(float dt);

	void drawNoLight() override;
};

