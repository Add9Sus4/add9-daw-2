/*
 * MainWindow.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#include "MainWindow.h"

MainWindow::MainWindow(int height, int width, int framesPerBuffer, int channels) : height(height), width(width),
	framesPerBuffer(framesPerBuffer), channels(channels) {
	playingFile = NULL; // No audio file currently playing
	// Add window areas to main window
	arrangeArea = new WindowArea(arrangeAreaLeftSeparator, arrangeAreaTopSeparator, rightOfScreen, effectsAreaTopSeparator, this);
	effectsArea = new WindowArea(leftOfScreen, effectsAreaTopSeparator, rightOfScreen, bottomOfScreen, this);
	filesArea = new WindowArea(leftOfScreen, arrangeAreaTopSeparator, arrangeAreaLeftSeparator, effectsAreaTopSeparator, this);
	topMenuArea = new WindowArea(leftOfScreen, topOfScreen, rightOfScreen, arrangeAreaTopSeparator, this);
	windowAreas.push_back(arrangeArea);
	windowAreas.push_back(effectsArea);
	windowAreas.push_back(filesArea);
	windowAreas.push_back(topMenuArea);
	// Add audio tracks to arrange window
	for (int i=0; i<6; i++) {
		AudioTrack* audioTrack = new AudioTrack(arrangeArea->getL() + PADDING,
				arrangeArea->getT() - DEFAULT_AUDIO_TRACK_HEIGHT*i - PADDING,
				arrangeArea->getR() - PADDING,
				arrangeArea->getT() - DEFAULT_AUDIO_TRACK_HEIGHT*(i + 1) + PADDING, this);
		char buf[20];
		snprintf(buf, 20, "Track %d", i + 1);
		audioTrack->setName(buf);
		arrangeArea->addChildWindow(audioTrack);
	}
	// Add folders to files area
	Folder* kickFolder = new Folder(KICK_DIR_PATH, filesArea->getL() + PADDING, filesArea->getT() - PADDING,
			filesArea->getR() - PADDING, filesArea->getT() - 0.1 - PADDING, this);
	filesArea->addChildWindow(kickFolder);
}

MainWindow::~MainWindow() {
	// TODO Auto-generated destructor stub
}

float* MainWindow::nextBlock(int framesPerBuffer, int channels) {
	float* block = new float[framesPerBuffer*channels];
	if (playingFile) {
		bool playing = playingFile->getAudio(block, framesPerBuffer, channels);
		if (!playing) {
			playingFile = NULL;
		}
	} else {
		for (int i=0; i<framesPerBuffer*channels; i++) {
			block[i] = 0.0;
		}
	}
	return block;
}
