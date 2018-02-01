/*
 * audio_clip.h
 *
 *  Created on: Jan 26, 2018
 *      Author: aarondawson
 */

#ifndef AUDIO_CLIP_H_
#define AUDIO_CLIP_H_

#define AUDIO_TRACK_PADDING	0.01

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
	AudioClip(double left, double top, double right, double bottom, Window* parent,
			double position, AudioFile* file, int start_in_samples, int end_in_samples) :
		Window(left, top, right, bottom, parent),
		width_of_sample_(0.0001),
		bpm_(140),
		start_in_samples_(start_in_samples),
		end_in_samples_(end_in_samples),
		file_(file) {}
	virtual ~AudioClip();
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	inline bool is_being_dragged() { return dragging_; }
	inline bool contains(Mouse* mouse) override {
		return mouse->x >= left_ - get_x_offset() && mouse->x <= right_ - get_x_offset() && mouse->y - translate_amount_ <= top_ && mouse->y - translate_amount_ >= bottom_;
	}
	Rect Draw() override;
	inline void set_sample_width(double width_of_sample) { width_of_sample_ = width_of_sample; }
	inline void set_bpm(double bpm) { bpm_ = bpm; }
	inline void set_dragging(bool dragging) { dragging_ = dragging; }
	inline void set_drag_amt(double drag_amt) { drag_amt_ = drag_amt; }
	inline void set_start_in_samples(int start) { start_in_samples_ = start; }
	inline void set_end_in_samples(int end) { end_in_samples_ = end; }
	inline void set_x_offset(double x_offset) { x_offset_ = x_offset; }
	double* get_audio(double locator_position, int frames_per_buffer, int channels);
	inline int get_start_in_samples() { return start_in_samples_; }
	inline int get_end_in_samples() { return end_in_samples_; }
	inline double get_drag_amt() { return drag_amt_; }
	inline double get_x_offset() { return x_offset_ + drag_amt_; }
	inline std::string get_file_name() { return file_->get_name(); }
private:
	bool dragging_ = false;

	double width_of_sample_, bpm_;
	double drag_amt_ = 0.0;
	double x_offset_ = 0.0;

	// The start and end point of this clip, in terms of the samples offset since the beginning of the track
	int start_in_samples_, end_in_samples_;
	// A reference to the audio file to which this clip points.
	AudioFile* file_;
};

} /* namespace add9daw2 */

#endif /* AUDIO_CLIP_H_ */
