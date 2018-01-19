/*
 * AudioTrack.h
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_AUDIOTRACK_H_
#define ADD9DAW2_SRC_AUDIOTRACK_H_

#define DEFAULT_AUDIO_TRACK_HEIGHT	0.050
#define AUDIO_TRACK_PADDING			0.0005

#include "WindowArea.h"

class AudioTrack : public WindowArea {
public:
	AudioTrack(double l, double t, double r, double b, MainWindow* mainWindow) : WindowArea(l, t, r, b, mainWindow) {};
	virtual ~AudioTrack() {}
};

#endif /* ADD9DAW2_SRC_AUDIOTRACK_H_ */
