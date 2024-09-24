#pragma once
#include "ofMain.h"
#include "RenderEngine.h"
#include "PhysicsEngine.h"
#include "gui/GUIManager.h"
#include "Terrain.h"
#include <vector>

class ofApp : public ofBaseApp
{
private :
	std::vector<class Particle*> _particles;

	PhysicsEngine _physics_engine;
	RenderEngine _render_engine;
	GUIManager _gui_manager;

	double _elapsed_time;
	double _dt;

	unsigned char _selected_particle;

	Terrain _terrain;

public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y, float scrollX, float scrollY);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
};
