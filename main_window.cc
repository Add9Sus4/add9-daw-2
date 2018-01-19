/*
 * MainWindow.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#include "main_window.h"

#include "window_area.h"

namespace add9daw2 {

MainWindow::MainWindow(int height, int width, int frames_per_buffer, int channels) : height_(height), width_(width) {
	playing_file_ = NULL; // No audio file currently playing
	// Add window areas to main window
	arrange_area_ = new WindowArea(ARRANGE_AREA_LEFT_SEPARATOR, ARRANGE_AREA_TOP_SEPARATOR, RIGHT_OF_SCREEN, EFFECTS_AREA_TOP_SEPARATOR, this);
	effects_area_ = new WindowArea(LEFT_OF_SCREEN, EFFECTS_AREA_TOP_SEPARATOR, RIGHT_OF_SCREEN, BOTTOM_OF_SCREEN, this);
	files_area_ = new WindowArea(LEFT_OF_SCREEN, ARRANGE_AREA_TOP_SEPARATOR, ARRANGE_AREA_LEFT_SEPARATOR, EFFECTS_AREA_TOP_SEPARATOR, this);
	top_menu_area_ = new WindowArea(LEFT_OF_SCREEN, TOP_OF_SCREEN, RIGHT_OF_SCREEN, ARRANGE_AREA_TOP_SEPARATOR, this);
	window_areas_.push_back(arrange_area_);
	window_areas_.push_back(effects_area_);
	window_areas_.push_back(files_area_);
	window_areas_.push_back(top_menu_area_);
	// Add audio tracks to arrange window
	for (int i=0; i<6; i++) {
		AudioTrack* audio_track = new AudioTrack(arrange_area_->getL() + PADDING,
				arrange_area_->getT() - DEFAULT_AUDIO_TRACK_HEIGHT*i - PADDING,
				arrange_area_->getR() - PADDING,
				arrange_area_->getT() - DEFAULT_AUDIO_TRACK_HEIGHT*(i + 1) + PADDING, this);
		char buf[20];
		snprintf(buf, 20, "Track %d", i + 1);
		audio_track->setName(buf);
		arrange_area_->addChildWindow(audio_track);
	}
	// Add folders to files area
	Folder* kick_folder = new Folder(KICK_DIR_PATH, files_area_->getL() + PADDING, files_area_->getT() - PADDING,
			files_area_->getR() - PADDING, files_area_->getT() - 0.1 - PADDING, this);
	files_area_->addChildWindow(kick_folder);
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

float* MainWindow::nextBlock(int frames_per_buffer, int channels) {
	float* block = new float[frames_per_buffer*channels];
	if (playing_file_) {
		bool playing = playing_file_->getAudio(block, frames_per_buffer, channels);
		if (!playing) {
			playing_file_ = NULL;
		}
	} else {
		for (int i=0; i<frames_per_buffer*channels; i++) {
			block[i] = 0.0;
		}
	}
	return block;
}

} // namespace add9daw2
