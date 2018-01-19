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

	inline int getHeight() { return height; }
	inline int getWidth() { return width; }

	inline void draw(double x_offset, double y_offset) {
		for (int i=0; i<window_areas.size(); i++) {
			window_areas[i]->draw(x_offset, y_offset);
		}
	}
	inline void onClick(double x, double y) {
		// Check all window areas for click
		for (int i=0; i<window_areas.size(); i++) {
			window_areas[i]->onClick(x, y);
		}
	}
	inline void onDoubleClick(double x, double y) {
		// Check all window areas for double click
		for (int i=0; i<window_areas.size(); i++) {
			window_areas[i]->onDoubleClick(x, y);
		}
	}
	inline void onDrag(double x, double y) {
		// Check all window areas for drag
		for (int i=0; i<window_areas.size(); i++) {
			window_areas[i]->onDrag(x, y);
		}
	}
	inline void onUpClick(double x, double y) {
		// Check all window areas for up click
		for (int i=0; i<window_areas.size(); i++) {
			window_areas[i]->onUpClick(x, y);
		}
	}
	inline void setPlayingFile(AudioFile* file) { playing_file = file; }

	inline WindowArea* getFilesArea() {return files_area; }
private:
	AudioFile* playing_file;

	int height, width;

	std::vector<WindowArea*> window_areas;

	WindowArea *arrange_area, *effects_area, *files_area, *top_menu_area;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_MAINWINDOW_H_ */
