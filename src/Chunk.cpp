#include "Chunk.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>

constexpr float HEIGH_FACTOR = 5;
constexpr unsigned char WATER_LEVEL = 80;
constexpr unsigned char STONE_LEVEL = 130;
constexpr unsigned char SNOW_LEVEL = 180;

constexpr float NOISE_SCALE = 0.004;  // Base scale for noise (0.005)
constexpr int OCTAVES = 10;  // Number of noise layers to combine
constexpr float PERSISTANCE = 0.4;  // How much amplitude decreases each octave
constexpr float LACUNARITY = 2.0;   // How much frequency increases each octave

constexpr float BUSH_RADIUS = 20;

void Chunk::setup(float chunk_size, int chunk_division, const Vector3D& grid_coordinate, int seed)
{
	srand(seed);
	_world_position = &_spatial_coordinate;
	_terrain.clear();
	_bushes.clear();
	_grid_coordinate = grid_coordinate;
	_spatial_coordinate.x = chunk_size * grid_coordinate.x * ((chunk_division - 3) / static_cast<float>(chunk_division));
	_spatial_coordinate.y = 0;
	_spatial_coordinate.z = chunk_size * grid_coordinate.z * ((chunk_division - 3) / static_cast<float>(chunk_division));

	int noise_offset_x = grid_coordinate.x * (chunk_division - 3);
	int noise_offset_y = grid_coordinate.z * (chunk_division - 3);

	const ofColor COLOR_TABLE[4] = { ofColor(42, 104, 134), ofColor(63, 155, 11), ofColor(137, 125, 107), ofColor(255, 200, 200) };

	for (int i = 0; i < chunk_division; i++)
	{
		for (int j = 0; j < chunk_division; j++)
		{
			float color = terrainNoise(seed + (noise_offset_x + i) * NOISE_SCALE, seed + (noise_offset_y + j) * NOISE_SCALE, OCTAVES, PERSISTANCE, LACUNARITY);
			float color_squared = (color + 0.5) * (color + 0.5);

			color *= 255;
			//unsigned char color = noise.getColor(j * (noise_width / _TERRAIN_DIVISION) + i * noise_width).r;
			if (color < WATER_LEVEL)
			{
				color = WATER_LEVEL;
			}
			_terrain.addVertex(Vector3D(_spatial_coordinate.x - (chunk_size / 2) + chunk_size * (i / static_cast<float>(chunk_division)), color * color / 50.f, _spatial_coordinate.z - (chunk_size / 2) + chunk_size * (j / static_cast<float>(chunk_division))));
			if (color == WATER_LEVEL)
			{
				_terrain.addColor(COLOR_TABLE[0]);
			}
			else
			{
				char color_index = 1;

				color += (rand() % 30 - 15);
				if (color > STONE_LEVEL)
				{
					color_index++;
					if (color > SNOW_LEVEL)
					{
						color_index++;
					}
				}
				else
				{
					Vector3D bush_pos = _terrain.getVertex(_terrain.getNumVertices() - 1);
					bush_pos.x += rand() % 20 - 10;
					bush_pos.z += rand() % 20 - 10;
					float bush_noise = ofNoise(seed + (noise_offset_x + i) * NOISE_SCALE * 2 + 1, seed + (noise_offset_y + j) * NOISE_SCALE * 2 + 1);
					if (bush_noise > 0.9)
					{
						_bushes.push_back(bush_pos);
					}
				}
				_terrain.addColor(COLOR_TABLE[color_index]);
			}
		}
	}

	auto vertices = _terrain.getVertices();

	for (int i = 0; i < chunk_division - 1; i++)
	{
		for (int j = 0; j < chunk_division - 1; j++)
		{
			int current = i * (chunk_division)+j;
			int next = current + (chunk_division);

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

	for (size_t i = 0; i < indices.size(); i += 3)
	{
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
	for (int i = 0; i < vertex_number; i++)
	{
		normals[i].normalize();
		normals[i].invert();
	}

	_terrain.clearNormals();
	for (int i = 0; i < vertex_number; i++)
	{
		_terrain.addNormal(normals[i]);
	}
}

void Chunk::draw()
{
	
	_terrain.draw();
	ofSetColor(ofColor(65, 104, 74));
	for (size_t i = 0; i < _bushes.size(); i++)
	{
		ofDrawCone(_bushes[i], BUSH_RADIUS, -BUSH_RADIUS * 3);
	}
	//ofSetColor(ofColor(255, 0, 255, 50));
	//ofNoFill();
	//ofDrawBox(_spatial_coordinate.x, 500, _spatial_coordinate.z, 640, 5000, 640);
}


float Chunk::terrainNoise(float x, float y, int octaves, float persistence, float lacunarity)
{
	float total = 0.0f;
	float amplitude = 1.0f;
	float frequency = 1.0f;
	float max_value = 0.0f;  // Used to normalize the result to the range [0, 1]

	for (int i = 0; i < octaves; i++)
	{
		total += ofNoise(x * frequency, y * frequency) * amplitude;

		max_value += amplitude;

		amplitude *= persistence;  // Decrease amplitude
		frequency *= lacunarity;   // Increase frequency
	}

	return total / max_value;  // Normalize the result
}


