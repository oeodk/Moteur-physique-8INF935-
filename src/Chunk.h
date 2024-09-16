#pragma once
#include "Drawable.h"
#include "ofVboMesh.h"
#include "Vector3D.h

class Chunk :
    public Drawable
{
private :
	ofVboMesh  _terrain;
	std::vector<Vector3D> _bushes;
	std::vector<unsigned int> _bushes_to_draw;
	Vector3D _grid_coordinate;
	Vector3D _spatial_coordinate;



	float terrainNoise(float x, float y, int octaves, float persistence, float lacunarity);
public :
	Chunk() = default;
	Chunk(const Chunk&) = delete;
	void setup(float chunk_size, int chunk_division, const Vector3D& space_coordinate, int seed);
	void draw() override;

	const Vector3D& getGridCoordinate() const {return _grid_coordinate;}
	const Vector3D& getSpatialCoordinate() const { return _spatial_coordinate; }

};

