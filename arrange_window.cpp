/*
 * arrange_window.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#include "arrange_window.h"

namespace add9daw2 {

ArrangeWindow::ArrangeWindow(double left, double top, double right, double bottom, Window* parent) :
		Window(left, top, right, bottom, parent) {
	mouse_ = NULL;
	AddAudioTrack();
	AddAudioTrack();
	AddAudioTrack();
	playback_locator_color_ = playback_locator_color_init_;
	zoom_area_color_ = zoom_area_color_init_;
	zoom_area_ = new Rect(left, bottom + ZOOM_AREA_HEIGHT, right, bottom);
}

ArrangeWindow::~ArrangeWindow() {
	// TODO Auto-generated destructor stub
}

Rect ArrangeWindow::Draw() {
	// Draw zoom area
	glColor3d(zoom_area_color_.r, zoom_area_color_.g, zoom_area_color_.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(zoom_area_->left + ZOOM_AREA_PADDING, zoom_area_->top - ZOOM_AREA_PADDING);
	glVertex2d(zoom_area_->right - ZOOM_AREA_PADDING, zoom_area_->top - ZOOM_AREA_PADDING);
	glVertex2d(zoom_area_->right - ZOOM_AREA_PADDING, zoom_area_->bottom + ZOOM_AREA_PADDING);
	glVertex2d(zoom_area_->left + ZOOM_AREA_PADDING, zoom_area_->bottom + ZOOM_AREA_PADDING);
	glVertex2d(zoom_area_->left + ZOOM_AREA_PADDING, zoom_area_->top - ZOOM_AREA_PADDING);
	glEnd();

	// Draw window area
	glColor3d(color.r, color.g, color.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_, top_);
	glVertex2d(right_, top_);
	glVertex2d(right_, bottom_);
	glVertex2d(left_, bottom_);
	glVertex2d(left_, top_);
	glEnd();
	// Draw measure markers
	int measure_number = 1;
	for (double i=left_; i<right_; i+=WidthOfMeasure()) {
		glBegin(GL_LINE_STRIP);
		glVertex2d(i, top_);
		glVertex2d(i, top_ - 0.01);
		glEnd();
		Font(GLUT_BITMAP_HELVETICA_10, (char *) std::to_string(measure_number).c_str(), i, top_ + 0.01);
		measure_number++;
	}
	// Draw audio tracks
	for (int i=0; i<audio_tracks_.size(); i++) {
		audio_tracks_[i]->set_sample_width(get_width_of_sample());
		audio_tracks_[i]->set_bpm(bpm_);
		audio_tracks_[i]->Draw();
	}
	// Draw the ghost of any audio sample that's currently being dragged in the arrange window
	if (mouse_) {
		if (mouse_->file != NULL) {
			std::cout << "mouse contains file: " << mouse_->file->get_name() << std::endl;
			mouse_->file->DrawGhost(mouse_->x, mouse_->y, get_width_of_sample(), bpm_);
		}
	}
	// Draw playback locator
	glColor3d(playback_locator_color_.r,playback_locator_color_.g,playback_locator_color_.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_ + get_width_of_sample() * playback_locator_, top_);
	glVertex2d(left_ + get_width_of_sample() * playback_locator_, bottom_);
	glEnd();
	return Rect(left_, top_, right_, bottom_);
}

bool ArrangeWindow::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
//	if (mouse->file != NULL) {
//		mouse->file->DrawGhost(mouse->x, mouse->y);
//	}
	mouse_ = mouse;
	if (!contains(mouse) && !zoom_area_dragging_) {
		ResetColor();
		return false;
	}
	// Check if mouse is in audio track area
	bool audio_track_targeted = false;
	for (int i=0; i<audio_tracks_.size(); i++) {
		if (audio_tracks_[i]->ReceiveMouseEvent(mouse, mouseEventType)) {
			audio_track_targeted = true;
		}
	}
	// If an audio track was targeted, disregard arrange window events
	if (audio_track_targeted) {
		ResetColor();
		return false;
	}
	color = selected;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Arrange window received click" << std::endl;
			if (is_near_playback_locator(mouse)) {
				playback_locator_selected_ = true;
			}
			if (is_in_zoom_area(mouse)) {
				zoom_area_dragging_ = true;
			}
			break;
		case DOUBLE_CLICK:
			std::cout << "Arrange window received double click" << std::endl;
			break;
		case DRAG:
			std::cout << "Dragging in arrange window" << std::endl;
			if (playback_locator_selected_) {
				playback_locator_ = (mouse->x - left_) / get_width_of_sample();
			}
			if (zoom_area_dragging_) {
				double drag_amt = mouse->y - mouse->ypress;
				std::cout << "drag amt: " << drag_amt << std::endl;
				zoom_drag_amt_ = drag_amt;
				for (int i=0; i<audio_tracks_.size(); i++) {
					audio_tracks_[i]->set_sample_width(get_width_of_sample());
				}
			}
			break;
		case HOVER:
			std::cout << "Hovering in arrange window" << std::endl;
			if (is_near_playback_locator(mouse)) {
				playback_locator_color_ = playback_locator_color_selected_;
			} else {
				playback_locator_color_ = playback_locator_color_init_;
			}
			if (is_in_zoom_area(mouse)) {
				zoom_area_color_ = zoom_area_color_selected_;
			} else {
				zoom_area_color_ = zoom_area_color_init_;
			}
			break;
		case RELEASE:
			std::cout << "Arrange window received mouse release" << std::endl;
			mouse->ClearFile();
			playback_locator_selected_ = false;
			zoom_area_dragging_ = false;
			width_of_sample_ += width_of_sample_ * zoom_drag_amt_;
			zoom_drag_amt_ = 0.0;
			break;
		default:
			break;
	}
	return true;
}

double* ArrangeWindow::GetAudio(int frames_per_buffer, int channels) {
//	std::cout << "playback locator: " << playback_locator_ << std::endl;
	double* audio = new double[frames_per_buffer*channels];
	// Zero out audio
	for (int i=0; i<frames_per_buffer*channels; i++) {
		audio[i] = 0.0;
	}
	// Search all audio clips to see if the playback locator overlaps them (the next block will include some of the sample
	for (int i=0; i<audio_tracks_.size(); i++) {
		std::vector<AudioClip*> audio_clips = audio_tracks_[i]->get_audio_clips();
		for (int j=0; j<audio_clips.size(); j++) {
			int start = audio_clips[j]->get_start_in_samples();
			int end = audio_clips[j]->get_end_in_samples();
			// If the locator position has not yet gotten to the sample, but will do so within the next frames_per_buffer samples
			double distance_in_next_block = frames_per_buffer;
//			std::cout << "Clip left: " << audio_clips[i]->get_left() << ", locator position: " << locator_position <<
//					", distance_in_next_block: " << distance_in_next_block << ", future locator position: " << locator_position + distance_in_next_block << std::endl;
			if (playback_locator_ < start && playback_locator_ + distance_in_next_block >= start &&
					playback_locator_ + distance_in_next_block < end) {
				double* clip_audio = audio_clips[j]->get_audio(playback_locator_ - start, frames_per_buffer, channels);
				for (int k=0; k<frames_per_buffer*channels; k++) {
					audio[k] += clip_audio[k];
				}
				delete[] clip_audio;
			}
			// If the locator position is in the middle of a sample
			else if (playback_locator_ >= start && playback_locator_ < end) {
				double* clip_audio = audio_clips[j]->get_audio(playback_locator_ - start, frames_per_buffer, channels);
				for (int k=0; k<frames_per_buffer*channels; k++) {
					audio[k] += clip_audio[k];
				}
				delete[] clip_audio;
			}
		}
	}
//	for (int i=0; i<frames_per_buffer*channels; i++) {
//		std::cout << "audio[" << i << "]: " << audio[i] << std::endl;
//	}
	return audio;
}

void ArrangeWindow::AddAudioTrack() {
		// If there are no audio tracks put the track at the top
		if (audio_tracks_.size() == 0) {
			audio_tracks_.push_back(new AudioTrack(left_, top_, right_, top_ - AUDIO_TRACK_HEIGHT, this));
		}
		// Otherwise, place new audio track below existing tracks
		else {
			AudioTrack* last_audio_track = audio_tracks_[audio_tracks_.size() - 1];
			audio_tracks_.push_back(new AudioTrack(left_, last_audio_track->get_bottom(), right_, last_audio_track->get_bottom() - AUDIO_TRACK_HEIGHT, this));
		}
	}

void ArrangeWindow::AdvancePlaybackLocator(int frames_per_buffer) {
	playback_locator_ += frames_per_buffer;
}

void ArrangeWindow::Font(void *font, char *text, double x, double y) {
	glDisable(GL_LIGHTING);
	char buf[20];
	snprintf(buf, 20, "%s", text);
	glRasterPos2d(x, y);
	while ( *text != '\0') {
		glutBitmapCharacter(font, *text);
		++text;
	}
	glEnable(GL_LIGHTING);
}

} /* namespace add9daw2 */
