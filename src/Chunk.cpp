#include "Chunk.h"
#include "ofApp.h"
#include "particles/FireballParticle.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>
#include <chrono>
#include <thread>
#include <cmath>
using namespace std::chrono_literals;

constexpr float HEIGH_FACTOR = 5;
constexpr unsigned char WATER_LEVEL = 80;
constexpr unsigned char STONE_LEVEL = 130;
constexpr unsigned char SNOW_LEVEL = 190;

inline const ofColor COLOR_TABLE[] = { ofColor(42, 104, 134), ofColor(63, 155, 11), ofColor(137, 125, 107), ofColor(255, 200, 200), ofColor(226, 202, 118), ofColor(207, 16, 32),  ofColor(115, 155, 11) };


// Constants for the terrain noise
// Base scale for noise
constexpr float NOISE_SCALE = 0.002;//0.004
// Number of noise layers to combine
constexpr int OCTAVES = 10;
// How much amplitude decreases each octave
constexpr float PERSISTANCE = 0.4;
// How much frequency increases each octave
constexpr float LACUNARITY = 2.0;

constexpr float TREE_RADIUS = 20;

Chunk::Chunk() {
	_used_tree_buffer = 0;
	_can_edit_tree = true;
	_is_doing_setup = false;

	_trees_buffer[0] = nullptr;
	_trees_buffer[1] = nullptr;

	_terrain.disableTextures();

	_delete_old_buffer = false;

	if (!_primitive_initialized) {
		_primitive_initialized = true;
		_trees_primitive[0].load("tree_min_details.ply");
		_trees_primitive[1].load("tree_medium_details.ply");
		_trees_primitive[2].load("tree_max_details.ply");
		_trees_primitive[3].load("palm_tree.ply");
		for (size_t i = 0; i < _trees_primitive.size(); i++)
		{
			_trees_primitive[0].clearTexCoords();
			_trees_primitive[0].disableTextures();
		}
	}
}

float smoothstep(float edge0, float edge1, float x)
{
	// Scale, bias and saturate x to 0..1 range
	x = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	// Evaluate polynomial (3x^2 - 2x^3)
	return x * x * (3 - 2 * x);
}
float cubicExpInterp(float edge0, float edge1, float x, float exponent = 3.0f)
{
	// Clamp x to the range [edge0, edge1] and normalize to [0, 1]
	float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);

	// Apply cubic polynomial
	return std::pow(t, exponent);
}

void Chunk::setup(float chunk_size, int chunk_division, const Vector3D& grid_coordinate, int seed) {
	_is_doing_setup = true;
	_terrain.clear();
	_trees_positions.clear();

	_chunk_division = chunk_division;

	_lod = ChunkLod::NONE;
	srand(seed);
	_world_position = &_spatial_coordinate;
	_grid_coordinate = grid_coordinate;
	_old_grid_coordinate = _grid_coordinate;
	_spatial_coordinate.x = chunk_size * grid_coordinate.x;
	_spatial_coordinate.y = 0;
	_spatial_coordinate.z = chunk_size * grid_coordinate.z;

	int noise_offset_x = grid_coordinate.x * (_chunk_division);// - CHUNK_DIV_OFFSET);
	int noise_offset_y = grid_coordinate.z * (_chunk_division);// -CHUNK_DIV_OFFSET);

	bool has_oasis = false;

	_chunk_division += 1;
	for (int i = 0; i < _chunk_division; i++) {
		for (int j = 0; j < _chunk_division; j++) {
			

			
			constexpr float BIOME_FACTOR = 5;
			float biome = terrainNoise(seed + (noise_offset_x + i) * NOISE_SCALE / BIOME_FACTOR, seed + (noise_offset_y + j) * NOISE_SCALE / BIOME_FACTOR, OCTAVES, PERSISTANCE, LACUNARITY);
		

			constexpr float MOUNTAIN_LIMIT = 0.4;
			constexpr float VOLCAN_LIMIT = 0.6;
			constexpr float DELTA_TRANSITION = 0.01;
			
			float height;
			if (biome > MOUNTAIN_LIMIT - DELTA_TRANSITION && biome < MOUNTAIN_LIMIT + DELTA_TRANSITION || biome > VOLCAN_LIMIT - DELTA_TRANSITION && biome < VOLCAN_LIMIT + DELTA_TRANSITION)
			{
				height = terrainNoise(seed + (noise_offset_x + i) * NOISE_SCALE, seed + (noise_offset_y + j) * NOISE_SCALE, OCTAVES / 2, PERSISTANCE, LACUNARITY);
			}
			else
			{
				height = terrainNoise(seed + (noise_offset_x + i) * NOISE_SCALE, seed + (noise_offset_y + j) * NOISE_SCALE, OCTAVES, PERSISTANCE, LACUNARITY);
			}
			float height_squared = (height + 0.5) * (height + 0.5);
			height *= 255;

			float smoothBiome = 1 - smoothstep(MOUNTAIN_LIMIT - DELTA_TRANSITION, MOUNTAIN_LIMIT + DELTA_TRANSITION, biome) + smoothstep(VOLCAN_LIMIT - DELTA_TRANSITION, VOLCAN_LIMIT + DELTA_TRANSITION, biome);
			float blendedHeight =  ofLerp(height * 2, height * height * height / 5000.f, smoothBiome);

			E_Biome current_biome;

			if (biome < MOUNTAIN_LIMIT)
			{
				current_biome = MOUNTAIN;
			}
			else
			{
				if (biome > VOLCAN_LIMIT)
				{
					current_biome = VOLCAN;
				}
				else
				{
					current_biome = DESERT;
				}
			}

			switch (current_biome)
			{
			case MOUNTAIN:
				if (height < WATER_LEVEL)
				{
					blendedHeight = WATER_LEVEL * WATER_LEVEL * WATER_LEVEL / 5000.f;
					height = WATER_LEVEL;
				}
				_terrain.addVertex(Vector3D(_spatial_coordinate.x + chunk_size * (i / static_cast<float>(_chunk_division - 1)), blendedHeight, _spatial_coordinate.z + chunk_size * (j / static_cast<float>(_chunk_division - 1))));
				addMountainColor(height, seed, noise_offset_x, i, noise_offset_y, j);
				break;

			case DESERT:
				if (height < WATER_LEVEL / 2)
				{
					blendedHeight = WATER_LEVEL;
					height = WATER_LEVEL / 2;
					has_oasis = true;
				}
				_terrain.addVertex(Vector3D(_spatial_coordinate.x + chunk_size * (i / static_cast<float>(_chunk_division - 1)), blendedHeight, _spatial_coordinate.z + chunk_size * (j / static_cast<float>(_chunk_division - 1))));
				if (height == WATER_LEVEL / 2.f)
				{
					_terrain.addColor(COLOR_TABLE[0]);
				}
				else
				{
					height += (rand() % 2 - 1);
					if (height < STONE_LEVEL / 3.f && has_oasis)
					{
						_terrain.addColor(COLOR_TABLE[6]);
						if (rand() % 15 == 0)
						{
							Vector3D tree_pos = _terrain.getVertex(_terrain.getNumVertices() - 1);
							_trees_positions.push_back({ tree_pos, DESERT });
						}
					}
					else
					{
						_terrain.addColor(COLOR_TABLE[4]);
					}
				}
				break;
			case VOLCAN:
				constexpr float LAVA_LEVEL = SNOW_LEVEL + 10;
				int color_index = 2;
				if (height < WATER_LEVEL + 10)
				{
					blendedHeight = (WATER_LEVEL + 10) * (WATER_LEVEL + 10) * (WATER_LEVEL + 10) / 5000.f;
					color_index = 5;
				}
				if (height > LAVA_LEVEL)
				{
					constexpr int CUT = 8;
					blendedHeight = (LAVA_LEVEL - CUT) * (LAVA_LEVEL - CUT) * (LAVA_LEVEL - CUT) / 5000.f;
					color_index = 5;
				}
				else
				{
					if (height > LAVA_LEVEL - 7)
					{
						blendedHeight = (LAVA_LEVEL - 5) * (LAVA_LEVEL - 5) * (LAVA_LEVEL - 5) / 5000.f;
					}
				}
				_terrain.addVertex(Vector3D(_spatial_coordinate.x + chunk_size * (i / static_cast<float>(_chunk_division - 1)), blendedHeight, _spatial_coordinate.z + chunk_size * (j / static_cast<float>(_chunk_division - 1))));
				_terrain.addColor(COLOR_TABLE[color_index]);
				if (height > LAVA_LEVEL + 10)
				{
					_can_erupt = true;
					_eruption_pos = _terrain.getVertices().back();
					_max_eruption_cooldown = (rand() % 50) / 10.f + 2.5;
					_current_eruption_cooldown = 0;
					_eruption_pos.y += 20;
				}
			}
		}
	}

	const auto& vertices = _terrain.getVertices();
	for (int i = 0; i < _chunk_division - 1; i++) {
		for (int j = 0; j < _chunk_division - 1; j++) {
			int current = i * (_chunk_division)+j;
			int next = current + (_chunk_division);

			_terrain.addIndex(current);
			_terrain.addIndex(next);
			_terrain.addIndex(current + 1);

			_terrain.addIndex(next);
			_terrain.addIndex(next + 1);
			_terrain.addIndex(current + 1);
		}
	}

	int vertex_number = _terrain.getNumVertices();
	std::vector<Vector3D> normals(vertex_number, Vector3D());

	const std::vector<ofIndexType>& indices = _terrain.getIndices();

	for (size_t i = 0; i < indices.size(); i += 3) {
		const int index_0 = indices[i];
		const int index_1 = indices[i + 1];
		const int index_2 = indices[i + 2];

		const Vector3D& v_0 = _terrain.getVertex(index_0);
		const Vector3D& v_1 = _terrain.getVertex(index_1);
		const Vector3D& v_2 = _terrain.getVertex(index_2);

		Vector3D face_normal = Vector3D::crossProduct(v_1 - v_0, v_2 - v_0);
		face_normal.normalize();

		normals[index_0] += face_normal;
		normals[index_1] += face_normal;
		normals[index_2] += face_normal;
	}

	// Normalize the normals for each vertex
	for (int i = 0; i < vertex_number; i++) {
		normals[i].normalize();
		normals[i].invert();
	}

	_terrain.clearNormals();
	for (int i = 0; i < vertex_number; i++) {
		_terrain.addNormal(normals[i]);
	}
	_is_doing_setup = false;
}

void Chunk::addMountainColor(float& height, int seed, int noise_offset_x, int i, int noise_offset_y, int j)
{
	if (height == WATER_LEVEL)
	{
		_terrain.addColor(COLOR_TABLE[0]);
	}
	else
	{
		char color_index = 1;

		height += (rand() % 30 - 15);
		if (height > STONE_LEVEL)
		{
			color_index++;
			if (height > SNOW_LEVEL)
			{
				color_index++;
			}
		}
		else
		{
			Vector3D tree_pos = _terrain.getVertex(_terrain.getNumVertices() - 1);
			tree_pos.x += rand() % 20 - 10;
			tree_pos.z += rand() % 20 - 10;
			//tree_pos.y += TREE_RADIUS;
			float tree_noise = ofNoise(seed + (noise_offset_x + i) * NOISE_SCALE * 2 + 1, seed + (noise_offset_y + j) * NOISE_SCALE * 2 + 1);
			if (tree_noise > 0.8)
			{
				_trees_positions.push_back({ tree_pos, MOUNTAIN });
			}
		}
		_terrain.addColor(COLOR_TABLE[color_index]);
	}
}

void Chunk::draw() {
	_terrain.draw();
	ofSetColor(255);
	if (_can_edit_tree) {
		_can_edit_tree = false;
		if (_trees_buffer[_used_tree_buffer] != nullptr) {
			//_trees_buffer[_used_tree_buffer]->enableColors();
			_trees_buffer[_used_tree_buffer]->draw();
		}
		_can_edit_tree = true;
	}
}

void Chunk::update(float player_distance, float dt) {
	// The old buffer deletion is done here because it need a valid Opengl context
	if (_delete_old_buffer) {
		_delete_old_buffer = false;
		delete _trees_buffer[1 - _used_tree_buffer];
	}
	bool generate_trees = false;
	int primitive_index = 0;
	if (player_distance < 160 * 160 * 4) {
		if (_lod != ChunkLod::HIGH) {
			_lod = ChunkLod::HIGH;
			generate_trees = true;
			primitive_index = 2;
		}
	}
	else {
		if (player_distance < (5 * 160) * (5 * 160)) {
			if (_lod != ChunkLod::MEDUIM) {
				_lod = ChunkLod::MEDUIM;
				generate_trees = true;
				primitive_index = 1;
			}
		}
		else {
			if (_lod != ChunkLod::LOW) {
				_lod = ChunkLod::LOW;
				generate_trees = true;
				primitive_index = 0;
			}
		}
	}
	if (generate_trees) {
		_tree_loading_queue.push_back(primitive_index);
	}
	if (_tree_loading_queue.size() != 0) {
		std::future_status status = std::future_status::ready;
		if (_tree_loader.valid()) {
			status = _tree_loader.wait_for(std::chrono::nanoseconds(10));
		}
		if (status == std::future_status::ready) {
			if (_tree_loader.valid()) {
				_tree_loader.wait();
			}
			_tree_loader = std::async(std::launch::async, &Chunk::generateTrees, _tree_loading_queue.front(), this);
			_tree_loading_queue.erase(_tree_loading_queue.begin());
		}
	}

	if (_can_erupt && player_distance < 160 * 160 * 10)
	{
		_current_eruption_cooldown += dt;
		if (_current_eruption_cooldown > _max_eruption_cooldown)
		{
			_current_eruption_cooldown -= _max_eruption_cooldown;
			float angle = rand() % 360;
			Vector3D dir(std::cos(ofDegToRad(angle)), 2, std::sin(ofDegToRad(angle)));
			dir.normalize();
			Particle* newParticle = new FireballParticle(_eruption_pos, dir);
			ofApp::addParticle(newParticle);
		}
	}
}

float Chunk::getHeight(float x, float z) const {
	if (_is_doing_setup) {
		return 0.f;
	}
	const auto& vertices = _terrain.getVertices();
	int i = 0;
	while (i < _chunk_division && vertices[i].z <= z) {
		i++;
	}
	i--;
	if (i < 0)
	{
		i = 0;
	}
	while (i < _chunk_division * _chunk_division && vertices[i].x <= x) {
		i += _chunk_division;
	}
	i -= _chunk_division;
	if (i < 0)
	{
		i = 0;
	}

	if (i < _chunk_division * (_chunk_division - 1) - 1)
	{
		Vector3D point_to_interpolate(x, 0, z);
		return interpolateHeight4p(point_to_interpolate, vertices[i], vertices[i + _chunk_division], vertices[i + _chunk_division + 1], vertices[i + 1]);
	}
	return 0.0f;
}

Vector3D Chunk::getNormal(float x, float z) const
{
	if (_is_doing_setup)
	{
		return 0.f;
	}
	const auto& vertices = _terrain.getVertices();
	int i = 0;
	while (i < _chunk_division && vertices[i].z <= z)
	{
		i++;
	}
	i--;
	if (i < 0)
	{
		i = 0;
	}
	while (i < _chunk_division * _chunk_division && vertices[i].x <= x)
	{
		i += _chunk_division;
	}
	i -= _chunk_division;
	if (i < 0)
	{
		i = 0;
	}

	if (i < _chunk_division * (_chunk_division - 1) - 1)
	{
		Vector3D point_to_interpolate(x, 0, z);
		return interpolateNormal4p(point_to_interpolate, i, i + _chunk_division, i + _chunk_division + 1, i + 1);
	}
	return 0.0f;
}

float Chunk::terrainNoise(float x, float y, int octaves, float persistence, float lacunarity) {
	float total = 0.0f;
	float amplitude = 1.0f;
	float frequency = 1.0f;
	float max_value = 0.0f;

	for (int i = 0; i < octaves; i++) {
		total += ofNoise(x * frequency, y * frequency) * amplitude;

		max_value += amplitude;

		amplitude *= persistence;
		frequency *= lacunarity;
	}

	return total / max_value;
}

void Chunk::generateTrees(int primitive_index, Chunk* chunk) {
	chunk->_trees_buffer[1 - chunk->_used_tree_buffer] = new ofVboMesh();

	for (const auto& tree : chunk->_trees_positions) {
		int real_primitive_index = primitive_index;
		if (tree.biome == DESERT)
		{
			real_primitive_index = 3;
		}
		for (const auto& v : _trees_primitive[real_primitive_index].getVertices()) {
			chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->addVertex(v + tree.position);
		}

		chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->addNormals(_trees_primitive[real_primitive_index].getNormals());

		int base_index = chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->getNumVertices();
		for (const auto& ind : _trees_primitive[real_primitive_index].getIndices()) {
			chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->addIndex(ind + base_index);
		}

		chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->addColors(_trees_primitive[real_primitive_index].getColors());
		
	}
	while (!chunk->_can_edit_tree);
	chunk->_can_edit_tree = false;
	chunk->_used_tree_buffer = 1 - chunk->_used_tree_buffer;
	chunk->_can_edit_tree = true;
	chunk->_delete_old_buffer = true;
}

float Chunk::interpolateHeight4p(const Vector3D& point_to_interpolate, const Vector3D & top_left, const Vector3D & top_right, const Vector3D & bottom_right, const Vector3D & bottom_left)
{
	const float border_size = (top_right.x - top_left.x);
	const float inverse_border_size_squared = 1 / (border_size * border_size);
	const float top_left_part     = (border_size - abs(point_to_interpolate.x - top_left.x)    ) * (border_size - abs(point_to_interpolate.z - top_left.z)) * inverse_border_size_squared ;
	const float top_right_part    = (border_size - abs(top_right.x - point_to_interpolate.x)   ) * (border_size - abs(point_to_interpolate.z - top_right.z)) * inverse_border_size_squared ;
	const float bottom_right_part = (border_size - abs(bottom_right.x - point_to_interpolate.x)) * (border_size - abs(bottom_right.z - point_to_interpolate.z)) * inverse_border_size_squared;
	const float bottom_left_part  = (border_size - abs(point_to_interpolate.x - bottom_left.x) ) * (border_size - abs(bottom_left.z - point_to_interpolate.z)) * inverse_border_size_squared;
	return top_left_part * top_left.y + top_right_part * top_right.y + bottom_right_part * bottom_right.y + bottom_left_part * bottom_left.y;
}

Vector3D Chunk::interpolateNormal4p(const Vector3D& point_to_interpolate, float top_left_index, float top_right_index, float bottom_right_index, float bottom_left_index) const
{
	const auto& vertices = _terrain.getVertices();
	const auto& normales = _terrain.getNormals();

	const float border_size = (vertices[top_right_index].x - vertices[top_left_index].x);
	const float inverse_border_size_squared = 1 / (border_size * border_size);
	const float top_left_part     = (border_size - abs(point_to_interpolate.x - vertices[top_left_index].x)) * (border_size - abs(point_to_interpolate.z - vertices[top_left_index].z)) * inverse_border_size_squared;
	const float top_right_part    = (border_size - abs(vertices[top_right_index].x - point_to_interpolate.x)) * (border_size - abs(point_to_interpolate.z - vertices[top_right_index].z)) * inverse_border_size_squared;
	const float bottom_right_part = (border_size - abs(vertices[bottom_right_index].x - point_to_interpolate.x)) * (border_size - abs(vertices[bottom_right_index].z - point_to_interpolate.z)) * inverse_border_size_squared;
	const float bottom_left_part  = (border_size - abs(point_to_interpolate.x - vertices[bottom_left_index].x)) * (border_size - abs(vertices[bottom_left_index].z - point_to_interpolate.z)) * inverse_border_size_squared;
	return top_left_part * normales[top_left_index] + top_right_part * normales[top_right_index] + bottom_right_part * normales[bottom_right_index] + bottom_left_part * normales[bottom_left_index];
}

bool operator<(const Chunk& first, const Chunk& other)
{
	return false;
}
