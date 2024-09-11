#define _USE_MATH_DEFINES
#include "ofApp.h"
#include <execution>
#include <cmath>

constexpr float g = 9.81 * 10;
constexpr int SPEED_RANGE = 100;
constexpr float TRAJECTORY_POINT_NUMBER = 100;

//--------------------------------------------------------------
void ofApp::setup()
{
	pause_ = false;
	display_trajectory_ = true;
	elapsed_time_ = 0;

	constexpr unsigned int GUI_WIDTH = 250;

	gui_.setup();
	gui_.setWidthElements(GUI_WIDTH);
	gui_.add(frame_information_.setup("Frames informations"));

	frame_information_.add(label_dt_.setup("label", "THIS IS A LABEL"));
	frame_information_.add(label_fps_.setup("label", "THIS IS A LABEL"));
	
	label_dt_.setDefaultWidth(GUI_WIDTH - 4);
	label_fps_.setDefaultWidth(GUI_WIDTH - 4);

	gui_.add(particles_selection_group_.setup("Particles"));

	constexpr const char* PROJECTILE_NAME[4] = { "Bullet", "Laser", "Fire Ball", "Rubber Ball" };
	for (size_t i = 0; i < particles_selection_toggle_buttons_.size(); i++)
	{
		particles_selection_group_.add(particles_selection_toggle_buttons_[i].setup(particles_selection_parameters_[i].set(PROJECTILE_NAME[i], false)));
		particles_selection_toggle_buttons_[i].addListener(this, &ofApp::updateSelectedParticle);
	}

	gui_.add(shoot_group_.setup("Shoot data"));

	shoot_group_.add(shoot_angles_[0].set("Theta", 0.f, 0, 180));
	shoot_group_.add(shoot_angles_[1].set("Phi", 0.f, 0, 360));
	shoot_group_.add(shoot_button_.setup("Shoot"));
	shoot_button_.addListener(this, &ofApp::shootProjectile);

	gui_.enableHeader();

	theta_ = shoot_angles_[0].get();
	phi_= shoot_angles_[1].get();
	selected_particle_ = 0;

	particles_selection_parameters_[selected_particle_].set(true);
}

//--------------------------------------------------------------
void ofApp::update()
{
	frame_duration_ = ofGetLastFrameTime();
	label_dt_.setup("Frame duration", std::to_string(frame_duration_) + "s");
	label_fps_.setup("Frame duration", std::to_string(1.0 / frame_duration_) + "fps");
	elapsed_time_ += frame_duration_;

	render_engine_.update(frame_duration_);

	if(!pause_)
	{
		for (size_t i = 0; i < particles_.size(); i++)
		{
			//particles_[i]->update(frame_duration_);
			//if (particles_[i]->willDisappear())
			//{
			//	delete particles_[i];
			//	particles_.erase(particles_.begin() + i);
			//	i--;
			//}
		}
		//for_each(std::execution::par, particles_test_.begin(), particles_test_.end(), [&](Particle& p) { p.update(frame_duration_); });
	}

}

//--------------------------------------------------------------
void ofApp::draw()
{
	render_engine_.render();
	gui_.draw();
}

void ofApp::exit()
{
	for (size_t i = 0; i < particles_.size(); i++)
	{
		delete particles_[i];
	}
	shoot_button_.removeListener(this, &ofApp::shootProjectile);
}

void ofApp::shootProjectile()
{
}

void ofApp::updateSelectedParticle(bool& state)
{
	bool success = false;
	for (unsigned char i = 0; i < particles_selection_toggle_buttons_.size(); i++)
	{
		if (particles_selection_parameters_[i].get() && i != selected_particle_)
		{
			particles_selection_parameters_[selected_particle_].set(false);
			selected_particle_ = i;
			success = true;
		}
	}
	if (!success)
	{
		particles_selection_parameters_[selected_particle_].set(true);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == ofKey::OF_KEY_RETURN)
	{
		display_trajectory_ = !display_trajectory_;
	}
	if(key == ' ')
	{
		pause_ = !pause_;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
