/*
 * AudioFile.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#include "AudioFile.h"
#include "MainWindow.h"

namespace add9daw2 {

bool AudioFile::getAudio(float* emptyBlock, int framesPerBuffer, int channels) {
	// If the audio is mono and output is mono
	if (isMono() && channels == MONO) {
		// If there is at least one full block of audio left to send
		if (getNumFrames() > currentPosition_ + framesPerBuffer) {
			double* audio = getAudioFromChannel(LEFT, currentPosition_, currentPosition_ + framesPerBuffer);
			for (int i=0; i<framesPerBuffer; i++) {
				emptyBlock[i] = audio[i];
			}
			delete[] audio;
			// Advance position
			currentPosition_ += framesPerBuffer;
			return true;
		} else { // If there is less than one full block of audio left to send
			int remainingFrames = framesPerBuffer - (getNumFrames() - currentPosition_);
			double* audio = getAudioFromChannel(LEFT, currentPosition_, currentPosition_ + remainingFrames);
			// Use up remaining audio
			for (int i=0; i<remainingFrames; i++) {
				emptyBlock[i] = audio[i];
			}
			// Send zeros to fill out the rest of the block
			for (int i=remainingFrames; i<framesPerBuffer; i++) {
				emptyBlock[i] = 0.0;
			}
			delete[] audio;
			// Set current position back to zero
			currentPosition_ = 0;
			// Set playing to false
			return false;
		}
	}
	// If the audio is mono and output is stereo
	if (isMono() && channels == STEREO) {
		// If there is at least one full block of audio left to send
		if (getNumFrames() > currentPosition_ + framesPerBuffer) {
			double* audio = getAudioFromChannel(LEFT, currentPosition_, currentPosition_ + framesPerBuffer);
			for (int i=0; i<framesPerBuffer; i++) {
				emptyBlock[2*i] = audio[i];
				emptyBlock[2*i + 1] = audio[i];
			}
			delete[] audio;
			// Advance position
			currentPosition_ += framesPerBuffer;
			return true;
		} else { // If there is less than one full block of audio left to send
			int remainingFrames = framesPerBuffer - (getNumFrames() - currentPosition_);
			double* audio = getAudioFromChannel(LEFT, currentPosition_, currentPosition_ + remainingFrames);
			// Use up remaining audio
			for (int i=0; i<remainingFrames; i++) {
				emptyBlock[2*i] = audio[i];
				emptyBlock[2*i + 1] = audio[i];
			}
			// Send zeros to fill out the rest of the block
			for (int i=remainingFrames; i<framesPerBuffer; i++) {
				emptyBlock[2*i] = 0.0;
				emptyBlock[2*i + 1] = 0.0;
			}
			delete[] audio;
			// Set current position back to zero
			currentPosition_ = 0;
			// Set playing to false
			return false;
		}
	}
	// If the audio is stereo and output is mono
	if (isStereo() && channels == MONO) {
		// If there is at least one full block of audio left to send
		if (getNumFrames() > currentPosition_ + framesPerBuffer) {
			double* audioLeft = getAudioFromChannel(LEFT, currentPosition_, currentPosition_ + framesPerBuffer);
			double* audioRight = getAudioFromChannel(RIGHT, currentPosition_, currentPosition_ + framesPerBuffer);
			for (int i=0; i<framesPerBuffer; i++) {
				emptyBlock[i] = (audioLeft[i] + audioRight[i]) / 2.0;
			}
			delete[] audioLeft;
			delete[] audioRight;
			// Advance position
			currentPosition_ += framesPerBuffer;
			return true;
		} else { // If there is less than one full block of audio left to send
			int remainingFrames = framesPerBuffer - (getNumFrames() - currentPosition_);
			double* audioLeft = getAudioFromChannel(LEFT, currentPosition_, currentPosition_ + remainingFrames);
			double* audioRight = getAudioFromChannel(RIGHT, currentPosition_, currentPosition_ + remainingFrames);
			// Use up remaining audio
			for (int i=0; i<remainingFrames; i++) {
				emptyBlock[i] = (audioLeft[i] + audioRight[i]) / 2.0;;
			}
			// Send zeros to fill out the rest of the block
			for (int i=remainingFrames; i<framesPerBuffer; i++) {
				emptyBlock[i] = 0.0;
			}
			delete[] audioLeft;
			delete[] audioRight;
			// Set current position back to zero
			currentPosition_ = 0;
			// Set playing to false
			return false;
		}
	}
	// If the audio is stereo and output is stereo
	if (isStereo() && channels == STEREO) {
		// If there is at least one full block of audio left to send
		if (getNumFrames() > currentPosition_ + framesPerBuffer) {
			double* audioLeft = getAudioFromChannel(LEFT, currentPosition_, currentPosition_ + framesPerBuffer);
			double* audioRight = getAudioFromChannel(RIGHT, currentPosition_, currentPosition_ + framesPerBuffer);
			for (int i=0; i<framesPerBuffer; i++) {
				emptyBlock[2*i] = (float) audioLeft[i];
				emptyBlock[2*i + 1] = (float) audioRight[i];
			}
			delete[] audioLeft;
			delete[] audioRight;
			// Advance position
			currentPosition_ += framesPerBuffer;
			return true;
		} else { // If there is less than one full block of audio left to send
			int remainingFrames = getNumFrames() - currentPosition_;
			double* audioLeft = getAudioFromChannel(LEFT, currentPosition_, currentPosition_ + remainingFrames);
			double* audioRight = getAudioFromChannel(RIGHT, currentPosition_, currentPosition_ + remainingFrames);
			// Use up remaining audio
			for (int i=0; i<remainingFrames; i++) {
				emptyBlock[2*i] = (float) audioLeft[i];
				emptyBlock[2*i + 1] = (float) audioRight[i];
			}
			// Send zeros to fill out the rest of the block
			for (int i=remainingFrames; i<framesPerBuffer; i++) {
				emptyBlock[2*i] = 0.0;
				emptyBlock[2*i + 1] = 0.0;
			}
			delete[] audioLeft;
			delete[] audioRight;
			// Set current position back to zero
			currentPosition_ = 0;
			// Set playing to false
			return false;
		}
	}
	return false;
}
bool AudioFile::onClick(double x, double y) {
	bool inWindow = isInWindow(x, y);
	// Check all window areas for click
	if (inWindow) {
		loadAudio();
		std::cout << fileName_ << " clicked!" << std::endl;
		getMainWindow()->setPlayingFile(this);
	}
	return inWindow;
}

double* AudioFile::getAudioFromChannel(int channel)
{
	return getAudioFromChannel(channel, 0, numFrames_);
}
double* AudioFile::getAudioFromChannel(int channel, int startFrame, int endFrame)
{
	int length = endFrame - startFrame;
	if (endFrame > audio_[LEFT].size()) {
		std::cout << "End frame (" << endFrame << ") must not be greater than the number of audio frames (" << audio_[LEFT].size() << ")." << std::endl;
		return nullptr;
	}
	if (startFrame < 0) {
		std::cout << "Start frame (" << startFrame << ") must be 0 or greater." << std::endl;
		return nullptr;
	}
	if (length < 1) {
		std::cout << "Length (" << length << ") must be at least 1." << std::endl;
		return nullptr;
	}
	if (channel == LEFT) {
		double *left = new double[length];
		std::copy(audio_[LEFT].begin() + startFrame, audio_[LEFT].begin() + endFrame, left);
		return left;
	} else if (channel == RIGHT) {
		if (isMono()) {
			std::cout << "Cannot access channel " << RIGHT << " for mono audio." << std::endl;
			return nullptr;
		}
		double *right = new double[length];
		std::copy(audio_[RIGHT].begin() + startFrame, audio_[RIGHT].begin() + endFrame, right);
		return right;
	} else if (channel == LR_AVG) {
		if (isMono()) {
			std::cout << "Cannot access channel " << RIGHT << " for mono audio." << std::endl;
			return nullptr;
		}
		double *left = new double[length];
		double *right = new double[length];
		double *leftRightAverage = new double[length];
		std::copy(audio_[LEFT].begin() + startFrame, audio_[LEFT].begin() + endFrame, left);
		std::copy(audio_[RIGHT].begin() + startFrame, audio_[RIGHT].begin() + endFrame, right);
		for (int i=0; i<numFrames_; i++) {
			leftRightAverage[i] = (left[i] + right[i])/2.0;
		}
		delete[] left;
		delete[] right;
		return leftRightAverage;
	} else {
		std::cout << "Channel value of " << channel << " is not supported." << std::endl;
		return nullptr;
	}
}

void AudioFile::draw(double x_offset, double y_offset) {
	if (getL() + x_offset < getParentWindow()->getParentWindow()->getL() ||
		getT() + y_offset > getParentWindow()->getParentWindow()->getT() - 0.01 ||
		getR() + x_offset > getParentWindow()->getParentWindow()->getR() ||
		getB() + y_offset < getParentWindow()->getParentWindow()->getB()) {
		setHidden(true);
	} else {
		setHidden(false);
	}
	if (isHidden()) { return; }
	// Draw the folder name
//	string abbrevPath = fileName_.substr(fileName_.find("/"));
	glColor3d(0.6, 0.74, 0.80);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) fileName_.c_str(), normalizeCoord(getL() + 0.01 + x_offset), normalizeCoord(getT() - 0.01 + y_offset));
}
void AudioFile::invertPhase()
{
	if (isMono()) {
		std::transform(audio_[LEFT].begin(), audio_[LEFT].end(), audio_[LEFT].begin(),
		               std::bind1st(std::multiplies<double>(),-1.0));
	} else if (isStereo()) {
		std::transform(audio_[LEFT].begin(), audio_[LEFT].end(), audio_[LEFT].begin(),
				               std::bind1st(std::multiplies<double>(),-1.0));
		std::transform(audio_[RIGHT].begin(), audio_[LEFT].end(), audio_[LEFT].begin(),
				               std::bind1st(std::multiplies<double>(),-1.0));
	}
}
void AudioFile::loadAudio() {
	SF_INFO fileInfo;
	SNDFILE* file = sf_open(fileName_.c_str(), SFM_READ, &fileInfo);

	if (file == 0) {
		std::cout << "Error: could not open file: " << fileName_ << std::endl;
		puts(sf_strerror(0));
		fileName_ = "Error";
	} else {
		numChannels_ = fileInfo.channels;
		numFrames_ = fileInfo.frames;
		sampleRate_ = fileInfo.samplerate;

		if (isMono()) {
			double *temp = new double[numFrames_];
			sf_read_double(file, temp, numFrames_);
			std::vector<double> data(temp, temp + numFrames_);
			audio_.push_back(data);
		} else if (isStereo()) {
			double *temp = new double[numChannels_*numFrames_];
			sf_read_double(file, temp, numChannels_*numFrames_);
			double *tempLeft = new double[numFrames_];
			double *tempRight= new double[numFrames_];
			for (int i = 0; i < numFrames_; i++) {
				tempLeft[i] = temp[2 * i];
				tempRight[i] = temp[2 * i + 1];
			}

			std::vector<double> dataLeft(tempLeft, tempLeft + numFrames_);
			std::vector<double> dataRight(tempRight, tempRight + numFrames_);
			audio_.push_back(dataLeft);
			audio_.push_back(dataRight);
		} else {
			std::cout << "Error: invalid number of channels (" << numChannels_ << ").\n"
					"Only files with 1 (mono) or 2 (stereo) channels of audio are supported." << std::endl;
			fileName_ = "Error";
		}
	}
	sf_close(file);
}
void AudioFile::normalize()
{
	double max = 0.0;
	if (isMono()) {
		for (int i=0; i<numFrames_; i++) {
			if (fabs((double)audio_[LEFT][i]) >= max) {
				max = fabs((double)audio_[LEFT][i]);
			}
		}
		for (int i=0; i<numFrames_; i++) {
			audio_[LEFT][i] /= max;
		}
	} else if (isStereo()) {
		for (int i=0; i<numFrames_; i++) {
			if (fabs((double)audio_[LEFT][i]) >= max) {
				max = fabs((double)audio_[LEFT][i]);
			}
			if (fabs((double)audio_[RIGHT][i]) >= max) {
				max = fabs((double)audio_[RIGHT][i]);
			}
		}
		for (int i=0; i<numFrames_; i++) {
			audio_[LEFT][i] /= max;
			audio_[RIGHT][i] /= max;
		}
	}
}
void AudioFile::removeLeadingZeros()
{
	removeLeadingZeros(ZERO_REMOVAL_THRESHOLD);
}
void AudioFile::removeLeadingZeros(double threshold)
{
	if (isMono()) {
		while (audio_[LEFT].size() > 0 &&
				fabs((double)audio_[LEFT][0]) <= threshold) {
			audio_[LEFT].erase(audio_[LEFT].begin());
		}
	} else if (isStereo()) {
		while (audio_[LEFT].size() > 0 &&
				fabs((double)audio_[LEFT][0]) <= threshold &&
				fabs((double)audio_[RIGHT][0]) <= threshold) {
			audio_[LEFT].erase(audio_[LEFT].begin());
			audio_[RIGHT].erase(audio_[RIGHT].begin());
		}
	}
	numFrames_ = audio_[LEFT].size();
}
void AudioFile::removeTrailingZeros()
{
	removeTrailingZeros(ZERO_REMOVAL_THRESHOLD);
}
void AudioFile::removeTrailingZeros(double threshold)
{
	if (isMono()) {
		for (int i=audio_[LEFT].size()-1; i>0; i--) {
			if (fabs((double)audio_[LEFT][i]) >= 0.00001) {
				break;
			}
			audio_[LEFT].pop_back();
		}
	} else if (isStereo()) {
		for (int i=audio_[LEFT].size()-1; i>0; i--) {
			if (fabs((double)audio_[LEFT][i]) >= 0.00001 ||
				fabs((double)audio_[RIGHT][i]) >= 0.00001) {
				break;
			}
			audio_[LEFT].pop_back();
			audio_[RIGHT].pop_back();
		}
	}
	numFrames_ = audio_[LEFT].size();
}
void AudioFile::trimFromEnd(int numSamples)
{
	if (isMono()) {
		for (int i=0; i<numSamples; i++) {
			audio_[LEFT].pop_back();
		}
	} else if (isStereo()) {
		for (int i=0; i<numSamples; i++) {
			audio_[LEFT].pop_back();
			audio_[RIGHT].pop_back();
		}
	}
	numFrames_ = audio_[LEFT].size();
}
void AudioFile::zeroPad()
{
	int newLength = calculateNextPowerOfTwo(numFrames_);
	zeroPad(newLength);
}
void AudioFile::zeroPad(int newLength)
{
	if (newLength < numFrames_) {
		std::cout << "New length must be greater than the current file "
				"length (" << numFrames_ << ") in order to zero-pad."
				<< std::endl;
		return;
	}
	if (isMono()) {
		audio_[LEFT].resize(newLength);
	} else if (isStereo()) {
		audio_[LEFT].resize(newLength);
		audio_[RIGHT].resize(newLength);
	}
	numFrames_ = newLength;
}

} // namespace add9daw2
