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

// The main window in the DAW.
// Handles the organization of all sub-windows.
// Also responsible for getting audio to send to the output.
class MainWindow {
public:
	MainWindow(int height, int width, int frames_per_buffer, int channels);
	virtual ~MainWindow();

	// Gets the next block of audio to send to the output.
	// If an audio file is currently playing, this will get that audio.
	// Otherwise, it will send silence.
	float* GetNextBlock(int frames_per_buffer, int channels);

	// Returns the height of the main window.
	inline int get_height() { return height_; }

	// Returns the width of the main window.
	inline int get_width() { return width_; }

	// Handles the drawing of all sub-windows.
	inline void Draw(double x_offset, double y_offset) {
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->Draw(x_offset, y_offset);
		}
	}

	// Handles all clicks in the main window area.
	inline void OnClick(double x, double y) {
		// Check all window areas for click
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->OnClick(x, y);
		}
	}

	// Handles all double clicks in the main window area.
	inline void OnDoubleClick(double x, double y) {
		// Check all window areas for double click
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->OnDoubleClick(x, y);
		}
	}

	// Handles all mouse drag events in the main window area.
	inline void OnDrag(double x, double y) {
		// Check all window areas for drag
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->OnDrag(x, y);
		}
	}

	// Handles all mouse release events in the main window area.
	inline void OnUpClick(double x, double y) {
		// Check all window areas for up click
		for (int i=0; i<window_areas_.size(); i++) {
			window_areas_[i]->OnUpClick(x, y);
		}
	}

	// Sets the currently playing file to the specified audio file.
	inline void set_playing_file(AudioFile* file) { playing_file_ = file; }

	// Returns a reference to the window representing the files area.
	inline WindowArea* get_files_area() {return files_area_; }
private:
	AudioFile* playing_file_;

	int height_, width_;

	std::vector<WindowArea*> window_areas_;

	WindowArea *arrange_area_, *effects_area_, *files_area_, *top_menu_area_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_MAINWINDOW_H_ */
