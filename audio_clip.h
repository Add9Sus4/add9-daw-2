/*
 * audio_clip.h
 *
 *  Created on: Jan 26, 2018
 *      Author: aarondawson
 */

#ifndef AUDIO_CLIP_H_
#define AUDIO_CLIP_H_

#include <string>

#include "audio_file.h"
#include "window.h"

namespace add9daw2 {
/*
 * This class represents an audio clip (aka a pointer to an audio file) that is placed in the
 * arrange window.
 */
class AudioClip : public Window {
public:
	AudioClip(double left, double top, double right, double bottom, Window* parent, double position, AudioFile* file, int start_in_samples, int end_in_samples) :
		Window(left, top, right, bottom, parent),
		width_of_sample_(0.0001),
		bpm_(140),
		file_(file),
		start_in_samples_(start_in_samples),
		end_in_samples_(end_in_samples)
		{}
	virtual ~AudioClip();
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	Rect Draw() override;
	inline void set_sample_width(double width_of_sample) { width_of_sample_ = width_of_sample; }
	inline void set_bpm(double bpm) { bpm_ = bpm; }
	double* get_audio(double locator_position, int frames_per_buffer, int channels);
	inline int get_start_in_samples() { return start_in_samples_; }
	inline int get_end_in_samples() { return end_in_samples_; }
	inline std::string get_file_name() { return file_->get_name(); }
private:
	double width_of_sample_, bpm_;

	// The start and end point of this clip, in terms of the samples offset since the beginning of the track
	int start_in_samples_, end_in_samples_;
	// A reference to the audio file to which this clip points.
	AudioFile* file_;
};

} /* namespace add9daw2 */

#endif /* AUDIO_CLIP_H_ */
