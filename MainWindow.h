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

#include "AudioTrack.h"
#include "Folder.h"
#include "Resources.h"

namespace add9daw2 {

class WindowArea;

class MainWindow {
public:
	MainWindow(int height, int width, int framesPerBuffer, int channels);
	virtual ~MainWindow();
	inline void draw(double x_offset, double y_offset) {
		for (int i=0; i<windowAreas.size(); i++) {
			windowAreas[i]->draw(x_offset, y_offset);
		}
	}
	inline void onClick(double x, double y) {
		// Check all window areas for click
		for (int i=0; i<windowAreas.size(); i++) {
			windowAreas[i]->onClick(x, y);
		}
	}
	inline void onUpClick(double x, double y) {
		// Check all window areas for up click
		for (int i=0; i<windowAreas.size(); i++) {
			windowAreas[i]->onUpClick(x, y);
		}
	}
	inline void onDoubleClick(double x, double y) {
		// Check all window areas for double click
		for (int i=0; i<windowAreas.size(); i++) {
			windowAreas[i]->onDoubleClick(x, y);
		}
	}
	inline void onDrag(double x, double y) {
		// Check all window areas for drag
		for (int i=0; i<windowAreas.size(); i++) {
			windowAreas[i]->onDrag(x, y);
		}
	}
	inline void setPlayingFile(AudioFile* file) { playingFile = file; }
	float* nextBlock(int framesPerBuffer, int channels);
	inline int getHeight() { return height; }
	inline int getWidth() { return width; }
	inline WindowArea* getFilesArea() {return filesArea; }
private:
	int height, width;
	WindowArea* arrangeArea;
	WindowArea* effectsArea;
	WindowArea* filesArea;
	WindowArea* topMenuArea;
	std::vector<WindowArea*> windowAreas;
	AudioFile* playingFile;

};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_MAINWINDOW_H_ */
