/*
 * audio_clip.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: aarondawson
 */

#include "audio_clip.h"

namespace add9daw2 {

AudioClip::~AudioClip() {
	// TODO Auto-generated destructor stub
}

bool AudioClip::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {

	if (!contains(mouse)) {
		color_ = color_init_;
		return false;
	}
	color_ = color_selected_;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Audio clip received click" << std::endl;
			set_dragging(true);
			break;
		case DOUBLE_CLICK:
			std::cout << "Audio clip received double click" << std::endl;
			break;
		case DRAG:
			if (is_being_dragged()) {
				std::cout << "Dragging in audio clip" << std::endl;
				drag_amt_ = mouse->x - mouse->xpress;
				std::cout << "Drag amt: " << drag_amt_ << std::endl;
			}
			break;
		case HOVER:
			std::cout << "Hovering in audio clip" << std::endl;
			break;
		case RELEASE:
			std::cout << "Audio clip received mouse release" << std::endl;
			mouse->ClearFile();
			set_dragging(false);
			set_end_in_samples(get_end_in_samples() + drag_amt_ / width_of_sample_);
			set_start_in_samples(get_start_in_samples() + drag_amt_ / width_of_sample_);
			drag_amt_ = 0;
			break;
		default:
			break;
	}
	return true;
}

Rect AudioClip::Draw() {
	// Set bounds on clip
	left_ = get_parent()->get_left() + (double) start_in_samples_ * width_of_sample_;
	right_ = get_parent()->get_left() + (double) end_in_samples_ * width_of_sample_;
	double clip_rect_left = left_ - x_offset_ + drag_amt_;
	double clip_rect_right = right_ - x_offset_ + drag_amt_;
	// If the clip overflows on the left side of the arrange window
	if (clip_rect_left < get_parent()->get_left()) {
		clip_rect_left = get_parent()->get_left();
	}
	// If the clip overflows on the right side of the arrange window
	if (clip_rect_right > get_parent()->get_right()) {
		clip_rect_right = get_parent()->get_right();
	}
	// If the clip is outside the bounds of the arrange window completely
	if (clip_rect_right < get_parent()->get_left() || clip_rect_left > get_parent()->get_right()) {
		return Rect(left_, top_, right_, bottom_);
	}
	// Draw clip rectangle
	glColor3d(color_.r, color_.g, color_.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(clip_rect_left, top_ - AUDIO_TRACK_PADDING);
	glVertex2d(clip_rect_right, top_ - AUDIO_TRACK_PADDING);
	glVertex2d(clip_rect_right, bottom_ + AUDIO_TRACK_PADDING);
	glVertex2d(clip_rect_left, bottom_ + AUDIO_TRACK_PADDING);
	glVertex2d(clip_rect_left, top_ - AUDIO_TRACK_PADDING);
	glEnd();
	// Draw audio
	file_->DrawInClip(left_ - x_offset_ + drag_amt_, top_ - AUDIO_TRACK_PADDING, right_ - x_offset_ + drag_amt_, bottom_ + AUDIO_TRACK_PADDING);
	//TODO: Draw all audio clips
	return Rect(left_, top_, right_, bottom_);
}

double* AudioClip::get_audio(double locator_position, int frames_per_buffer, int channels) {
	double* output = new double[frames_per_buffer*channels];
	// what sample should we start at?
//	std::cout << "getting audio starting from frame " << locator_position << std::endl;
	// Mono audio to mono output
	if (file_->is_mono() && channels == MONO) {
		double* audio;
		// If there are fewer than frames_per_buffer samples remaining in the file
		if (file_->get_num_frames() < locator_position + frames_per_buffer) {
			int remaining_frames = file_->get_num_frames() - locator_position;
			audio = file_->GetAudioFromChannel(LEFT, locator_position, locator_position + remaining_frames);
			// Use up the remaining samples
			for (int i=0; i<remaining_frames; i++) {
				output[i] = audio[i];
			}
			// Fill the rest with zeros
			for (int i=remaining_frames; i<frames_per_buffer; i++) {
				output[i] = 0.0;
			}
		}
		// If there are enough samples to fill the buffer completely
		else {
			audio = file_->GetAudioFromChannel(LEFT, locator_position, locator_position + frames_per_buffer);
			for (int i=0; i<frames_per_buffer; i++) {
				output[i] = audio[i];
			}
		}
		delete[] audio;
	} // End mono audio to mono output
	// Mono audio to stereo output
	if (file_->is_mono() && channels == STEREO) {
		double* audio;
		// If there are fewer than frames_per_buffer samples remaining in the file
		if (file_->get_num_frames() < locator_position + frames_per_buffer) {
			int remaining_frames = file_->get_num_frames() - locator_position;
			audio = file_->GetAudioFromChannel(LEFT, locator_position, locator_position + remaining_frames);
			// Use up the remaining samples
			for (int i=0; i<remaining_frames; i++) {
				output[2*i] = audio[i];
				output[2*i + 1] = audio[i];
			}
			// Fill the rest with zeros
			for (int i=remaining_frames; i<frames_per_buffer; i++) {
				output[2*i] = 0.0;
				output[2*i + 1] = 0.0;
			}
		}
		// If there are enough samples to fill the buffer completely
		else {
			audio = file_->GetAudioFromChannel(LEFT, locator_position, locator_position + frames_per_buffer);
			for (int i=0; i<frames_per_buffer; i++) {
				output[2*i] = audio[i];
				output[2*i + 1] = audio[i];
			}
		}
		delete[] audio;
	} // End mono audio to stereo output
	// Stereo audio to mono output
	if (file_->is_stereo() && channels == MONO) {
		double* audio;
		// If there are fewer than frames_per_buffer samples remaining in the file
		if (file_->get_num_frames() < locator_position + frames_per_buffer) {
			int remaining_frames = file_->get_num_frames() - locator_position;
			audio = file_->GetAudioFromChannel(LR_AVG, locator_position, locator_position + remaining_frames);
			// Use up the remaining samples
			for (int i=0; i<remaining_frames; i++) {
				output[i] = audio[i];
			}
			// Fill the rest with zeros
			for (int i=remaining_frames; i<frames_per_buffer; i++) {
				output[i] = 0.0;
			}
		}
		// If there are enough samples to fill the buffer completely
		else {
			audio = file_->GetAudioFromChannel(LR_AVG, locator_position, locator_position + frames_per_buffer);
			for (int i=0; i<frames_per_buffer; i++) {
				output[i] = audio[i];
			}
		}
		delete[] audio;
	} // End stereo audio to mono output
	// Stereo audio to stereo output
	if (file_->is_stereo() && channels == STEREO) {
		double* audio_left;
		double* audio_right;
		// If there are fewer than frames_per_buffer samples remaining in the file
		if (file_->get_num_frames() < locator_position + frames_per_buffer) {
			int remaining_frames = file_->get_num_frames() - locator_position;
			audio_left = file_->GetAudioFromChannel(LEFT, locator_position, locator_position + remaining_frames);
			audio_right = file_->GetAudioFromChannel(RIGHT, locator_position, locator_position + remaining_frames);
			// Use up the remaining samples
			for (int i=0; i<remaining_frames; i++) {
				output[2*i] = audio_left[i];
				output[2*i + 1] = audio_right[i];
			}
			// Fill the rest with zeros
			for (int i=remaining_frames; i<frames_per_buffer; i++) {
				output[2*i] = 0.0;
				output[2*i + 1] = 0.0;
			}
		}
		// If there are enough samples to fill the buffer completely
		else {
			audio_left = file_->GetAudioFromChannel(LEFT, locator_position, locator_position + frames_per_buffer);
			audio_right = file_->GetAudioFromChannel(RIGHT, locator_position, locator_position + frames_per_buffer);
			for (int i=0; i<frames_per_buffer; i++) {
				output[2*i] = audio_left[i];
				output[2*i + 1] = audio_right[i];
			}
		}
		delete[] audio_left;
		delete[] audio_right;
	} // End stereo audio to stereo output

	return output;
}

} /* namespace add9daw2 */
