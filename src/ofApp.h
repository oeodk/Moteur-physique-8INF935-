#pragma once
#include "ofMain.h"
#include "RenderEngine.h"
#include "PhysicsEngine.h"
#include "Terrain.h"
#include "GlobalConstants.h"
#include <vector>


class ofApp : public ofBaseApp
{
private :
	std::vector<class Particle*> _particles;

	PhysicsEngine _physics_engine;
	RenderEngine _render_engine;

	double _elapsed_time;
	double _dt;

	ofxPanel gui_;
	ofxLabel label_dt_;
	ofxLabel label_fps_;
	ofxGuiGroup frame_information_, particles_selection_group_, shoot_group_;
	std::array<ofxToggle, 4> particles_selection_toggle_buttons_;
	std::array<ofParameter<bool>, 4> particles_selection_parameters_;
	unsigned char selected_particle_;
	std::array< ofParameter<int>, 2> shoot_angles_;
	ofxButton shoot_button_;

	Terrain _terrain;

public:
	void setup();
	void update();
	void draw();
	void exit();

	void shootProjectile();
	void updateSelectedParticle(bool& state);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void spawnParticle(BulletType type = BULLET);
};
