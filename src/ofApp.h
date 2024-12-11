#pragma once
#define RIGID_BODY_ONLY

#include "ofMain.h"
#include "forces/ParticleForceRegistry.h"
#include "RenderEngine.h"
#include "PhysicsEngine.h"
#include "gui/GUIManager.h"
#include "Terrain.h"
#include "GlobalConstants.h"
#include "particles/Blob.h"
#include "ocTree.h"

#include <vector>

class ofApp : public ofBaseApp {
private:
	inline static std::vector<class Particle*> _particles;
	inline static std::vector<class RigidBody*> _rigid_body;
	std::vector<BulletType> _particle_types = {
		BulletType::CHICKEN,
		BulletType::MASAMUNE,
		BulletType::SEPHIROTH,
		BulletType::GOAT,
		BulletType::STATIC_CUBE,
		BulletType::MOVING_CUBE
	};
	ocTree _particles_octree;

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

	bool _do_simulation = true;
	bool _show_octree = true;
public:
	inline static bool _show_hitbox = true;
	
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
	void spawnRigidBody(BulletType type = BULLET);
	void moveBlobs();

	static void addParticle(Particle* particle) { _particles.push_back(particle); }
	static void addRigidBody(RigidBody* particle) { _rigid_body.push_back(particle); }
};
