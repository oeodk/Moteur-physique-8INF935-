#include "Chunk.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>
#include <chrono>
#include <thread>
using namespace std::chrono_literals;

constexpr float HEIGH_FACTOR = 5;
constexpr unsigned char WATER_LEVEL = 80;
constexpr unsigned char STONE_LEVEL = 130;
constexpr unsigned char SNOW_LEVEL = 190;

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
		_trees_primitive[0].set(TREE_RADIUS, -TREE_RADIUS * 3, 3, 1, 2, OF_PRIMITIVE_TRIANGLES);
		_trees_primitive[1].set(TREE_RADIUS, -TREE_RADIUS * 3, 10, 1, 2, OF_PRIMITIVE_TRIANGLES);
		_trees_primitive[2].set(TREE_RADIUS, -TREE_RADIUS * 3, 20, 1, 2, OF_PRIMITIVE_TRIANGLES);
	}
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
	_spatial_coordinate.x = chunk_size * grid_coordinate.x;// *((chunk_division - CHUNK_DIV_OFFSET) / static_cast<float>(chunk_division));
	_spatial_coordinate.y = 0;
	_spatial_coordinate.z = chunk_size * grid_coordinate.z;// *((chunk_division - CHUNK_DIV_OFFSET) / static_cast<float>(chunk_division));

	int noise_offset_x = grid_coordinate.x * (_chunk_division);// - CHUNK_DIV_OFFSET);
	int noise_offset_y = grid_coordinate.z * (_chunk_division);// -CHUNK_DIV_OFFSET);

	const ofColor COLOR_TABLE[4] = { ofColor(42, 104, 134), ofColor(63, 155, 11), ofColor(137, 125, 107), ofColor(255, 200, 200) };
	_chunk_division += 1;
	for (int i = 0; i < _chunk_division; i++) {
		for (int j = 0; j < _chunk_division; j++) {
			float height = terrainNoise(seed + (noise_offset_x + i) * NOISE_SCALE, seed + (noise_offset_y + j) * NOISE_SCALE, OCTAVES, PERSISTANCE, LACUNARITY);
			float height_squared = (height + 0.5) * (height + 0.5);

			height *= 255;
			if (height < WATER_LEVEL) {
				height = WATER_LEVEL;
			}
			_terrain.addVertex(Vector3D(_spatial_coordinate.x + chunk_size * (i / static_cast<float>(_chunk_division - 1)), height * height * height / 5000.f /*height * height / 50.f*/, _spatial_coordinate.z + chunk_size * (j / static_cast<float>(_chunk_division - 1))));
			if (height == WATER_LEVEL) {
				_terrain.addColor(COLOR_TABLE[0]);
			}
			else {
				char color_index = 1;

				height += (rand() % 30 - 15);
				if (height > STONE_LEVEL) {
					color_index++;
					if (height > SNOW_LEVEL) {
						color_index++;
					}
				}
				else {
					Vector3D tree_pos = _terrain.getVertex(_terrain.getNumVertices() - 1);
					tree_pos.x += rand() % 20 - 10;
					tree_pos.z += rand() % 20 - 10;
					tree_pos.y += TREE_RADIUS;
					float tree_noise = ofNoise(seed + (noise_offset_x + i) * NOISE_SCALE * 2 + 1, seed + (noise_offset_y + j) * NOISE_SCALE * 2 + 1);
					if (tree_noise > 0.8) {
						_trees_positions.push_back(tree_pos);
					}
				}
				_terrain.addColor(COLOR_TABLE[color_index]);
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

void Chunk::draw() {
	_terrain.draw();
	ofSetColor(ofColor(65, 104, 74));
	if (_can_edit_tree) {
		_can_edit_tree = false;
		if (_trees_buffer[_used_tree_buffer] != nullptr) {
			_trees_buffer[_used_tree_buffer]->draw();
		}
		_can_edit_tree = true;
	}
}

void Chunk::update(float player_distance) {
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
		for (const auto& v : _trees_primitive[primitive_index].getMesh().getVertices()) {
			chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->addVertex(v + tree);
		}

		for (const auto& n : _trees_primitive[primitive_index].getMesh().getNormals()) {
			chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->addNormal(-n);
		}

		int base_index = chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->getNumVertices();
		for (const auto& ind : _trees_primitive[primitive_index].getMesh().getIndices()) {
			chunk->_trees_buffer[1 - chunk->_used_tree_buffer]->addIndex(ind + base_index);
		}
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

