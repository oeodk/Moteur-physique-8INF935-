#pragma once
#include "Drawable.h"
#include "ofVboMesh.h"
#include "Vector3D.h"
#include <future>

class Chunk :
    public Drawable
{
private :
	ofVboMesh  _terrain;
	ofVboMesh  _trees;
	std::vector<Vector3D> _bushes;
	std::vector<unsigned int> _bushes_to_draw;
	Vector3D _grid_coordinate;
	Vector3D _spatial_coordinate;
	int _distance_range;

	std::future<void> _tree_loader;
	std::list<int> _tree_loading_queue;

	float terrainNoise(float x, float y, int octaves, float persistence, float lacunarity);
	static void generateTrees(int section_point, Chunk* chunk);
public :
	Chunk() = default;
	Chunk(const Chunk&) = delete;
	void setup(float chunk_size, int chunk_division, const Vector3D& grid_coordinate, int seed);
	void draw() override;

	void update(float player_distance);

	const Vector3D& getGridCoordinate() const {return _grid_coordinate;}
	const Vector3D& getSpatialCoordinate() const { return _spatial_coordinate; }
	void setGridCoordinate(const Vector3D& grid_coordinate) { _grid_coordinate.set(grid_coordinate); }
};

