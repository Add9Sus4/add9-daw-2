/*
 * audio_track.h
 *
 *  Created on: Jan 26, 2018
 *      Author: aarondawson
 */

#ifndef AUDIO_TRACK_H_
#define AUDIO_TRACK_H_

#define AUDIO_TRACK_HEIGHT	0.1

#include <glut/GLUT.h>

#include <iostream>
#include <vector>

#include "audio_clip.h"
#include "window.h"

namespace add9daw2 {

class AudioTrack : public Window {
public:
	AudioTrack(double left, double top, double right, double bottom, Window* parent) :
		Window(left, top, right, bottom, parent), width_of_sample_(0.0001), bpm_(140.0) {}
	virtual ~AudioTrack();
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	Rect Draw() override;
	inline std::vector<AudioClip*> get_audio_clips() { return audio_clips_; }
	void AddAudioClip(double position, AudioFile* file);
	inline void set_sample_width(double width_of_sample) {
		width_of_sample_ = width_of_sample;
		for (int i=0; i<audio_clips_.size(); i++) {
			audio_clips_[i]->set_sample_width(width_of_sample);
		}
	}
	inline void set_bpm(double bpm) { bpm_ = bpm; }
	inline void set_x_offset(double x_offset) { x_offset_ = x_offset; }
private:
	Color audio_track_init_ = {0.4, 0.35, 0.3};
	Color audio_track_selected_ = {0.8, 0.7, 0.6};
	std::vector<AudioClip*> audio_clips_;
	double width_of_sample_, bpm_;
	double x_offset_ = 0.0;
};

} /* namespace add9daw2 */

#endif /* AUDIO_TRACK_H_ */
