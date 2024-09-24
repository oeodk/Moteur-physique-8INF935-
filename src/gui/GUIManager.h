#pragma once

#include "ofMain.h"
#include "ofImage.h"

class GUIManager
{
protected:
	static const int NB_OF_SLOTS = 4;
	static const int SLOT_SIZE = 60;
	static const int SLOT_PADDING = 6;

	int _currentSlot = 0;

	ofImage _slot_images[NB_OF_SLOTS];
	ofImage _slot_icon_images[NB_OF_SLOTS];
	ofImage _slot_select;

	float _fps;
	float _dt;

public:
	void setup();
	void update(float const dt);
	void draw();

	void setupSlots();
	void drawSlots();

	void drawFps();

	void onMouseScrolled(const float scroll);

	int getSelectedSlot();
	void setSelectedSlot(const int slot);
};

