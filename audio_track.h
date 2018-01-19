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

#include "window_area.h"

namespace add9daw2 {

class WindowArea;

// Represents a track of audio within the DAW.
// Handles the positioning and arrangement of audio files within a track.
class AudioTrack : public WindowArea {
public:
	AudioTrack(double l, double t, double r, double b, MainWindow* main_window) : WindowArea(l, t, r, b, main_window) {};
	virtual ~AudioTrack() {}
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_AUDIOTRACK_H_ */
