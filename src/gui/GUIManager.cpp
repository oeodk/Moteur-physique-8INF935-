#include "GUIManager.h"

void GUIManager::setup() {
	setupSlots();
}

void GUIManager::draw() {
	ofSetColor(255);
	ofEnableAlphaBlending();
	
	drawFps();
	drawSlots();
	
	ofDisableAlphaBlending();
}

void GUIManager::setupSlots() {
	for (int i = 0; i < NB_OF_SLOTS; i++) {
		slot_images[i].load("ui_slot.png");
		slot_icon_images[i].load("particle_bullet.png");
	}
	slot_select.load("ui_slot_select.png");
}

void GUIManager::drawSlots() {
	int screenHeight = ofGetHeight();

	int drawHeight = screenHeight / 2 - 2 * (SLOT_SIZE + SLOT_PADDING);
	for (int i = 0; i < NB_OF_SLOTS; i++) {
		slot_images[i].draw(20, drawHeight, SLOT_SIZE, SLOT_SIZE);
		slot_icon_images[i].draw(30, drawHeight + 10, SLOT_SIZE - 20, SLOT_SIZE - 20);

		if (currentSlot == i) {
			slot_select.draw(14, drawHeight - 6, SLOT_SIZE + 12, SLOT_SIZE + 12);
			ofDrawBitmapString("Bullet name", 20 + SLOT_SIZE + 12, drawHeight + SLOT_SIZE / 2);
		}

		drawHeight += (SLOT_SIZE + SLOT_PADDING);
	}
}

void GUIManager::drawFps() {
	stringstream fspStringStream;
	fspStringStream << std::fixed << std::setprecision(1)
		<< ofGetFrameRate() << "fps "
		<< std::setprecision(3) << "(" << ofGetLastFrameTime() << "s)";

	ofDrawBitmapString(fspStringStream.str(), 20, 20);
}

void GUIManager::onMouseScrolled(const float scroll) {
	if (scroll > 0)
		currentSlot = currentSlot == 0 ? NB_OF_SLOTS - 1 : currentSlot - 1;
	else if (scroll < 0)
		currentSlot = currentSlot == NB_OF_SLOTS - 1 ? 0 : currentSlot + 1;
}

int GUIManager::getSelectedSlot() { return currentSlot; }
void GUIManager::setSelectedSlot(const int slot) { currentSlot = slot; }