#pragma once
#include "Chunk.h"
#include <future>

class Terrain
{
private :
	inline static std::vector<Chunk*> _chunks;
	std::vector<Chunk*> _rendered_chunks;
	int _terrain_size = 10000, _terrain_division = 1024;
	inline static float _chunk_size;
	inline static int _seed;
	float _render_distance = 2200;
	float _generation_distance;

	Vector3D player_previous_chunk;
	bool _z_update;

	std::array<std::future<void>, 10> _chunk_generators;
	std::array<Chunk*, 10> _block_chunk_movement;
	std::list<Chunk*> _chunk_to_move;


	static void generateChunk(int x, int z, Chunk* chunk_to_edit = nullptr);
	void refreshChunk();
	std::tuple<int, int> getInGridCoordinate(float x, float z) const;
public :
	Terrain();
	~Terrain();
	void setup();
	
	//void draw() override;
	void update(const Vector3D& player_position);
	const std::vector<Chunk*>& getRenderedChunk() const { return _rendered_chunks; }
	void sedRenderDistance(float value);
	void keyPressed(ofKeyEventArgs& key);
};

