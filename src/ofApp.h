#pragma once
#include "ofMain.h"
#include "forces/ParticleForceRegistry.h"
#include "RenderEngine.h"
#include "PhysicsEngine.h"
#include "gui/GUIManager.h"
#include "Terrain.h"
#include "GlobalConstants.h"
#include "particles/Blob.h"

#include <vector>

class ofApp : public ofBaseApp {
private:
	std::vector<class Particle*> _particles;
	std::vector<BulletType> _particle_types = {
		BulletType::BULLET,
		BulletType::CANNONBALL,
		BulletType::FIREBALL,
		BulletType::BUBBLE
	};

	ParticleForceRegistry _forces_registry;
	PhysicsEngine _physics_engine;
	RenderEngine _render_engine;
	GUIManager _gui_manager;

	double _elapsed_time;
	double _dt;

	unsigned char _selected_particle;

	Terrain _terrain;

	std::vector<Blob*> _blobs;
	std::map<ofKey, bool> _blob_key;
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y, float scrollX, float scrollY);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void spawnParticle(BulletType type = BULLET);
	void moveBlobs();
};
