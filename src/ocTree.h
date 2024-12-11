#pragma once

// Implémentation inspirée (mais pas copiée!) du site GeeksForGeeks : https://www.geeksforgeeks.org/octree-insertion-and-searching/

#include <vector>
#include "Vector3D.h"
#include "particles/Particle.h"
using namespace std;

constexpr int PARTICLE_PER_NODE = 2;
constexpr int CHILD_PER_NODE = 8;

#define TopLeftFront 0
#define TopRightFront 1
#define BottomRightFront 2
#define BottomLeftFront 3
#define TopLeftBack 4
#define TopRightBack 5
#define BottomRightBack 6
#define BottomLeftBack 7

class Cube {
private:
	Vector3D _origin;
	Vector3D _size;

public:
	const Vector3D getOrigin() { return _origin; }
	const Vector3D getSize() { return _size; }
	Cube(Vector3D origin = Vector3D(0., 0., 0.), Vector3D size = Vector3D(1., 1., 1.)) :
		_origin(origin), _size(size) {}
};


class ocTree : public Drawable {
protected:
	Cube* _nodeVolume;
	vector<Particle*> _particles;
	vector<ocTree*> _children;

	ofVboMesh _octree_mesh;

public:
	ocTree();
	ocTree(Vector3D origin, Vector3D size);

	void build(const vector<Particle*>& particles);
	vector<int> getAffectedChildIndices(Particle* p);

	void draw() override;
	void drawNoLight() override;
};

