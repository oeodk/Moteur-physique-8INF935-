#pragma once
//#define DEBUG_CAMERA

#ifdef DEBUG_CAMERA
#include <ofEasyCam.h>
#endif

#include <unordered_map>
#include <vector>

#include <ofParameter.h>
#include <ofCamera.h>
#include <ofLight.h>
#include <of3dPrimitives.h>

#include "GlobalConstants.h"
#include "Vector3D.h"
#include <ofVboMesh.h>


class Drawable;

class RenderEngine {
private:
	ofCamera _camera;
	ofLight _light_source, _light_source_weapon, _night_light_source;

	std::unordered_map<int, bool> _camera_movement;

	std::vector<Drawable*> _render_targets;
	std::vector<Drawable*> _render_targets_no_light;

	static constexpr float _CAMERA_SPEED = 100;//100
	static constexpr float _CAMERA_SPRINT_VALUE = 5;//5
	static constexpr float _MOUSE_SENSIBILITY = 5.f;

	static constexpr int _FORWARD_KEY = 'z';
	static constexpr int _FORWARD_KEY_ALT = 'w';
	static constexpr int _BACKWARD_KEY = 's';
	static constexpr int _RIGHT_KEY = 'd';
	static constexpr int _LEFT_KEY = 'q';
	static constexpr int _LEFT_KEY_ALT = 'a';
	static constexpr int _UP_KEY = ' ';
	static constexpr int _DOWN_KEY = ofKey::OF_KEY_LEFT_SHIFT;
	static constexpr int _SRINT_KEY = ofKey::OF_KEY_LEFT_CONTROL;

	int _old_mouse_x, _old_mouse_y, _mouse_x, _mouse_y;

	ofColor _background_color;
	float _simulation_total_time;
	// Test if a render target is in front of the camera
	bool willRender(Vector3D target_position) const;

#ifdef DEBUG_CAMERA
	ofEasyCam _debug_camera;
#endif
	ofCylinderPrimitive _cannon;
	ofColor _cannon_color;

	// Remove the key modifier (ex : Ctrl + Z => Z)
	int removeModifier(ofKeyEventArgs& key);

public:
	RenderEngine();
	RenderEngine(const RenderEngine&) = delete;
	RenderEngine(RenderEngine&&) = delete;
	~RenderEngine();

	void mouseMoved(ofMouseEventArgs& mouse);
	/*
		The event mouseDragged is triggered when the mouse button is pressed and the mouse is moving
		With this event the user is able move his mouse on the screen without moving the camera
	*/
	void mouseDragged(ofMouseEventArgs& mouse);

	void update(float delta_t);
	void render();

	void keyPressed(ofKeyEventArgs& key);
	void keyReleased(ofKeyEventArgs& key);
	void windowResized(int w, int h);

	void addRenderTarget(Drawable* render_target, bool use_light = true);

	void setCameraPosition(const Vector3D& new_position);
	Vector3D getCameraPosition() const { return Vector3D(_camera.getPosition()); }
	const ofCamera& getCamera() const { return _camera; }

	float getFarPlane() const { return _camera.getFarClip(); }

	void setWeaponColor(const ofColor& new_color) { _cannon_color = new_color; }

	void updateSelectedParticle(BulletType bullet);
};