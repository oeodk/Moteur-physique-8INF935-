#include "GUIManager.h"

void GUIManager::setup(const std::vector<BulletType>& particle_types) {
	_particle_types = particle_types;
	_nb_of_slots = _particle_types.size();
	setupSlots();
}

void GUIManager::update(float const dt, unsigned char const selected_particle) {
	_dt = dt;
	_fps = ofGetFrameRate();

	setSelectedSlot(selected_particle);
}

void GUIManager::draw() {
	ofSetColor(255);
	ofEnableAlphaBlending();

	drawFps();
	drawSlots();

	ofDisableAlphaBlending();
}

// Get a name to display based on the selected bullet type
std::string GUIManager::getDisplayName(BulletType bullet_type) {
	switch (bullet_type) {
	case BULLET: return "Balle de fusil";
	case CANNONBALL: return "Boulet de canon";
	case FIREBALL: return "Boule de feu";
	case BUBBLE: return "Bulle de savon";
	default: return "";
	}
}

// Get an icon to display based on the selected bullet type
std::string GUIManager::getIconPath(BulletType bullet_type) {
	switch (bullet_type) {
	case BULLET: return "particle_bullet.png";
	case CANNONBALL: return "particle_cannonball.png";
	case FIREBALL: return "particle_fireball.png";
	case BUBBLE: return "particle_bubble.png";
	default: return "";
	}
}

void GUIManager::setupSlots() {
	_slot_images.reserve(_nb_of_slots);
	_slot_icon_images.reserve(_nb_of_slots);

	for (int i = 0; i < _nb_of_slots; i++) {
		_slot_images.push_back(ofImage());
		_slot_images[i].load("ui_slot.png");

		_slot_icon_images.push_back(ofImage());
		_slot_icon_images[i].load(getIconPath(_particle_types[i]));
	}
	_slot_select.load("ui_slot_select.png");
}

void GUIManager::drawSlots() {
	const int screenHeight = ofGetHeight();

	// Compute the height at which to draw to first slot
	int drawHeight = (screenHeight - _nb_of_slots * (SLOT_SIZE + SLOT_PADDING)) / 2;
	
	for (int i = 0; i < _nb_of_slots; i++) {
		_slot_images[i].draw(20, drawHeight, SLOT_SIZE, SLOT_SIZE);
		_slot_icon_images[i].draw(30, drawHeight + 10, SLOT_SIZE - 20, SLOT_SIZE - 20);

		if (_current_slot == i) {
			_slot_select.draw(14, drawHeight - 6, SLOT_SIZE + 12, SLOT_SIZE + 12);
			ofDrawBitmapString(getDisplayName(_particle_types[i]), 20 + SLOT_SIZE + 12, drawHeight + SLOT_SIZE / 2);
		}

		drawHeight += (SLOT_SIZE + SLOT_PADDING);
	}
}

void GUIManager::drawFps() {
	stringstream fspStringStream;
	fspStringStream << std::fixed << std::setprecision(1)
		<< _fps << "fps "
		<< std::setprecision(3) << "(" << _dt << "s)";

	ofDrawBitmapString(fspStringStream.str(), 20, 20);
}
