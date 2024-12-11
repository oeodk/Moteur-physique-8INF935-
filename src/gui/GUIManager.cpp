#include "GUIManager.h"
#include <cmath>

void GUIManager::setup(const std::vector<BulletType>& particle_types) {
	_particle_types = particle_types;
	_nb_of_slots = _particle_types.size();
	_gui_oscillation_t = 0.f;
	_blob_particle_count = -1;
	setupSlots();
}

void GUIManager::update(float const dt, unsigned char const selected_particle) {
	_dt = dt;
	_fps = ofGetFrameRate();

	setSelectedSlot(selected_particle);
	updateBlobParticleCountTextHeight(dt);
}

void GUIManager::draw() {
	ofSetColor(255);
	ofEnableAlphaBlending();

	drawFps();
	drawSlots();

	drawBlobParticleCount();
	drawInstructions();

	ofDisableAlphaBlending();
}

// Get a name to display based on the selected bullet type
std::string GUIManager::getDisplayName(BulletType bullet_type) {
	switch (bullet_type) {
	case BULLET: return "Balle de fusil";
	case CANNONBALL: return "Boulet de canon";
	case FIREBALL: return "Boule de feu";
	case BUBBLE: return "Bulle de savon";
	case BLOB: return "Noyau de blob";
	case CHICKEN: return "Poulet";
	case MASAMUNE: return "Masamune";
	case SEPHIROTH: return "Sephiroth";
	case GOAT: return "Chevre";
	case STATIC_CUBE: return "Cube de test immobile";
	case MOVING_CUBE: return "Cube de test mobile";
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
	case BLOB: return "particle_blob.png";
	case CHICKEN: return "rigidbody_icons/chicken.png";
	case MASAMUNE: return "rigidbody_icons/masamune.png";
	case SEPHIROTH: return "rigidbody_icons/sephiroth.png";
	case GOAT: return "rigidbody_icons/goat.png";
	case STATIC_CUBE: return "rigidbody_icons/static_cube.png";
	case MOVING_CUBE: return "rigidbody_icons/moving_cube.png";
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

void GUIManager::drawInstructions() {
	const int screenHeight = ofGetHeight();
	string messages[] = {
		"Clic gauche : Tirer une particule",
		"Molette : Choisir une particule",
		"Ctrl gauche : Sprint",
		"Tab : Montrer / cacher les trainees des particules",
		"Alt : Effacer toutes les particules",
		"Flèches et Entree : Deplacer le blob",
		"Maj droite : Arrêter les mouvements du blob",
		"Ctrl droite : Separer le blob",
		"1 : Exemple contrainte de cable",
		"2 : Exemple contrainte de tige"
	};

	int length = sizeof(messages) / sizeof(messages[0]);
	for (int i = 0; i < length; i++)
		ofDrawBitmapString(messages[i], 20, screenHeight - (5 + 12 * (length - i)));
}

void GUIManager::updateBlobParticleCountTextHeight(float const dt) {
	_gui_oscillation_t += dt;

	float amplitude = GUI_OSCILLATION_AMPLITUDE * std::exp(-GUI_OSCILLATION_DAMPING_RATIO * _gui_oscillation_t);
	_gui_particle_count_height = amplitude * std::sin(GUI_OSCILLATION_FREQUENCY * _gui_oscillation_t);
}

void GUIManager::drawBlobParticleCount() {
	if (_blob_particle_count < 0) return;
	string text = "Nb. particules du blob : " + std::to_string(_blob_particle_count);
	ofDrawBitmapString(text, 20, 70 + _gui_particle_count_height);
}

// Called in ofApp.cpp
void GUIManager::setBlobParticleCount(int count) {
	if (count == _blob_particle_count) return;
	_blob_particle_count = count;
	_gui_oscillation_t = 0.f;
}
