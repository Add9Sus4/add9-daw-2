/*
 * AudioFile.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#include "audio_file.h"

namespace add9daw2 {

bool AudioFile::FillBlockWithAudio(float* empty_block, int frames_per_buffer, int channels) {
	// If the audio is mono and output is mono
	if (is_mono() && channels == MONO) {
		// If there is at least one full block of audio left to send
		if (get_num_frames() > current_position_ + frames_per_buffer) {
			double* audio = GetAudioFromChannel(LEFT, current_position_, current_position_ + frames_per_buffer);
			for (int i=0; i<frames_per_buffer; i++) {
				empty_block[i] = audio[i];
			}
			delete[] audio;
			// Advance position
			current_position_ += frames_per_buffer;
			return true;
		} else { // If there is less than one full block of audio left to send
			int remaining_frames = frames_per_buffer - (get_num_frames() - current_position_);
			double* audio = GetAudioFromChannel(LEFT, current_position_, current_position_ + remaining_frames);
			// Use up remaining audio
			for (int i=0; i<remaining_frames; i++) {
				empty_block[i] = audio[i];
			}
			// Send zeros to fill out the rest of the block
			for (int i=remaining_frames; i<frames_per_buffer; i++) {
				empty_block[i] = 0.0;
			}
			delete[] audio;
			// Set current position back to zero
			current_position_ = 0;
			// Set playing to false
			return false;
		}
	}
	// If the audio is mono and output is stereo
	if (is_mono() && channels == STEREO) {
		// If there is at least one full block of audio left to send
		if (get_num_frames() > current_position_ + frames_per_buffer) {
			double* audio = GetAudioFromChannel(LEFT, current_position_, current_position_ + frames_per_buffer);
			for (int i=0; i<frames_per_buffer; i++) {
				empty_block[2*i] = audio[i];
				empty_block[2*i + 1] = audio[i];
			}
			delete[] audio;
			// Advance position
			current_position_ += frames_per_buffer;
			return true;
		} else { // If there is less than one full block of audio left to send
			int remaining_frames = frames_per_buffer - (get_num_frames() - current_position_);
			double* audio = GetAudioFromChannel(LEFT, current_position_, current_position_ + remaining_frames);
			// Use up remaining audio
			for (int i=0; i<remaining_frames; i++) {
				empty_block[2*i] = audio[i];
				empty_block[2*i + 1] = audio[i];
			}
			// Send zeros to fill out the rest of the block
			for (int i=remaining_frames; i<frames_per_buffer; i++) {
				empty_block[2*i] = 0.0;
				empty_block[2*i + 1] = 0.0;
			}
			delete[] audio;
			// Set current position back to zero
			current_position_ = 0;
			// Set playing to false
			return false;
		}
	}
	// If the audio is stereo and output is mono
	if (is_stereo() && channels == MONO) {
		// If there is at least one full block of audio left to send
		if (get_num_frames() > current_position_ + frames_per_buffer) {
			double* audio_left = GetAudioFromChannel(LEFT, current_position_, current_position_ + frames_per_buffer);
			double* audio_right = GetAudioFromChannel(RIGHT, current_position_, current_position_ + frames_per_buffer);
			for (int i=0; i<frames_per_buffer; i++) {
				empty_block[i] = (audio_left[i] + audio_right[i]) / 2.0;
			}
			delete[] audio_left;
			delete[] audio_right;
			// Advance position
			current_position_ += frames_per_buffer;
			return true;
		} else { // If there is less than one full block of audio left to send
			int remaining_frames = frames_per_buffer - (get_num_frames() - current_position_);
			double* audio_left = GetAudioFromChannel(LEFT, current_position_, current_position_ + remaining_frames);
			double* audio_right = GetAudioFromChannel(RIGHT, current_position_, current_position_ + remaining_frames);
			// Use up remaining audio
			for (int i=0; i<remaining_frames; i++) {
				empty_block[i] = (audio_left[i] + audio_right[i]) / 2.0;;
			}
			// Send zeros to fill out the rest of the block
			for (int i=remaining_frames; i<frames_per_buffer; i++) {
				empty_block[i] = 0.0;
			}
			delete[] audio_left;
			delete[] audio_right;
			// Set current position back to zero
			current_position_ = 0;
			// Set playing to false
			return false;
		}
	}
	// If the audio is stereo and output is stereo
	if (is_stereo() && channels == STEREO) {
		// If there is at least one full block of audio left to send
		if (get_num_frames() > current_position_ + frames_per_buffer) {
			double* audio_left = GetAudioFromChannel(LEFT, current_position_, current_position_ + frames_per_buffer);
			double* audio_right = GetAudioFromChannel(RIGHT, current_position_, current_position_ + frames_per_buffer);
			for (int i=0; i<frames_per_buffer; i++) {
				empty_block[2*i] = audio_left[i];
				empty_block[2*i + 1] = audio_right[i];
			}
			delete[] audio_left;
			delete[] audio_right;
			// Advance position
			current_position_ += frames_per_buffer;
			return true;
		} else { // If there is less than one full block of audio left to send
			int remaining_frames = get_num_frames() - current_position_;
			double* audio_left = GetAudioFromChannel(LEFT, current_position_, current_position_ + remaining_frames);
			double* audio_right = GetAudioFromChannel(RIGHT, current_position_, current_position_ + remaining_frames);
			// Use up remaining audio
			for (int i=0; i<remaining_frames; i++) {
				empty_block[2*i] = audio_left[i];
				empty_block[2*i + 1] = audio_right[i];
			}
			// Send zeros to fill out the rest of the block
			for (int i=remaining_frames; i<frames_per_buffer; i++) {
				empty_block[2*i] = 0.0;
				empty_block[2*i + 1] = 0.0;
			}
			delete[] audio_left;
			delete[] audio_right;
			// Set current position back to zero
			current_position_ = 0;
			// Set playing to false
			return false;
		}
	}
	return false;
}

double* AudioFile::GetAudioFromChannel(int channel)
{
	return GetAudioFromChannel(channel, 0, num_frames_);
}
double* AudioFile::GetAudioFromChannel(int channel, int start_frame, int end_frame)
{
	std::cout << "getting audio starting from frame " << start_frame << " to " << end_frame << std::endl;
	int length = end_frame - start_frame;
	if (end_frame > audio_[LEFT].size()) {
		std::cout << "End frame (" << end_frame << ") must not be greater than the number of audio frames (" << audio_[LEFT].size() << ")." << std::endl;
		return nullptr;
	}
	// If the start frame is negative, fill all negative frames with zeros
	if (start_frame < 0) {
		if (channel == LEFT) {
			double* left = new double[length];
			for (int i=start_frame; i<0; i++) {
				left[i - start_frame] = 0.0;
			}
			 for (int i=0; i<end_frame; i++) {
				 left[i - start_frame] = audio_[LEFT][i];
			 }
			 return left;
		} else if (channel == RIGHT) {
			if (is_mono()) {
				std::cout << "Cannot access channel " << RIGHT << " for mono audio." << std::endl;
				return nullptr;
			}
			double* right = new double[length];
			for (int i=start_frame; i<0; i++) {
				right[i - start_frame] = 0.0;
			}
			for (int i=0; i<end_frame; i++) {
			 right[i - start_frame] = audio_[LEFT][i];
			}
			return right;
		} else if (channel == LR_AVG) {
			if (is_mono()) {
				std::cout << "Cannot access channel " << RIGHT << " for mono audio." << std::endl;
				return nullptr;
			}
			double *left_right_average = new double[length];
			for (int i=start_frame; i<0; i++) {
				left_right_average[i - start_frame] = 0.0;
			}
			for (int i=0; i<end_frame; i++) {
				left_right_average[i - start_frame] = (audio_[LEFT][i] + audio_[RIGHT][i]) / 2.0;
			}
			return left_right_average;
		} else {
			std::cout << "Channel value of " << channel << " is not supported." << std::endl;
			return nullptr;
		}
	}
	if (length < 1) {
		std::cout << "Length (" << length << ") must be at least 1." << std::endl;
		return nullptr;
	}
	if (channel == LEFT) {
		double *left = new double[length];
		std::copy(audio_[LEFT].begin() + start_frame, audio_[LEFT].begin() + end_frame, left);
		return left;
	} else if (channel == RIGHT) {
		if (is_mono()) {
			std::cout << "Cannot access channel " << RIGHT << " for mono audio." << std::endl;
			return nullptr;
		}
		double *right = new double[length];
		std::copy(audio_[RIGHT].begin() + start_frame, audio_[RIGHT].begin() + end_frame, right);
		return right;
	} else if (channel == LR_AVG) {
		if (is_mono()) {
			std::cout << "Cannot access channel " << RIGHT << " for mono audio." << std::endl;
			return nullptr;
		}
		double *left = new double[length];
		double *right = new double[length];
		double *left_right_average = new double[length];
		std::copy(audio_[LEFT].begin() + start_frame, audio_[LEFT].begin() + end_frame, left);
		std::copy(audio_[RIGHT].begin() + start_frame, audio_[RIGHT].begin() + end_frame, right);
		for (int i=0; i<num_frames_; i++) {
			left_right_average[i] = (left[i] + right[i])/2.0;
		}
		delete[] left;
		delete[] right;
		return left_right_average;
	} else {
		std::cout << "Channel value of " << channel << " is not supported." << std::endl;
		return nullptr;
	}
}

Rect AudioFile::Draw() {
	return Rect(left_, top_, right_, bottom_);
}

void AudioFile::DrawGhost(double x, double y, double width_of_sample_, double bpm_) {
	// If the audio has not been loaded, load it now
	if (!loaded_) {
		LoadAudio();
	}
	// If it's still not loaded, do not draw anything
	if (!loaded_) {
		return;
	} else {
		std::cout << "Drawing ghost for file: " << get_name() << std::endl;
		glColor3d(audio_file_ghost_.r,audio_file_ghost_.g,audio_file_ghost_.b);
		glBegin(GL_LINE_STRIP);
		int inc = get_num_frames()/100;
		for (int i=0; i<get_num_frames(); i+=inc) {
			glVertex2d(x + (double)width_of_sample_*i, y + audio_[LEFT][i]/20);
		}
		glEnd();
	}
}

Rect AudioFile::DrawBelow(Rect rect, double translate_amount) {
	translate_amount_ = translate_amount;
	top_ = rect.bottom;
	bottom_ = rect.bottom - FILE_HEIGHT;
	// Hide if the file is below the bottom of the arrange window
	if (bottom_ + translate_amount < get_parent()->get_parent()->get_bottom() ||
			top_ + translate_amount > get_parent()->get_bottom()) {
		set_hidden(true);
	} else {
		set_hidden(false);
	}
	// If the file is hidden, return the rect but don't draw the file.
	if (hidden_) {
		return Rect(left_, top_, right_, bottom_);
	}
	glColor3d(color.r, color.g, color.b);
	// Draw the rectangle around the folder
	// Draw the file name
	Font(GLUT_BITMAP_HELVETICA_10, (char*) name_.c_str(), left_ + 0.035, top_ - 0.035 + translate_amount);
	// Return the bottom of the folder if closed; otherwise, return the bottom of the last file in the list.
	return Rect(left_, top_, right_, bottom_);
}

void AudioFile::DrawInClip(double left, double top, double right, double bottom) {
	double width_of_clip = right - left;
	double width_per_frame = width_of_clip / get_num_frames();
	double midpoint_y = bottom + (top - bottom)/2.0;
	glColor3d(audio_file_clip_.r,audio_file_clip_.g,audio_file_clip_.b);
	glBegin(GL_LINE_STRIP);
	int inc = get_num_frames()/100;
	for (int i=0; i<get_num_frames(); i+=inc) {
		glVertex2d(left + (double)width_per_frame*i, midpoint_y + audio_[LEFT][i]/20);
	}
	glEnd();
}

bool AudioFile::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
	// Mouse cannot interact with hidden audio files
	if (hidden_) {
		return false;
	}
	if (!contains(mouse)) {
		ResetColor();
		return false;
	}
	color = selected;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "File " << name_ << " received click" << std::endl;
			break;
		case DOUBLE_CLICK:
			std::cout << "File " << name_ << " received double click" << std::endl;
			break;
		case DRAG:
			if (mouse->file == 0) {
				mouse->file = this;
			}
			std::cout << "Dragging in file " << name_ << std::endl;
			break;
		case HOVER:
			std::cout << "Hovering in file " << name_ << std::endl;
			break;
		case RELEASE:
			std::cout << "File " << name_ << " received mouse release" << std::endl;
			mouse->ClearFile();
			break;
		default:
			break;
	}
	return true;
}
void AudioFile::Font(void *font, char *text, double x, double y) {
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
void AudioFile::InvertPhase()
{
	if (is_mono()) {
		std::transform(audio_[LEFT].begin(), audio_[LEFT].end(), audio_[LEFT].begin(),
		               std::bind1st(std::multiplies<double>(),-1.0));
	} else if (is_stereo()) {
		std::transform(audio_[LEFT].begin(), audio_[LEFT].end(), audio_[LEFT].begin(),
				               std::bind1st(std::multiplies<double>(),-1.0));
		std::transform(audio_[RIGHT].begin(), audio_[LEFT].end(), audio_[LEFT].begin(),
				               std::bind1st(std::multiplies<double>(),-1.0));
	}
}
void AudioFile::LoadAudio() {
	std::cout << "Loading audio..." << std::endl;
	std::string full_name = dir_ + "/" + name_;
	SF_INFO file_info;
	SNDFILE* file = sf_open(full_name.c_str(), SFM_READ, &file_info);

	if (file == 0) {
		std::cout << "Error: could not open file: " << name_ << std::endl;
		puts(sf_strerror(0));
		name_ = "Error";
	} else {
		num_channels_ = file_info.channels;
		num_frames_ = file_info.frames;
		sample_rate_ = file_info.samplerate;

		if (is_mono()) {
			double *temp = new double[num_frames_];
			sf_read_double(file, temp, num_frames_);
			std::vector<double> data(temp, temp + num_frames_);
			audio_.push_back(data);
		} else if (is_stereo()) {
			double *temp = new double[num_channels_*num_frames_];
			sf_read_double(file, temp, num_channels_*num_frames_);
			double *temp_left = new double[num_frames_];
			double *temp_right= new double[num_frames_];
			for (int i = 0; i < num_frames_; i++) {
				temp_left[i] = temp[2 * i];
				temp_right[i] = temp[2 * i + 1];
			}

			std::vector<double> data_left(temp_left, temp_left + num_frames_);
			std::vector<double> data_right(temp_right, temp_right + num_frames_);
			audio_.push_back(data_left);
			audio_.push_back(data_right);
		} else {
			std::cout << "Error: invalid number of channels (" << num_channels_ << ").\n"
					"Only files with 1 (mono) or 2 (stereo) channels of audio are supported." << std::endl;
			name_ = "Error";
		}
	}
	sf_close(file);
	loaded_ = true;
}
void AudioFile::Normalize()
{
	double max = 0.0;
	if (is_mono()) {
		for (int i=0; i<num_frames_; i++) {
			if (fabs((double)audio_[LEFT][i]) >= max) {
				max = fabs((double)audio_[LEFT][i]);
			}
		}
		for (int i=0; i<num_frames_; i++) {
			audio_[LEFT][i] /= max;
		}
	} else if (is_stereo()) {
		for (int i=0; i<num_frames_; i++) {
			if (fabs((double)audio_[LEFT][i]) >= max) {
				max = fabs((double)audio_[LEFT][i]);
			}
			if (fabs((double)audio_[RIGHT][i]) >= max) {
				max = fabs((double)audio_[RIGHT][i]);
			}
		}
		for (int i=0; i<num_frames_; i++) {
			audio_[LEFT][i] /= max;
			audio_[RIGHT][i] /= max;
		}
	}
}
void AudioFile::RemoveLeadingZeros()
{
	RemoveLeadingZeros(ZERO_REMOVAL_THRESHOLD);
}
void AudioFile::RemoveLeadingZeros(double threshold)
{
	if (is_mono()) {
		while (audio_[LEFT].size() > 0 &&
				fabs((double)audio_[LEFT][0]) <= threshold) {
			audio_[LEFT].erase(audio_[LEFT].begin());
		}
	} else if (is_stereo()) {
		while (audio_[LEFT].size() > 0 &&
				fabs((double)audio_[LEFT][0]) <= threshold &&
				fabs((double)audio_[RIGHT][0]) <= threshold) {
			audio_[LEFT].erase(audio_[LEFT].begin());
			audio_[RIGHT].erase(audio_[RIGHT].begin());
		}
	}
	num_frames_ = audio_[LEFT].size();
}
void AudioFile::RemoveTrailingZeros()
{
	RemoveTrailingZeros(ZERO_REMOVAL_THRESHOLD);
}
void AudioFile::RemoveTrailingZeros(double threshold)
{
	if (is_mono()) {
		for (int i=audio_[LEFT].size()-1; i>0; i--) {
			if (fabs((double)audio_[LEFT][i]) >= 0.00001) {
				break;
			}
			audio_[LEFT].pop_back();
		}
	} else if (is_stereo()) {
		for (int i=audio_[LEFT].size()-1; i>0; i--) {
			if (fabs((double)audio_[LEFT][i]) >= 0.00001 ||
				fabs((double)audio_[RIGHT][i]) >= 0.00001) {
				break;
			}
			audio_[LEFT].pop_back();
			audio_[RIGHT].pop_back();
		}
	}
	num_frames_ = audio_[LEFT].size();
}
void AudioFile::TrimFromEnd(int num_samples)
{
	if (is_mono()) {
		for (int i=0; i<num_samples; i++) {
			audio_[LEFT].pop_back();
		}
	} else if (is_stereo()) {
		for (int i=0; i<num_samples; i++) {
			audio_[LEFT].pop_back();
			audio_[RIGHT].pop_back();
		}
	}
	num_frames_ = audio_[LEFT].size();
}
void AudioFile::ZeroPad()
{
	int new_length = CalculateNextPowerOfTwo(num_frames_);
	ZeroPad(new_length);
}
void AudioFile::ZeroPad(int new_length)
{
	if (new_length < num_frames_) {
		std::cout << "New length must be greater than the current file "
				"length (" << num_frames_ << ") in order to zero-pad."
				<< std::endl;
		return;
	}
	if (is_mono()) {
		audio_[LEFT].resize(new_length);
	} else if (is_stereo()) {
		audio_[LEFT].resize(new_length);
		audio_[RIGHT].resize(new_length);
	}
	num_frames_ = new_length;
}

} // namespace add9daw2
