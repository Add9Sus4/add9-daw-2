/*
 * arrange_window.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#include "arrange_window.h"
#include "resources.h"

namespace add9daw2 {

ArrangeWindow::ArrangeWindow(double left, double top, double right, double bottom, Window* parent) :
		Window(left, top, right, bottom, parent) {
	mouse_ = NULL;
	// Add initial audio tracks
	AddAudioTrack(KICK);
	AddAudioTrack(CLAP);
	AddAudioTrack(SNARE);
	AddAudioTrack(HAT);
	AddAudioTrack(HIGH_IMPACT);
	AddAudioTrack(LOW_IMPACT);
	AddAudioTrack(PUNCHY_IMPACT);
	AddAudioTrack(SWEEP_UP);
	AddAudioTrack(SWEEP_DOWN);
	// Initialize colors
	playback_locator_color_ = playback_locator_color_init_;
	zoom_area_color_ = zoom_area_color_init_;
	// Create zoom area
	zoom_area_ = new Rect(left, bottom + ZOOM_AREA_HEIGHT, right, bottom);
	// Add initial sections
	AddSection(new Section(INTRO, 1, 5, this));
	AddSection(new Section(VERSE, 5, 21, this));
	AddSection(new Section(BREAK, 21, 29, this));
	AddSection(new Section(BUILD, 29, 37, this));
	AddSection(new Section(DROP, 37, 53, this));
	AddSection(new Section(BREAK, 53, 57, this));
	AddSection(new Section(VERSE, 57, 73, this));
	AddSection(new Section(BREAK, 73, 81, this));
	AddSection(new Section(BUILD, 81, 89, this));
	AddSection(new Section(DROP, 89, 105, this));
	AddSection(new Section(OUTRO, 105, 113, this));

	if (sections_.size() > 1) {
		AdjustBounds(sections_[0]->get_start_measure(), sections_[sections_.size()-1]->get_end_measure());
	}
}

ArrangeWindow::~ArrangeWindow() {
	// TODO Auto-generated destructor stub
}

Rect ArrangeWindow::Draw() {
	// If a menu is open
	bool menu_open = false;
	for (int i=0; i<sections_.size(); i++) {
		if (sections_[i]->has_menu_open()) {
			menu_open = true;
		}
	}
	if (!menu_open) {
		// Draw zoom area
		glColor3d(zoom_area_color_.r, zoom_area_color_.g, zoom_area_color_.b);
		glBegin(GL_LINE_STRIP);
		glVertex2d(zoom_area_->left + ZOOM_AREA_PADDING, zoom_area_->top - ZOOM_AREA_PADDING);
		glVertex2d(zoom_area_->right - ZOOM_AREA_PADDING, zoom_area_->top - ZOOM_AREA_PADDING);
		glVertex2d(zoom_area_->right - ZOOM_AREA_PADDING, zoom_area_->bottom + ZOOM_AREA_PADDING);
		glVertex2d(zoom_area_->left + ZOOM_AREA_PADDING, zoom_area_->bottom + ZOOM_AREA_PADDING);
		glVertex2d(zoom_area_->left + ZOOM_AREA_PADDING, zoom_area_->top - ZOOM_AREA_PADDING);
		glEnd();
	}

	// Draw window area
	glColor3d(color_.r, color_.g, color_.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_, top_);
	glVertex2d(right_, top_);
	glVertex2d(right_, bottom_);
	glVertex2d(left_, bottom_);
	glVertex2d(left_, top_);
	glEnd();
	// Draw track sections
	for (int i=0; i<sections_.size(); i++) {
		Section* section = sections_[i];
		section->SetBounds(left_ - get_x_offset() + section->get_start_measure() * WidthOfMeasure(),
				top_ + 0.1,
				left_ - get_x_offset() + section->get_end_measure() * WidthOfMeasure(),
				top_ + 0.05);
		section->Draw();
	}
	if (!menu_open) {
		// Draw measure markers
		double num_measures = width() / WidthOfMeasure();
		int measure_number = 0;
		for (double i=left_ - get_x_offset(); i<right_; i+=WidthOfMeasure()) {
			if (i > left_) {
				glColor3d(color_.r, color_.g, color_.b);
				glBegin(GL_LINE_STRIP);
				glVertex2d(i, top_);
				glVertex2d(i, top_ - 0.01);
				glEnd();
				// If more than 50 measures are visible, draw every other one
				if (measure_number%4 == 0) {
					Font(GLUT_BITMAP_HELVETICA_10, (char *) std::to_string(measure_number).c_str(), i, top_ + 0.01);
				} else if (measure_number%4 == 2) {
					if (num_measures < 100.0) {
						Font(GLUT_BITMAP_HELVETICA_10, (char *) std::to_string(measure_number).c_str(), i, top_ + 0.01);
					}
				} else {
					if (num_measures < 50.0) {
						Font(GLUT_BITMAP_HELVETICA_10, (char *) std::to_string(measure_number).c_str(), i, top_ + 0.01);
					}
				}
			}
			measure_number++;
		}
		// Draw audio tracks
		for (int i=0; i<audio_tracks_.size(); i++) {
			audio_tracks_[i]->set_sample_width(get_width_of_sample());
			audio_tracks_[i]->set_x_offset(get_x_offset());
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
		if (get_width_of_sample() * playback_locator_ > get_x_offset() && get_width_of_sample() * playback_locator_ + left_ - get_x_offset() < right_) {
			glColor3d(playback_locator_color_.r,playback_locator_color_.g,playback_locator_color_.b);
			glBegin(GL_LINE_STRIP);
			glVertex2d(left_ - get_x_offset() + get_width_of_sample() * playback_locator_, top_);
			glVertex2d(left_ - get_x_offset() + get_width_of_sample() * playback_locator_, bottom_);
			glEnd();
		}
	}
	return Rect(left_, top_, right_, bottom_);
}

bool ArrangeWindow::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
	mouse_ = mouse;
	// Check if mouse is in section area, and also if a menu is open
	bool mouse_in_section_area = false;
	bool menu_open = false;
	for (int i=0; i<sections_.size(); i++) {
		if (sections_[i]->has_menu_open()) {
			menu_open = true;
			sections_[i]->ReceiveMouseEvent(mouse, mouseEventType);
		}
		if (sections_[i]->contains(mouse)) {
			mouse_in_section_area = true;
			sections_[i]->ReceiveMouseEvent(mouse, mouseEventType);
		} else {
			sections_[i]->set_not_selected();
		}
	}
	if (mouse_in_section_area) {
		ResetColor();
		return false;
	}
	if (menu_open) {
		ResetColor();
		return false;
	}
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
	color_ = color_selected_;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Arrange window received click" << std::endl;
			if (is_near_playback_locator(mouse) && !is_in_zoom_area(mouse)) {
				playback_locator_selected_ = true;
			} else if (is_in_zoom_area(mouse)) {
				zoom_area_dragging_ = true;
			} else {
				playback_locator_ = (mouse->x + get_x_offset() - left_) / get_width_of_sample();
			}
			break;
		case DOUBLE_CLICK:
			std::cout << "Arrange window received double click" << std::endl;
			if (sections_.size() > 1) {
				AdjustBounds(sections_[0]->get_start_measure(), sections_[sections_.size()-1]->get_end_measure());
			}
			break;
		case DRAG:
			std::cout << "Dragging in arrange window" << std::endl;
			if (playback_locator_selected_) {
				playback_locator_ = (mouse->x + get_x_offset() - left_) / get_width_of_sample();
			}
			if (zoom_area_dragging_) {
				double drag_z_amt = mouse->y - mouse->ypress;
				double drag_x_amt = mouse->x - mouse->xpress;
				std::cout << "drag amt: " << drag_x_amt << std::endl;
				zoom_drag_z_amt_ = drag_z_amt;
				if (x_offset_ + drag_x_amt < 0.0) {

					zoom_drag_x_amt_ = -x_offset_;
				} else {

					set_zoom_drag_x_amt(drag_x_amt);
				}
				for (int i=0; i<audio_tracks_.size(); i++) {
					audio_tracks_[i]->set_sample_width(get_width_of_sample());
				}
			}
			break;
		case HOVER:
			std::cout << "Hovering in arrange window" << std::endl;
			if (is_near_playback_locator(mouse) && !is_in_zoom_area(mouse)) {
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
			width_of_sample_ += width_of_sample_ * zoom_drag_z_amt_;
			zoom_drag_z_amt_ = 0.0;
			x_offset_ += zoom_drag_x_amt_;
			zoom_drag_x_amt_ = 0.0;
			break;
		default:
			break;
	}
	return true;
}

void ArrangeWindow::AdjustBounds(int start_measure, int end_measure) {
	double current_length_in_measures = width() / WidthOfMeasure();
	double new_length_in_measures = (double) end_measure - start_measure;
	width_of_sample_ *= current_length_in_measures / new_length_in_measures;
	x_offset_ = start_measure * WidthOfMeasure();
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

void ArrangeWindow::AddAudioTrack(SampleType sample_type) {
		// If there are no audio tracks put the track at the top
		if (audio_tracks_.size() == 0) {
			audio_tracks_.push_back(new AudioTrack(sample_type, left_, top_, right_, top_ - AUDIO_TRACK_HEIGHT, this));
		}
		// Otherwise, place new audio track below existing tracks
		else {
			AudioTrack* last_audio_track = audio_tracks_[audio_tracks_.size() - 1];
			audio_tracks_.push_back(new AudioTrack(sample_type, left_, last_audio_track->get_bottom(), right_, last_audio_track->get_bottom() - AUDIO_TRACK_HEIGHT, this));
		}
	}

void ArrangeWindow::AddPattern(SampleType sample_type, int start_measure, int end_measure) {
	AudioFile* file = 0;
	bool add = false;
	int start_in_samples = 0, end_in_samples = 0, increment = 0;
	switch (sample_type) {
		case KICK:
			// Get random kick sample
			file = AudioFile::GetRandomSampleFromDir(KICK_DIR_PATH);
			// Find the kick audio track
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					start_in_samples = start_measure * num_samples_per_measure();
					end_in_samples = end_measure * num_samples_per_measure();
					increment = num_samples_per_measure() / 4.0;
					for (int j=start_in_samples; j<end_in_samples; j += increment) {
						audio_tracks_[i]->AddAudioClip(j, file);
					}
				}
			}
			break;
		case CLAP:
//			 Get random clap sample
			file = AudioFile::GetRandomSampleFromDir(CLAP_DIR_PATH);
//			 Find the clap audio track
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					start_in_samples = start_measure * num_samples_per_measure();
					end_in_samples = end_measure * num_samples_per_measure();
					increment = num_samples_per_measure() / 4.0;
					for (int j=start_in_samples; j<end_in_samples; j += increment) {
						if (add) {
							audio_tracks_[i]->AddAudioClip(j, file);
						}
						add = !add;
					}
				}
			}
			break;
		case SNARE:
//			 Get random snare sample
			file = AudioFile::GetRandomSampleFromDir(SNARE_DIR_PATH);
//			 Find the snare audio track
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					start_in_samples = start_measure * num_samples_per_measure();
					end_in_samples = end_measure * num_samples_per_measure();
					increment = num_samples_per_measure() / 2.0;
					for (int j=start_in_samples; j<end_in_samples; j += increment) {
						if (add) {
							audio_tracks_[i]->AddAudioClip(j, file);
						}
						add = !add;
					}
				}
			}
			break;
		case HAT:
//			 Get random hat sample
			file = AudioFile::GetRandomSampleFromDir(HAT_DIR_PATH);
//			 Find the hat audio track
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					start_in_samples = start_measure * num_samples_per_measure();
					end_in_samples = end_measure * num_samples_per_measure();
					increment = num_samples_per_measure() / 16.0;
					for (int j=start_in_samples; j<end_in_samples; j += increment) {
						if (rand()%100 > 50) {
							audio_tracks_[i]->AddAudioClip(j, file);
						}
					}
				}
			}
			break;
		case HIGH_IMPACT:
			// Get random high impact sample
			file = AudioFile::GetRandomSampleFromDir(HIGH_IMPACT_DIR_PATH);
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					start_in_samples = start_measure * num_samples_per_measure();
					audio_tracks_[i]->AddAudioClip(start_in_samples, file);
				}
			}
			break;
		case LOW_IMPACT:
			// Get random low impact sample
			file = AudioFile::GetRandomSampleFromDir(LOW_IMPACT_DIR_PATH);
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					start_in_samples = start_measure * num_samples_per_measure();
					audio_tracks_[i]->AddAudioClip(start_in_samples, file);
				}
			}
			break;
		case PUNCHY_IMPACT:
			// Get random punchy impact sample
			file = AudioFile::GetRandomSampleFromDir(PUNCHY_IMPACT_DIR_PATH);
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					start_in_samples = start_measure * num_samples_per_measure();
					audio_tracks_[i]->AddAudioClip(start_in_samples, file);
				}
			}
			break;
		case SWEEP_UP:
			// Get random sweep up sample
			file = AudioFile::GetRandomSampleFromDir(SWEEP_UP_DIR_PATH);
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					end_in_samples = end_measure * num_samples_per_measure();
					audio_tracks_[i]->AddAudioClip(end_in_samples - file->get_num_frames(), file);
				}
			}
			break;
		case SWEEP_DOWN:
			// Get random sweep down sample
			file = AudioFile::GetRandomSampleFromDir(SWEEP_DOWN_DIR_PATH);
			for (int i=0; i<audio_tracks_.size(); i++) {
				if (audio_tracks_[i]->get_sample_type() == sample_type) {
					start_in_samples = start_measure * num_samples_per_measure();
					audio_tracks_[i]->AddAudioClip(start_in_samples, file);
				}
			}
			break;
		default:
			break;
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
