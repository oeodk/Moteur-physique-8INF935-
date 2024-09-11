#pragma once
#include <ofxGui.h>
#include <ofParameter.h>
#include <ofCamera.h>
#include <ofLight.h>

#include <unordered_map>
#include <vector>

class Drawable;

class RenderEngine
{
private : 
	ofCamera _camera;
	ofLight _light_source;

	std::unordered_map<int, bool> _camera_movement;

	std::vector<Drawable*> _render_targets;
	std::vector<Drawable*> _render_targets_no_light;

	static constexpr float _CAMERA_SPEED = 200.f;
	static constexpr float _MOUSE_SENSIBILITY = 5.f;

	static constexpr int _FORWARD_KEY    = 'z';// ofKey::OF_KEY_UP;
	static constexpr int _BACKWARD_KEY   = 's';//ofKey::OF_KEY_DOWN;
	static constexpr int _RIGHT_KEY		 = 'd';//ofKey::OF_KEY_RIGHT;
	static constexpr int _LEFT_KEY		 = 'q';//ofKey::OF_KEY_LEFT;
	static constexpr int _UP_KEY		 = ' ';//ofKey::OF_KEY_LEFT;
	static constexpr int _DOWN_KEY		 = ofKey::OF_KEY_LEFT_SHIFT;
	static constexpr int _SRINT_KEY		 = ofKey::OF_KEY_LEFT_CONTROL;

	int _old_mouse_x, _old_mouse_y, _mouse_x, _mouse_y;

	static constexpr int _TERRAIN_SIZE = 5000;
	static constexpr int _TERRAIN_DIVISION = 1024;
	ofVboMesh  _terrain;
public : 
	RenderEngine();
	RenderEngine(const RenderEngine&) = delete;
	RenderEngine(RenderEngine&&) = delete;
	~RenderEngine();

	void mouseMoved(ofMouseEventArgs& mouse);
	void mouseDragged(ofMouseEventArgs& mouse);

	void update(float delta_t);
	void render();

	void keyPressed(ofKeyEventArgs& key);
	void keyReleased(ofKeyEventArgs& key);

	void addRenderTarget(Drawable * render_target, bool use_light = true);
};

