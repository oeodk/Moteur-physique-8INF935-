#pragma once
#include "Drawable.h"
#include "ofVboMesh.h"
#include "Vector3D.h"
#include "GlobalConstants.h"
#include <future>
#include <of3dPrimitives.h>

static constexpr int CHUNK_DIV_OFFSET = 4;

// Part of the terrain

class Chunk :
	public Drawable {
private:
	ofVboMesh _terrain;
	int _chunk_division;
	Vector3D _grid_coordinate;
	Vector3D _spatial_coordinate;

	ofVboMesh* _trees_buffer[2];
	char _used_tree_buffer;
	std::atomic<bool> _can_edit_tree;
	std::atomic<bool> _is_doing_setup;
	std::vector<Vector3D> _trees_positions;
	// Level of detail of the trees
	ChunkLod _lod;

	// Base primitives for the trees
	inline static ofConePrimitive _trees_primitive[3];
	inline static bool _primitive_initialized = false;

	// Threes loading thread
	std::future<void> _tree_loader;
	std::list<int> _tree_loading_queue;
	bool _delete_old_buffer;

	float terrainNoise(float x, float y, int octaves, float persistence, float lacunarity);
	static void generateTrees(int primitive_index, Chunk* chunk);
	static float interpolateHeight4p(const Vector3D& point_to_interpolate, const Vector3D & top_left, const Vector3D & top_right, const Vector3D & bottom_right, const Vector3D & bottom_left);
	Vector3D interpolateNormal4p(const Vector3D& point_to_interpolate, float top_left_index, float top_right_index, float bottom_right_index, float bottom_left_index) const;
public:
	Chunk();
	Chunk(const Chunk&) = delete;
	void setup(float chunk_size, int chunk_division, const Vector3D& grid_coordinate, int seed);
	void draw() override;
	void update(float player_distance);

	const Vector3D& getGridCoordinate() const { return _grid_coordinate; }
	const Vector3D& getSpatialCoordinate() const { return _spatial_coordinate; }
	void setGridCoordinate(const Vector3D& grid_coordinate) { _grid_coordinate.set(grid_coordinate); }
	float getHeight(float x, float z) const;
	Vector3D getNormal(float x, float z) const;
};

