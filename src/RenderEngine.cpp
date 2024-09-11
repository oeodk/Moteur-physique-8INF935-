#include "RenderEngine.h"
#include <ofMain.h>
#include <of3dGraphics.h>

#include "Vector3D.h"
#include "Drawable.h"

constexpr float HEIGH_FACTOR = 5;
constexpr unsigned char WATER_LEVEL = 80;
constexpr unsigned char STONE_LEVEL = 130;
constexpr unsigned char SNOW_LEVEL = 180;


RenderEngine::RenderEngine()
{
	ofAddListener(ofEvents().mouseMoved, this, &RenderEngine::mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &RenderEngine::mouseDragged);
	ofAddListener(ofEvents().keyPressed, this, &RenderEngine::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &RenderEngine::keyReleased);

	_light_source.setDirectional();
	_light_source.rotate(45, 1, 0, 0);

	_camera.setFov(90);
	_camera.setPosition(0, 225, 100);

	_camera_movement.insert({ _FORWARD_KEY   , false });
	_camera_movement.insert({ _BACKWARD_KEY , false });
	_camera_movement.insert({ _RIGHT_KEY, false });
	_camera_movement.insert({ _LEFT_KEY , false });
	_camera_movement.insert({ _UP_KEY , false });
	_camera_movement.insert({ _DOWN_KEY , false });
	_camera_movement.insert({ _SRINT_KEY , false });

	_old_mouse_x = -1;
	_old_mouse_y = -1;
	_mouse_x = -1;
	_mouse_y = -1;

	ofImage noise;
	std::cout<<noise.load("noise_4096x4096.jpg");
	int noise_width = noise.getWidth();
	
	const ofColor COLOR_TABLE[4] = { ofColor(42, 104, 134), ofColor(63, 155, 11), ofColor(137, 125, 107), ofColor(255, 200, 200) };

	for (int i = 0; i < _TERRAIN_DIVISION; i++)
	{
		for (int j = 0; j < _TERRAIN_DIVISION; j++)
		{
			unsigned char color = noise.getColor(j * (noise_width / _TERRAIN_DIVISION) + i * noise_width).r;
			if (color < WATER_LEVEL)
			{
				color = WATER_LEVEL;
			}
			_terrain.addVertex(Vector3D(-(_TERRAIN_SIZE / 2) + _TERRAIN_SIZE * (i / static_cast<float>(_TERRAIN_DIVISION)), color * color / 50.f, -(_TERRAIN_SIZE / 2) + _TERRAIN_SIZE * (j / static_cast<float>(_TERRAIN_DIVISION))));
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
				_terrain.addColor(COLOR_TABLE[color_index]);
			}
		}
	}

	auto vertices = _terrain.getVertices();

	for (int i = 0; i < _TERRAIN_DIVISION - 1; i++)
	{
		for (int j = 0; j < _TERRAIN_DIVISION - 1; j++)
		{
			int current = i * (_TERRAIN_DIVISION) + j;
			int next = current + (_TERRAIN_DIVISION);

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

RenderEngine::~RenderEngine()
{
	ofRemoveListener(ofEvents().mouseMoved, this, &RenderEngine::mouseMoved);
	ofRemoveListener(ofEvents().keyPressed, this, &RenderEngine::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &RenderEngine::keyReleased);
	ofRemoveListener(ofEvents().mouseDragged, this, &RenderEngine::mouseDragged);
}

void RenderEngine::mouseMoved(ofMouseEventArgs& mouse)
{
	if (_old_mouse_x == -1)
	{
		_mouse_x = mouse.x;
		_old_mouse_x = mouse.x;
	}
	else
	{
		_mouse_x = mouse.x;
	}

	if (_old_mouse_y == -1)
	{
		_mouse_y = mouse.y;
		_old_mouse_y = mouse.y;
	}
	else
	{
		_mouse_y = mouse.y;
	}

}

void RenderEngine::mouseDragged(ofMouseEventArgs& mouse)
{
	_mouse_x = mouse.x;
	_old_mouse_x = mouse.x;

	_mouse_y = mouse.y;
	_old_mouse_y = mouse.y;
}

void RenderEngine::update(float delta_t)
{
	if (_old_mouse_x != _mouse_x)
	{
		_camera.rotate((_old_mouse_x - _mouse_x) / _MOUSE_SENSIBILITY, 0.f, 1.f, 0.f);
		_old_mouse_x = _mouse_x;
	}
	if (_old_mouse_y != _mouse_y)
	{
		_camera.rotate((_old_mouse_y - _mouse_y) / _MOUSE_SENSIBILITY, _camera.getSideDir());
		_old_mouse_y = _mouse_y;
	}

	const float SPEED = _CAMERA_SPEED * delta_t;
	Vector3D movement;
	if (_camera_movement.at(_FORWARD_KEY))
	{
		Vector3D camera_direction(_camera.getLookAtDir());
		camera_direction.y = 0;
		camera_direction.normalize();
		movement += camera_direction;
	}
	if (_camera_movement.at(_BACKWARD_KEY))
	{
		Vector3D camera_direction(_camera.getLookAtDir());
		camera_direction.y = 0;
		camera_direction.normalize();
		movement -= camera_direction;
	}
	if (_camera_movement.at(_RIGHT_KEY))
	{
		movement += _camera.getSideDir();
	}
	if (_camera_movement.at(_LEFT_KEY))
	{
		movement -= _camera.getSideDir();
	}
	//Normalizing movement here would make the speed constant in all directions
	if (_camera_movement.at(_UP_KEY))
	{
		movement += Vector3D(0.f, 1.f, 0.f);;
	}
	if (_camera_movement.at(_DOWN_KEY))
	{
		movement -= Vector3D(0.f, 1.f, 0.f);;
	}
	if (movement != constants::EMPTY_VECTOR3D)
	{
		//movement.normalize();
		if (_camera_movement.at(_SRINT_KEY))
		{
			movement *= 5;
		}
		movement *= SPEED;
		_camera.move(movement);
	}
}

void RenderEngine::render()
{
	ofBackgroundGradient(ofColor(0, 200, 255), ofColor(0, 150, 205));

	_camera.begin();
	ofEnableDepthTest();

	for (Drawable* render_target : _render_targets_no_light)
	{
		render_target->draw();
	}

	_light_source.enable();

	/* TO DO : create a World class that manage the environment*/
	ofSetColor(ofColor::red);
	ofDrawBox(Vector3D(50, 200, 0), 10);
	ofSetColor(ofColor::green);
	ofDrawBox(Vector3D(0, 250, 0), 10);
	ofSetColor(ofColor::blue);
	ofDrawBox(Vector3D(0, 200, 50), 10);
	ofSetColor(ofColor::purple);
	ofDrawBox(Vector3D(0, 200, 0), 10);

	for (Drawable* render_target : _render_targets)
	{
		render_target->draw();
	}

	_terrain.draw();

	_camera.end();
	_light_source.disable();
	ofDisableLighting();
	ofDisableDepthTest();
}

void RenderEngine::keyPressed(ofKeyEventArgs& key)
{
	auto found = _camera_movement.find(key.key);
	if (found != _camera_movement.end())
	{
		_camera_movement.at(key.key) = true;
	}
}

void RenderEngine::keyReleased(ofKeyEventArgs& key)
{
	auto found = _camera_movement.find(key.key);
	if (found != _camera_movement.end())
	{
		_camera_movement.at(key.key) = false;
	}
}

void RenderEngine::addRenderTarget(Drawable* render_target, bool use_light)
{
	if (use_light)
	{
		_render_targets.push_back(render_target);
	}
	else
	{
		_render_targets_no_light.push_back(render_target);
	}
}
