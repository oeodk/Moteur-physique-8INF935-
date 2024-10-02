#include "Terrain.h"
#include "Vector3D.h"	
#include <of3dGraphics.h>
#include <ofGraphics.h>

constexpr int CHUNK_DIVISION = 32;
constexpr bool RANDOM_SEED = true;



Terrain::Terrain() {
	ofAddListener(ofEvents().keyPressed, this, &Terrain::keyPressed);
	_chunk_size = 320;
	for (size_t i = 0; i < _block_chunk_movement.size(); i++) {
		_block_chunk_movement[i] = nullptr;
	}
}

Terrain::~Terrain() {
	ofRemoveListener(ofEvents().keyPressed, this, &Terrain::keyPressed);

	for (auto& future : _chunk_generators) {
		if (future.valid()) {
			future.wait();
		}
	}
	for (size_t i = 0; i < _chunks.size(); i++) {
		delete _chunks[i];
	}
}

void Terrain::setup() {
	_seed = 0;
	if (RANDOM_SEED) {
		_seed = rand();
	}

	for (int i = -_generation_distance; i < _generation_distance + 1; i++) {
		for (int j = -_generation_distance; j < _generation_distance + 1; j++) {
			generateChunk(i, j);
		}
	}
	_player_previous_chunk.set(0);
	_z_update = true;
}

void Terrain::generateChunk(int x, int z, Chunk* chunk_to_edit) {
	Chunk* chunk = chunk_to_edit;
	if (chunk == nullptr) {
		chunk = new Chunk();
		_chunks.push_back(chunk);
	}
	chunk->setup(_chunk_size, CHUNK_DIVISION, Vector3D(x, 0, z), _seed);
}

void Terrain::refreshChunk() {
	_chunk_to_move.clear();
	int chunk_reseted = 0;
	for (int i = -_generation_distance + _player_previous_chunk.x; i < _generation_distance + 1 + _player_previous_chunk.x; i++) {
		for (int j = -_generation_distance + _player_previous_chunk.z; j < _generation_distance + 1 + _player_previous_chunk.z; j++) {
			generateChunk(i, j, _chunks[chunk_reseted]);
			chunk_reseted++;
		}
	}
}

std::tuple<int, int> Terrain::getInGridCoordinate(float x, float z) const {
	return { (x) * static_cast<float>(CHUNK_DIVISION + 2) / ((CHUNK_DIVISION - 1) * _chunk_size), (z) * static_cast<float>(CHUNK_DIVISION + 2) / ((CHUNK_DIVISION - 1) * _chunk_size) };
}

void Terrain::update(const Vector3D& player_position) {
	for (const std::future<void>& generator : _chunk_generators) {
		if (generator.valid()) {
			generator.wait();
		}
	}
	for (size_t i = 0; i < _block_chunk_movement.size(); i++) {
		_block_chunk_movement[i] = nullptr;
	}

	_rendered_chunks.clear();
	const int chunk_number = _chunks.size();
	auto [player_chunk_x, player_chunk_z] = getInGridCoordinate(player_position.x, player_position.z);

	for (int i = 0; i < chunk_number; i++) {
		Vector3D chunk_distance(player_position);
		chunk_distance.y = 0;
		const Vector3D& chunk_spatial_coordinate = _chunks[i]->getSpatialCoordinate();
		const Vector3D& chunk_grid_coordinate = _chunks[i]->getGridCoordinate();

		chunk_distance -= chunk_spatial_coordinate;
		const float chunk_distance_squared_norm = chunk_distance.squareNorm();
		// Only render chunk that are not too far from the player
		if (chunk_distance_squared_norm < _render_distance * _render_distance) {
			_chunks[i]->update(chunk_distance_squared_norm);
			_rendered_chunks.push_back(_chunks[i]);
		}
		else {
			// Put chunk that are too far from the player in the generation queue
			if (_z_update) {
				if (chunk_grid_coordinate.z > _generation_distance + player_chunk_z + 0.5 || chunk_grid_coordinate.z < player_chunk_z - (_generation_distance + 0.5)) {
					_chunks[i]->setGridCoordinate(Vector3D(chunk_grid_coordinate.x, 0, chunk_grid_coordinate.z + (_generation_distance * 2 + 1) * ((player_chunk_z - chunk_grid_coordinate.z) / std::abs(player_chunk_z - chunk_grid_coordinate.z))));
					_chunk_to_move.push_back(_chunks[i]);
				}
			}
			else {
				if (chunk_grid_coordinate.x > _generation_distance + player_chunk_x + 0.5 || chunk_grid_coordinate.x < player_chunk_x - (_generation_distance + 0.5)) {
					_chunks[i]->setGridCoordinate(Vector3D(chunk_grid_coordinate.x + (_generation_distance * 2 + 1) * ((player_chunk_x - chunk_grid_coordinate.x) / std::abs(player_chunk_x - chunk_grid_coordinate.x)), 0, chunk_grid_coordinate.z));
					_chunk_to_move.push_back(_chunks[i]);
				}
			}
		}
	}

	size_t index = 0;
	int blocked_generation = 0;
	// Generate a part of the gereration queue's chunk
	while (_chunk_to_move.size() > 0 && index < 10 && blocked_generation < _chunk_to_move.size()) {
		bool generate = true;
		for (size_t i = 0; i < _block_chunk_movement.size(); i++) {
			if (_block_chunk_movement[i] == nullptr) {
				break;
			}
			if (_block_chunk_movement[i] == _chunk_to_move.front()) {
				generate = false;
				_chunk_to_move.push_back(_chunk_to_move.front());
				_chunk_to_move.erase(_chunk_to_move.begin());
				blocked_generation++;
				break;
			}
		}
		if (generate) {
			const Vector3D& chunk_grid_coordinate(_chunk_to_move.front()->getGridCoordinate());
			_chunk_generators[index] = (std::async(std::launch::async, &Terrain::generateChunk, chunk_grid_coordinate.x, chunk_grid_coordinate.z, _chunk_to_move.front()));
			_block_chunk_movement[index] = _chunk_to_move.front();
			_chunk_to_move.erase(_chunk_to_move.begin());
			index++;
		}
	}
	if (_z_update) {
		_player_previous_chunk.z = player_chunk_z;
	}
	else {
		_player_previous_chunk.x = player_chunk_x;
	}
	_z_update = !_z_update;
}

void Terrain::sedRenderDistance(float value) {
	int rendered_chunk = value / _chunk_size;
	_render_distance = (rendered_chunk + 4) * _chunk_size;
	_generation_distance = (rendered_chunk + 6);
}

void Terrain::keyPressed(ofKeyEventArgs& key) {
	if (key.key == ofKey::OF_KEY_F5) {
		refreshChunk();
	}
}

float Terrain::getHeight(float x, float z) const {
	auto [chunk_x, chunk_z] = getInGridCoordinate(x, z);
	Chunk* relevant_chunk = nullptr;
	const size_t chunk_number = _chunks.size();
	size_t i = 0;
	do {
		const Vector3D& chunk_coord = _chunks[i]->getGridCoordinate();
		if (chunk_x == chunk_coord.x && chunk_z == chunk_coord.z) {
			relevant_chunk = _chunks[i];
		}
		i++;

	} while (relevant_chunk == nullptr && i < chunk_number);
	if (relevant_chunk != nullptr) {
		return relevant_chunk->getHeight(x, z);
	}
	return 0.0f;
}

