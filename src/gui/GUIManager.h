#pragma once

#include "ofMain.h"
#include "ofImage.h"

class GUIManager
{
protected:
	static const int NB_OF_SLOTS = 4;
	static const int SLOT_SIZE = 60;
	static const int SLOT_PADDING = 6;

	int currentSlot = 0;

	ofImage slot_images[NB_OF_SLOTS];
	ofImage slot_icon_images[NB_OF_SLOTS];
	ofImage slot_select;

public:
	void setup();
	void draw();

	void setupSlots();
	void drawSlots();

	void drawFps();

	void onMouseScrolled(const float scroll);

	int getSelectedSlot();
	void setSelectedSlot(const int slot);
};

