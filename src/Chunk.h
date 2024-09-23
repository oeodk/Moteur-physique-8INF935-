#pragma once
#include "Drawable.h"
#include "ofVboMesh.h"
#include "Vector3D.h"
#include <future>
#include <of3dPrimitives.h>

class Chunk :
    public Drawable
{
private :
	ofVboMesh _terrain;
	ofVboMesh* _trees_buffer[2];
	char _used_tree_buffer;
	std::atomic<bool> _can_edit_tree;
	std::vector<Vector3D> _bushes;
	std::vector<unsigned int> _bushes_to_draw;
	Vector3D _grid_coordinate;
	Vector3D _spatial_coordinate;
	int _distance_range;

	inline static bool _primitive_initialized = false;
	inline static ofConePrimitive _trees_primitive[3];

	std::future<void> _tree_loader;
	bool _delete_old_buffer;
	std::list<int> _tree_loading_queue;

	float terrainNoise(float x, float y, int octaves, float persistence, float lacunarity);
	static void generateTrees(int primitive_index, Chunk* chunk);
public :
	Chunk();
	Chunk(const Chunk&) = delete;
	void setup(float chunk_size, int chunk_division, const Vector3D& grid_coordinate, int seed);
	void draw() override;

	void update(float player_distance);

	const Vector3D& getGridCoordinate() const {return _grid_coordinate;}
	const Vector3D& getSpatialCoordinate() const { return _spatial_coordinate; }
	void setGridCoordinate(const Vector3D& grid_coordinate) { _grid_coordinate.set(grid_coordinate); }
};

