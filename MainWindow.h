/*
 * MainWindow.h
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_MAINWINDOW_H_
#define ADD9DAW2_SRC_MAINWINDOW_H_

#define effectsAreaTopSeparator		0.19
#define arrangeAreaLeftSeparator	0.2
#define arrangeAreaTopSeparator		0.96
#define leftOfScreen				0.0
#define topOfScreen					1.0
#define rightOfScreen				1.0
#define bottomOfScreen				0.0

#include <GLUT/glut.h>
#include <iostream>
#include "AudioTrack.h"
#include "Folder.h"
#include "Resources.h"
#include "WindowArea.h"

using namespace std;
// bottom to top = 0.0 to 1.0
// left to right = 0.0 to 1.0
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
	int height, width;
	WindowArea* arrangeArea;
	WindowArea* effectsArea;
	WindowArea* filesArea;
	WindowArea* topMenuArea;
	vector<WindowArea*> windowAreas;
	int framesPerBuffer, channels;
	AudioFile* playingFile;

};

#endif /* ADD9DAW2_SRC_MAINWINDOW_H_ */
