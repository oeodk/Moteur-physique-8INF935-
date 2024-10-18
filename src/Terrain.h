#pragma once
#include "Chunk.h"
#include <future>

// Environment

class Terrain {
private:
	inline static std::vector<Chunk*> _chunks;
	std::vector<Chunk*> _rendered_chunks;

	inline static float _chunk_size;
	// Seed for world generation
	inline static int _seed;

	// Distance for rendering chunks
	float _render_distance;
	// Distance for creating chunks
	float _generation_distance;

	Vector3D _player_previous_chunk;
	// bool used to alternate between chunk update on the x and z axis
	bool _z_update;

	// Thread that generate chunk
	std::array<std::future<void>, 10> _chunk_generators;
	// Array that prevent chunk from beeing moved twice on the same frame
	std::array<Chunk*, 10> _block_chunk_movement;
	std::list<Chunk*> _chunk_to_move;


	std::tuple<int, int> getInGridCoordinate(float x, float z) const;
	static void generateChunk(int x, int z, Chunk* chunk_to_edit = nullptr);
	void refreshChunk();
	// Convert coordinate from world coordinate to chunk grid coordinate

public:
	Terrain();
	~Terrain();
	void setup();

	void update(const Vector3D& player_position);
	const std::vector<Chunk*>& getRenderedChunk() const { return _rendered_chunks; }
	void sedRenderDistance(float value);
	void keyPressed(ofKeyEventArgs& key);

	float getHeight(float x, float z) const;
	Vector3D getNormal(float x, float z) const;
};

