#include "RenderEngine.h"
#include <of3dGraphics.h>
#include <ofGraphics.h>

#include "Vector3D.h"
#include "Drawable.h"

constexpr int RENDER_DISTANCE = 5000;

RenderEngine::RenderEngine() {
	ofAddListener(ofEvents().mouseMoved, this, &RenderEngine::mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &RenderEngine::mouseDragged);
	ofAddListener(ofEvents().keyPressed, this, &RenderEngine::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &RenderEngine::keyReleased);

	_light_source.setDirectional();
	_light_source.rotate(45, 1, 0, 0);

	_light_source_weapon.setDirectional();
	_light_source_weapon.rotate(-45, 1, 0, 0);

	_camera.setFov(70);
	_camera.setFarClip(RENDER_DISTANCE);
	_camera.setPosition(0, 225, 100);

	_camera_movement.insert({ _FORWARD_KEY   , false });
	_camera_movement.insert({ _FORWARD_KEY_ALT   , false });
	_camera_movement.insert({ _BACKWARD_KEY , false });
	_camera_movement.insert({ _RIGHT_KEY, false });
	_camera_movement.insert({ _LEFT_KEY , false });
	_camera_movement.insert({ _LEFT_KEY_ALT , false });
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
	glFogi(GL_FOG_MODE, GL_LINEAR);

	// Set fog color
	GLfloat fogColor[] = { 0, 0.68, 1, 1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);

	// Set start and end distances for fog
	glFogf(GL_FOG_START, RENDER_DISTANCE * 0.9);
	glFogf(GL_FOG_END, RENDER_DISTANCE);

#endif

	_cannon.set(1, 20, 20, 2);
	_cannon.rotateDeg(90, 1, 0, 0);

	_cannon.setPosition(ofGetWidth() * 0.9, ofGetHeight() * 0.9, 0);
	_cannon.setScale(100);
	_cannon_color = ofColor(204, 153, 0);

	glPointSize(5);
	glLineWidth(5);
}

RenderEngine::~RenderEngine() {
	ofRemoveListener(ofEvents().mouseMoved, this, &RenderEngine::mouseMoved);
	ofRemoveListener(ofEvents().keyPressed, this, &RenderEngine::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &RenderEngine::keyReleased);
	ofRemoveListener(ofEvents().mouseDragged, this, &RenderEngine::mouseDragged);
}

void RenderEngine::mouseMoved(ofMouseEventArgs& mouse) {
	if (_old_mouse_x == -1) {
		_mouse_x = mouse.x;
		_old_mouse_x = mouse.x;
	}
	else {
		_mouse_x = mouse.x;
	}

	if (_old_mouse_y == -1) {
		_mouse_y = mouse.y;
		_old_mouse_y = mouse.y;
	}
	else {
		_mouse_y = mouse.y;
	}
}

void RenderEngine::mouseDragged(ofMouseEventArgs& mouse) {
	_mouse_x = mouse.x;
	_old_mouse_x = mouse.x;

	_mouse_y = mouse.y;
	_old_mouse_y = mouse.y;
}

void RenderEngine::update(float delta_t) {
	Vector3D side_dir(_camera.getSideDir());
	Vector3D look_at_dir(_camera.getLookAtDir());

	// Update camera rotation
	if (_old_mouse_x != _mouse_x) {
		_camera.rotate((_old_mouse_x - _mouse_x) / _MOUSE_SENSIBILITY, 0.f, 1.f, 0.f);
		_camera.lookAt(_camera.getPosition() + _camera.getLookAtDir(), Vector3D(0, 1, 0));
		_old_mouse_x = _mouse_x;
	}
	if (_old_mouse_y != _mouse_y) {
		if ((Vector3D::dotProduct(_camera.getLookAtDir(), Vector3D(0, 1, 0)) > -0.9 || (_old_mouse_y - _mouse_y) > 0) && (Vector3D::dotProduct(_camera.getLookAtDir(), Vector3D(0, 1, 0)) < 0.9 || (_old_mouse_y - _mouse_y) < 0)) {
			_camera.rotate((_old_mouse_y - _mouse_y) / _MOUSE_SENSIBILITY, side_dir);
			_camera.lookAt(_camera.getPosition() + _camera.getLookAtDir(), Vector3D(0, 1, 0));
		}
		_old_mouse_y = _mouse_y;
	}
	// Update camera position
	const float SPEED = _CAMERA_SPEED * delta_t;
	Vector3D movement;
	if (_camera_movement.at(_FORWARD_KEY) || _camera_movement.at(_FORWARD_KEY_ALT)) {
		Vector3D camera_direction(look_at_dir);
		camera_direction.y = 0;
		camera_direction.normalize();
		movement += camera_direction;
	}
	if (_camera_movement.at(_BACKWARD_KEY)) {
		Vector3D camera_direction(look_at_dir);
		camera_direction.y = 0;
		camera_direction.normalize();
		movement -= camera_direction;
	}
	if (_camera_movement.at(_RIGHT_KEY)) {
		movement += side_dir;
	}
	if (_camera_movement.at(_LEFT_KEY) || _camera_movement.at(_LEFT_KEY_ALT)) {
		movement -= side_dir;
	}
	if (_camera_movement.at(_UP_KEY)) {
		movement += Vector3D(0.f, 1.f, 0.f);;
	}
	if (_camera_movement.at(_DOWN_KEY)) {
		movement -= Vector3D(0.f, 1.f, 0.f);;
	}
	if (movement != constants::EMPTY_VECTOR3D) {
		if (_camera_movement.at(_SRINT_KEY)) {
			movement *= 5;
		}
		movement *= SPEED;
		_camera.move(movement);
	}
}

void RenderEngine::render() {
	ofBackgroundGradient(ofColor(0, 200, 255), ofColor(0, 150, 205));
	ofEnableDepthTest();
	ofEnableAlphaBlending();

#ifdef DEBUG_CAMERA
	_debug_camera.begin();
	ofSetColor(ofColor::purple);
	_camera.drawFrustum();
#else
	_camera.begin();
#endif // DEBUG_CAMERA

	for (Drawable* render_target : _render_targets_no_light) {
		if (willRender(*render_target->getPosition())) {
			render_target->drawNoLight();
		}
	}

	_light_source.enable();
	for (Drawable* render_target : _render_targets) {
		if (willRender(*render_target->getPosition())) {
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

void RenderEngine::keyPressed(ofKeyEventArgs& key) {
	int k = removeModifier(key);
	auto found = _camera_movement.find(k);

	if (found != _camera_movement.end()) {
		_camera_movement.at(k) = true;
	}
}

void RenderEngine::keyReleased(ofKeyEventArgs& key) {
	int k = removeModifier(key);
	auto found = _camera_movement.find(k);

	if (found != _camera_movement.end()) {
		_camera_movement.at(k) = false;
	}
}

void RenderEngine::windowResized(int w, int h) {
	_cannon.setPosition(w * 0.9, h * 0.9, 0);
}

int RenderEngine::removeModifier(ofKeyEventArgs& key) {
	int k = key.key;
	if (key.key != ' ' && key.key != ofKey::OF_KEY_LEFT_CONTROL && key.key != ofKey::OF_KEY_LEFT_SHIFT) {
		if (key.hasModifier(2)) {
			k += 96;
		}
		if (key.hasModifier(1)) {
			k += 32;
		}
	}
	return k;
}


void RenderEngine::addRenderTarget(Drawable* render_target, bool use_light) {
	if (use_light) {
		_render_targets.push_back(render_target);
	}
	else {
		_render_targets_no_light.push_back(render_target);
	}
}

void RenderEngine::setCameraPosition(const Vector3D& new_position) {
	_camera.setPosition(new_position);
}

void RenderEngine::updateSelectedParticle(BulletType bullet) {
	switch (bullet) {
	case BULLET:
		_cannon_color.set(204, 153, 0);
		break;
	case CANNONBALL:
		_cannon_color.set(60);
		break;
	case FIREBALL:
		_cannon_color.set(240, 60, 0);
		break;
	case BUBBLE:
		_cannon_color.set(30, 255, 255);
		break;
	default:
		break;
	}
}

bool RenderEngine::willRender(Vector3D target_position) const {
	Vector3D look_at_dir(_camera.getLookAtDir());
	Vector3D camera_target_vector(target_position - _camera.getPosition());
	camera_target_vector.normalize();
	if (Vector3D::dotProduct(look_at_dir, camera_target_vector) > 0.25) {
		return true;
	}
	return false;
}