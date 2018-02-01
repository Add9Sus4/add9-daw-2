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
	// Check if an audio clip is being dragged
	// See if the mouse is inside an audio clip
	bool clip_selected = false;
	for (int i=0; i<audio_clips_.size(); i++) {
		// Also check if the audio clip is being dragged
		if (audio_clips_[i]->is_being_dragged()) {
			audio_clips_[i]->set_drag_amt(mouse->x - mouse->xpress);
		}
		if (audio_clips_[i]->ReceiveMouseEvent(mouse, mouseEventType)) {
			clip_selected = true;
		}
	}
	if (clip_selected) {
		color_ = audio_track_init_;
		return true;
	}
	if (!contains(mouse)) {
		color_ = audio_track_init_;
		return false;
	}
	color_ = audio_track_selected_;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Audio track received click" << std::endl;
			break;
		case DOUBLE_CLICK:
			std::cout << "Audio track received double click" << std::endl;
			break;
		case DRAG:
			std::cout << "Dragging in audio track" << std::endl;
			break;
		case HOVER:
			std::cout << "Hovering in audio track" << std::endl;
			break;
		case RELEASE:
			std::cout << "Audio track received mouse release" << std::endl;
			// If the mouse currently has an audio file
			if (mouse->file != NULL) {
				AddAudioClip(mouse->x + x_offset_, mouse->file);
			}
			// Check to see if an audio clip was being dragged
			for (int i=0; i<audio_clips_.size(); i++) {
				if (audio_clips_[i]->is_being_dragged()) {
					std::cout << "releasing audio clip from drag" << std::endl;
					audio_clips_[i]->set_dragging(false);
					audio_clips_[i]->set_end_in_samples(audio_clips_[i]->get_end_in_samples() + audio_clips_[i]->get_drag_amt() / width_of_sample_);
					audio_clips_[i]->set_start_in_samples(audio_clips_[i]->get_start_in_samples() + audio_clips_[i]->get_drag_amt() / width_of_sample_);
					audio_clips_[i]->set_drag_amt(0.0);
				}
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
	glColor3d(color_.r, color_.g, color_.b);
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
		audio_clips_[i]->set_x_offset(x_offset_);
		audio_clips_[i]->set_bpm(bpm_);
		audio_clips_[i]->Draw();
	}
	// Draw name of audio track
	Font(GLUT_BITMAP_HELVETICA_10, (char*) name_.c_str(), right_ + AUDIO_TRACK_PADDING, bottom_ + (top_ - bottom_)/2.0);
	return Rect(left_, top_, right_, bottom_);
}

std::string AudioTrack::get_sample_type_name() {
	std::string name = "N/A";
	switch (sample_type_) {
	case CLAP:
		name = "Clap";
		break;
	case CRASH:
		name = "Crash";
		break;
	case EIGHT_O_EIGHT:
		name = "808";
		break;
	case HAT:
		name = "Hat";
		break;
	case HIGH_IMPACT:
		name = "High Impact";
		break;
	case KICK:
		name = "Kick";
		break;
	case LOW_IMPACT:
		name = "Low Impact";
		break;
	case PERC:
		name = "Perc";
		break;
	case PITCH_DOWN:
		name = "Pitch Down";
		break;
	case PITCH_RISER:
		name = "Pitch Riser";
		break;
	case PRYDA:
		name = "Pryda";
		break;
	case PUNCHY_IMPACT:
		name = "Punchy Impact";
		break;
	case RIDE:
		name = "Ride";
		break;
	case SHAKER:
		name = "Shaker";
		break;
	case SNAP:
		name = "Snap";
		break;
	case SNARE:
		name = "Snare";
		break;
	case SUB_DROP:
		name = "Sub Drop";
		break;
	case SWEEP_DOWN:
		name = "Sweep Down";
		break;
	case SWEEP_UP:
		name = "Sweep Up";
		break;
	case TOM:
		name = "Tom";
		break;
	case WAR_DRUM:
		name = "War Drum";
		break;
	default:
		name = "N/A";
		break;
	}
	return name;
}

void AudioTrack::AddAudioClip(double position, AudioFile* file) {
	// Calculate clip length as displayed on screen based on the width of a measure. the bpm, and the sample rate
	int start_in_samples = (int) ((position - get_parent()->get_left()) / width_of_sample_);
	double clip_length = file->get_num_frames() * width_of_sample_;
	int end_in_samples = start_in_samples + file->get_num_frames();
	audio_clips_.push_back(new AudioClip(position, top_, position + clip_length, bottom_, this, position, file,
			start_in_samples, end_in_samples));
}

void AudioTrack::AddAudioClip(int start_in_samples, AudioFile* file) {
	// Calculate clip position
	double position = start_in_samples * width_of_sample_ + get_parent()->get_left();
	double clip_length = file->get_num_frames() * width_of_sample_;
	int end_in_samples = start_in_samples + file->get_num_frames();
	AudioClip* clip = new AudioClip(position, top_, position + clip_length, bottom_, this, position, file,
			start_in_samples, end_in_samples);
	file->set_parent(clip);
//	std::cout << "creating audio clip with bounds: " << position << ", " << top_ << ", " << position + clip_length
//			<< ", " << bottom_ << std::endl;
	audio_clips_.push_back(clip);
}

} /* namespace add9daw2 */
