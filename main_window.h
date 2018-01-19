/*
 * MainWindow.h
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_MAINWINDOW_H_
#define ADD9DAW2_SRC_MAINWINDOW_H_

#define EFFECTS_AREA_TOP_SEPARATOR	0.19
#define ARRANGE_AREA_LEFT_SEPARATOR	0.2
#define ARRANGE_AREA_TOP_SEPARATOR	0.96
#define LEFT_OF_SCREEN				0.0
#define TOP_OF_SCREEN				1.0
#define RIGHT_OF_SCREEN				1.0
#define BOTTOM_OF_SCREEN			0.0

#include <GLUT/glut.h>

#include <iostream>

#include "audio_track.h"
#include "folder.h"
#include "resources.h"

namespace add9daw2 {

class WindowArea;

class MainWindow {
public:
	MainWindow(int height, int width, int frames_per_buffer, int channels);
	virtual ~MainWindow();

	float* nextBlock(int frames_per_buffer, int channels);

	inline int getHeight() { return height_; }
	inline int getWidth() { return width_; }

	inline void draw(double x_offset, double y_offset) {
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->draw(x_offset, y_offset);
		}
	}
	inline void onClick(double x, double y) {
		// Check all window areas for click
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->onClick(x, y);
		}
	}
	inline void onDoubleClick(double x, double y) {
		// Check all window areas for double click
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->onDoubleClick(x, y);
		}
	}
	inline void onDrag(double x, double y) {
		// Check all window areas for drag
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->onDrag(x, y);
		}
	}
	inline void onUpClick(double x, double y) {
		// Check all window areas for up click
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->onUpClick(x, y);
		}
	}
	inline void setPlayingFile(AudioFile* file) { playing_file_ = file; }

	inline WindowArea* getFilesArea() {return files_area_; }
private:
	AudioFile* playing_file_;

	int height_, width_;

	std::vector<WindowArea*> window_areas_;

	WindowArea *arrange_area_, *effects_area_, *files_area_, *top_menu_area_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_MAINWINDOW_H_ */
