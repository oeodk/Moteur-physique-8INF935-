#include "RenderEngine.h"
#include <of3dGraphics.h>

#include "Vector3D.h"
#include "Drawable.h"


RenderEngine::RenderEngine()
{
	ofAddListener(ofEvents().mouseMoved, this, &RenderEngine::mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &RenderEngine::mouseDragged);
	ofAddListener(ofEvents().keyPressed, this, &RenderEngine::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &RenderEngine::keyReleased);

	_light_source.setDirectional();
	_light_source.rotate(45, 1, 0, 0);

	_camera.setFov(70);
	_camera.setFarClip(2000);
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

#ifndef DEBUG_CAMERA
	// Enable fog
	glEnable(GL_FOG);

	// Set fog mode (GL_LINEAR for smooth linear fog)
	glFogi(GL_FOG_MODE, GL_LINEAR);

	// Set fog color
	GLfloat fogColor[] = { 0, 0.68, 1, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);

	// Set start and end distances for fog
	glFogf(GL_FOG_START, 1800);  // Start applying fog at 200 units
	glFogf(GL_FOG_END, 2000);    // Fully apply fog at 600 units

	// Optional: Set fog density (if using GL_EXP or GL_EXP2)
	// glFogf(GL_FOG_DENSITY, 0.35);
#endif
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
	//ofBackground(ofColor(0, 200, 255));
#ifdef DEBUG_CAMERA
	_debug_camera.begin();
	ofSetColor(ofColor::purple);
	_camera.drawFrustum();
#else
	_camera.begin();
#endif // DEBUG_CAMERA

	ofEnableDepthTest();
	ofEnableAlphaBlending();

	for (Drawable* render_target : _render_targets_no_light)
	{
		if(willRender(*render_target->getPosition()))
		{
			render_target->draw();
		}
	}

	_light_source.enable();
	for (Drawable* render_target : _render_targets)
	{
		if (willRender(*render_target->getPosition()))
		{
			render_target->draw();

		}
	}
#ifdef DEBUG_CAMERA
	_debug_camera.end();
#else
	_camera.end();
#endif // DEBUG_CAMERA
	_light_source.disable();
	ofDisableLighting();
	ofDisableDepthTest();
	ofDisableAlphaBlending();

	_render_targets_no_light.clear();
	_render_targets.clear();
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

void RenderEngine::setCameraPosition(const Vector3D& new_position)
{
	_camera.setPosition(new_position);
}


bool RenderEngine::willRender(const Vector3D& target_position) const
{
	Vector3D look_at_dir(_camera.getLookAtDir());
	Vector3D camera_target_vector(target_position - _camera.getPosition());
	//look_at_dir.y = 0;
	//camera_target_vector.y = 0;
	camera_target_vector.normalize();
	//std::cout << std::abs(Vector3D::dotProduct(look_at_dir, camera_target_vector)) << std::endl;
	if (Vector3D::dotProduct(look_at_dir, camera_target_vector) > 0.25)
	{
		return true;
	}
	return false;
}