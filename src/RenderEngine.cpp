#include "RenderEngine.h"
#include <of3dGraphics.h>

#include "Vector3D.h"
#include "Drawable.h"

constexpr int RENDER_DISTANCE = 5000;

RenderEngine::RenderEngine()
{
	ofAddListener(ofEvents().mouseMoved, this, &RenderEngine::mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &RenderEngine::mouseDragged);
	ofAddListener(ofEvents().keyPressed, this, &RenderEngine::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &RenderEngine::keyReleased);
	ofAddListener(ofEvents().windowResized, this, &RenderEngine::windowResized);

	_light_source.setDirectional();
	_light_source.rotate(45, 1, 0, 0);
	_light_source_weapon.setDirectional();
	_light_source_weapon.rotate(-45, 1, 0, 0);

	_camera.setFov(70);
	_camera.setFarClip(RENDER_DISTANCE);
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
	glFogf(GL_FOG_START, RENDER_DISTANCE * 0.9);  // Start applying fog at 200 units
	glFogf(GL_FOG_END, RENDER_DISTANCE);    // Fully apply fog at 600 units

	// Optional: Set fog density (if using GL_EXP or GL_EXP2)
	// glFogf(GL_FOG_DENSITY, 0.35);
#endif

	_test.addVertex(Vector3D(0, 1000, 0));
	_test.addVertex(Vector3D(10, 1000, 10));
	_test.addVertex(Vector3D(-10, 1000, 10));
	_test.addVertex(Vector3D(10, 1000, -10));
	_test.addVertex(Vector3D(-10, 1000, -10));

	_test.addIndex(0);
	_test.addIndex(1);
	_test.addIndex(2);
	_test.addIndex(3);
	_test.addIndex(4);

	_test.setMode(OF_PRIMITIVE_POINTS);

	_cannon.set(1, 20, 20, 2);
	_cannon.rotateDeg(90, 1, 0, 0);


	_cannon.setPosition(ofGetWidth() * 0.9, ofGetHeight() * 0.9, 0);
	_cannon.setScale(100);
	_cannon_color = ofColor(65, 104, 74);
	//_cannon.col(ofColor::black);
	//_cannon.setPosition(20, 0, 0);
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
	Vector3D side_dir(_camera.getSideDir());
	Vector3D look_at_dir(_camera.getLookAtDir());
	bool update_cannon_position = false;

	if (_old_mouse_x != _mouse_x)
	{
		_camera.rotate((_old_mouse_x - _mouse_x) / _MOUSE_SENSIBILITY, 0.f, 1.f, 0.f);
		update_cannon_position = true;
		_camera.lookAt(_camera.getPosition() + _camera.getLookAtDir(), Vector3D(0, 1, 0));
		_old_mouse_x = _mouse_x;
	}
	if (_old_mouse_y != _mouse_y)
	{
		if(Vector3D::dotProduct(_camera.getLookAtDir(), Vector3D(0, 1, 0)) > -0.9 || (_old_mouse_y - _mouse_y)> 0)
		{
			_camera.rotate((_old_mouse_y - _mouse_y) / _MOUSE_SENSIBILITY, side_dir);
			_camera.lookAt(_camera.getPosition() + _camera.getLookAtDir(), Vector3D(0, 1, 0));
			update_cannon_position = true;
		}
		_old_mouse_y = _mouse_y;
	}

	const float SPEED = _CAMERA_SPEED * delta_t;
	Vector3D movement;
	if (_camera_movement.at(_FORWARD_KEY))
	{
		Vector3D camera_direction(look_at_dir);
		camera_direction.y = 0;
		camera_direction.normalize();
		movement += camera_direction;
	}
	if (_camera_movement.at(_BACKWARD_KEY))
	{
		Vector3D camera_direction(look_at_dir);
		camera_direction.y = 0;
		camera_direction.normalize();
		movement -= camera_direction;
	}
	if (_camera_movement.at(_RIGHT_KEY))
	{
		movement += side_dir;
	}
	if (_camera_movement.at(_LEFT_KEY))
	{
		movement -= side_dir;
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
		update_cannon_position = true;
	}
	if(update_cannon_position)
	{
		//_cannon.setPosition(_camera.getPosition() + side_dir * 6 + down_dir * 7 + look_at_dir * 5);
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

	//ofSetColor(ofColor::red);
	//glPointSize(10);
	//_test.draw();
	
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

	_light_source_weapon.enable();
	ofSetColor(_cannon_color);
	_cannon.draw();
	_light_source_weapon.disable();
	
	ofDisableDepthTest();
	ofDisableLighting();
	ofDisableAlphaBlending();

	_render_targets_no_light.clear();
	_render_targets.clear();
}

void RenderEngine::keyPressed(ofKeyEventArgs& key)
{
	int k = removeModifier(key);

	auto found = _camera_movement.find(k);
	//printf("key pressed");
	if (found != _camera_movement.end())
	{
		_camera_movement.at(k) = true;
	}
}

void RenderEngine::keyReleased(ofKeyEventArgs& key)
{
	int k = removeModifier(key);

	auto found = _camera_movement.find(k);
	if (found != _camera_movement.end())
	{
		_camera_movement.at(k) = false;
	}
}

void RenderEngine::windowResized(ofResizeEventArgs& event)
{
	_cannon.setPosition(ofGetWidth() * 0.9, ofGetHeight() * 0.9, 0);
}

int RenderEngine::removeModifier(ofKeyEventArgs& key)
{
	int k = key.key;
	if(key.key != ' ' && key.key != ofKey::OF_KEY_LEFT_CONTROL && key.key != ofKey::OF_KEY_LEFT_SHIFT)
	{
		if (key.hasModifier(2))
		{
			k += 96;
		}
		if (key.hasModifier(1))
		{
			k += 32;
		}
	}
	return k;
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
	Vector3D side_dir(_camera.getSideDir());
	Vector3D look_at_dir(_camera.getLookAtDir());
	Vector3D down_dir(Vector3D::crossProduct(look_at_dir, side_dir));
	//_cannon.setPosition(_camera.getPosition() + side_dir * 6 + down_dir * 7 + look_at_dir * 5);
}


bool RenderEngine::willRender(Vector3D target_position) const
{
	Vector3D look_at_dir(_camera.getLookAtDir());
	Vector3D camera_target_vector(target_position - _camera.getPosition());
	camera_target_vector.normalize();
	if (Vector3D::dotProduct(look_at_dir, camera_target_vector) > 0.25)
	{
		return true;
	}
	return false;
}