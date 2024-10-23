#pragma once

#include "ofMain.h"
#include "ofImage.h"
#include "../GlobalConstants.h"

class GUIManager {
protected:

	static const int SLOT_SIZE = 60;
	static const int SLOT_PADDING = 6;

	std::vector<BulletType> _particle_types;
	unsigned char _nb_of_slots;
	unsigned char _current_slot = 0;

	std::vector<ofImage> _slot_images;
	std::vector<ofImage> _slot_icon_images;
	ofImage _slot_select;

	float _fps;
	float _dt;
	
	// HUD Oscillation
	int _blob_particle_count;		// -1 if there is no blob
	const float GUI_OSCILLATION_FREQUENCY = 10.f;
	const float GUI_OSCILLATION_DAMPING_RATIO = .9f;
	const float GUI_OSCILLATION_AMPLITUDE = 40.f;
	float _gui_oscillation_t;
	float _gui_particle_count_height;

public:
	void setup(const std::vector<BulletType>& particle_types);
	void update(float const dt, unsigned char const selected_particle);
	void draw();

	std::string getDisplayName(BulletType bullet_type);
	std::string getIconPath(BulletType bullet_type);

	void setupSlots();
	void drawSlots();

	void drawFps();
	void drawBlobParticleCount();
	void updateBlobParticleCountTextHeight(float const dt);
	void setBlobParticleCount(int count);
	void drawInstructions();

	int getSelectedSlot() { return _current_slot; }
	void setSelectedSlot(const int slot) { _current_slot = slot; }
};

