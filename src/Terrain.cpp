#include "Terrain.h"
#include "Vector3D.h"	
#include <of3dGraphics.h>
#include <ofGraphics.h>

constexpr int CHUNK_DIVISION = 32;
constexpr bool RANDOM_SEED = true;



Terrain::Terrain()
{
	ofAddListener(ofEvents().keyPressed, this, &Terrain::keyPressed);
	_chunk_size = 320;// _terrain_size / (static_cast<float>(_terrain_division) / (CHUNK_DIVISION + 2));
}

Terrain::~Terrain()
{
	ofRemoveListener(ofEvents().keyPressed, this, &Terrain::keyPressed);

	for (size_t i = 0; i < _chunks.size(); i++)
	{
		delete _chunks[i];
	}
}

void Terrain::setup()
{
	//ofImage noise;
	//std::cout << noise.load("noise_1024x1024.jpg");
	//int noise_width = noise.getWidth();

	_seed = 0;
	if(RANDOM_SEED)
	{
		_seed = rand();
	}
	//_chunks.reserve((_terrain_division / CHUNK_DIVISION) * (_terrain_division / CHUNK_DIVISION));

	for (int i = -_generation_distance; i < _generation_distance + 1; i++)
	{
		for (int j = -_generation_distance; j < _generation_distance + 1; j++)
		{
			//if (_chunk_size * _chunk_size * (i * i + j * j) < _generation_distance * _generation_distance)
			{
				generateChunk(i, j);
			}
		}
	}
	player_previous_chunk.set(0);
	_z_update = true;
}

void Terrain::generateChunk(int x, int z, Chunk* chunk_to_edit)
{
	Chunk* chunk = chunk_to_edit;
	if (chunk == nullptr)
	{
		chunk = new Chunk();
		_chunks.push_back(chunk);
	}
	//(CHUNK_DIVISION - 1) * _terrain_size * (z / static_cast<float>(_terrain_division)
	chunk->setup(_chunk_size, CHUNK_DIVISION, Vector3D(x, 0, z), _seed);
}

void Terrain::refreshChunk()
{
	int chunk_reseted = 0;
	for (int i = -_generation_distance + player_previous_chunk.x; i < _generation_distance + 1 + player_previous_chunk.x; i++)
	{
		for (int j = -_generation_distance + player_previous_chunk.z; j < _generation_distance + 1 + player_previous_chunk.z; j++)
		{
			generateChunk(i, j, _chunks[chunk_reseted]);
			chunk_reseted++;
		}
	}
}

std::tuple<int, int> Terrain::getInGridCoordinate(float x, float z) const
{
	return { (x) * static_cast<float>(CHUNK_DIVISION + 2) / ((CHUNK_DIVISION - 1) * _chunk_size), (z) * static_cast<float>(CHUNK_DIVISION + 2) / ((CHUNK_DIVISION - 1) * _chunk_size) };
}

void Terrain::update(const Vector3D& player_position)
{
	for (auto& future : _chunk_generators)
	{
		future.wait();
	}
	_rendered_chunks.clear();
	_chunk_generators.clear();
	int chunk_number = _chunks.size();

	auto [player_chunk_x, player_chunk_z] = getInGridCoordinate(player_position.x, player_position.z);
	//int player_chunk_x = (player_position.x) * static_cast<float>(CHUNK_DIVISION + 2) / ((CHUNK_DIVISION - 1) * _chunk_size);
	//int player_chunk_z = (player_position.z) * static_cast<float>(CHUNK_DIVISION + 2) / ((CHUNK_DIVISION - 1) * _chunk_size);

	for(int i = 0; i < chunk_number; i++)
	{
		Vector3D chunk_distance(player_position);
		chunk_distance.y = 0;
		const Vector3D& chunk_spatial_coordinate = _chunks[i]->getSpatialCoordinate();
		const Vector3D& chunk_grid_coordinate = _chunks[i]->getGridCoordinate();

		chunk_distance -= chunk_spatial_coordinate;
		float chunk_distance_squared_norm = chunk_distance.squareNorm();
		if (chunk_distance_squared_norm < _render_distance * _render_distance)
		{
			_rendered_chunks.push_back(_chunks[i]);
		}
		else
		{
			if (_z_update)
			{
				if (chunk_grid_coordinate.z > _generation_distance + player_chunk_z + 0.5 || chunk_grid_coordinate.z < player_chunk_z - (_generation_distance + 0.5))
				{
					_chunk_generators.push_back(std::async(std::launch::async, &Terrain::generateChunk, chunk_grid_coordinate.x, 2 * player_chunk_z - chunk_grid_coordinate.z + (player_previous_chunk.z - player_chunk_z), _chunks[i]));
				}
			}
			else
			{
				if (chunk_grid_coordinate.x > _generation_distance + player_chunk_x + 0.5 || chunk_grid_coordinate.x < player_chunk_x - (_generation_distance + 0.5))
				{
					_chunk_generators.push_back(std::async(std::launch::async, &Terrain::generateChunk, 2 * player_chunk_x - chunk_grid_coordinate.x + (player_previous_chunk.x - player_chunk_x), chunk_grid_coordinate.z, _chunks[i]));
				}
			}
		}
	}
	if (_z_update)
	{
		player_previous_chunk.z = player_chunk_z;
	}
	else
	{
		player_previous_chunk.x = player_chunk_x;
	}
	_z_update = !_z_update;
}

void Terrain::sedRenderDistance(float value)
{
	int rendered_chunk = (value ) / _chunk_size;
	_render_distance = (rendered_chunk + 4) * _chunk_size;
	_generation_distance = (rendered_chunk + 5);
}

void Terrain::keyPressed(ofKeyEventArgs& key)
{
	if (key.key == ofKey::OF_KEY_F5)
	{
		refreshChunk();
	}
}

