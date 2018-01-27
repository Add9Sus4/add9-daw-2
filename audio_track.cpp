/*
 * audio_track.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: aarondawson
 */

#include "audio_track.h"

namespace add9daw2 {

AudioTrack::~AudioTrack() {
	// TODO Auto-generated destructor stub
}

bool AudioTrack::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {

	if (!contains(mouse)) {
		color = audio_track_init_;
		return false;
	}
	color = audio_track_selected_;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Arrange window received click" << std::endl;
			break;
		case DOUBLE_CLICK:
			std::cout << "Arrange window received double click" << std::endl;
			break;
		case DRAG:
			std::cout << "Dragging in arrange window" << std::endl;
			break;
		case HOVER:
			std::cout << "Hovering in arrange window" << std::endl;
			break;
		case RELEASE:
			std::cout << "Arrange window received mouse release" << std::endl;
			// If the mouse currently has an audio file
			if (mouse->file != NULL) {
				AddAudioClip(mouse->x, mouse->file);
			}
			mouse->ClearFile();
			break;
		default:
			break;
	}
	return true;
}

Rect AudioTrack::Draw() {
	// Draw track area
	glColor3d(color.r, color.g, color.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_, top_ - AUDIO_TRACK_PADDING);
	glVertex2d(right_, top_ - AUDIO_TRACK_PADDING);
	glVertex2d(right_, bottom_ + AUDIO_TRACK_PADDING);
	glVertex2d(left_, bottom_ + AUDIO_TRACK_PADDING);
	glVertex2d(left_, top_ - AUDIO_TRACK_PADDING);
	glEnd();
	//TODO: Draw all audio clips
	for (int i=0; i<audio_clips_.size(); i++) {
		audio_clips_[i]->set_sample_width(width_of_sample_);
		audio_clips_[i]->set_bpm(bpm_);
		audio_clips_[i]->Draw();
	}
	return Rect(left_, top_, right_, bottom_);
}

void AudioTrack::AddAudioClip(double position, AudioFile* file) {
	// Calculate clip length as displayed on screen based on the width of a measure. the bpm, and the sample rate
	int start_in_samples = (int) ((position - get_parent()->get_left()) / width_of_sample_);
	double clip_length = file->get_num_frames() * width_of_sample_;
	int end_in_samples = start_in_samples + file->get_num_frames();
	audio_clips_.push_back(new AudioClip(position, top_, position + clip_length, bottom_, this, position, file,
			start_in_samples, end_in_samples));
}

} /* namespace add9daw2 */
