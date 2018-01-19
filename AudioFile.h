/*
 * AudioFile.h
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_AUDIOFILE_H_
#define ADD9DAW2_SRC_AUDIOFILE_H_

#define MONO					1
#define STEREO					2
#define LEFT					0
#define RIGHT					1
#define LR_AVG					2
#define ZERO_REMOVAL_THRESHOLD	0.00001

#include <dirent.h>
#include <GLUT/glut.h>
#include <math.h>
#include <sndfile.h>
#include <unistd.h>

#include <string>
#include <vector>

#include "WindowArea.h"

namespace add9daw2 {

inline int calculateNextPowerOfTwo(int number) {
		unsigned int x = number;
		while (x != (x & (~x + 1))) {
			x++;
		}
		return x;
	}

class MainWindow;

class AudioFile : public WindowArea {
public:
	AudioFile(std::string fileName, double l, double t, double r, double b, MainWindow* mainWindow) :
		WindowArea(l, t, r, b, mainWindow), currentPosition_(0), numChannels_(MONO),
		 numFrames_(0), sampleRate_(44100), fileName_(fileName) {}
	virtual ~AudioFile() {}

	bool getAudio(float* emptyBlock, int framesPerBuffer, int channels);
	inline bool isMono() { return numChannels_ == MONO; }
	inline bool isStereo() { return numChannels_ == STEREO; }
	bool onClick(double x, double y) override;

	double* getAudioFromChannel(int channel);
	double* getAudioFromChannel(int channel, int startFrame, int endFrame);

	inline int getNumChannels() { return numChannels_; }
	inline int getNumFrames() { return numFrames_; }
	inline int getSampleRate() { return sampleRate_; }

	inline std::string getFileName() { return fileName_; }

	void draw(double x_offset, double y_offset) override;
	void invertPhase();
	void loadAudio();
	void normalize();
	void zeroPad();
	void zeroPad(int newLength);
	void removeLeadingZeros();
	void removeLeadingZeros(double threshold);
	void removeTrailingZeros();
	void removeTrailingZeros(double threshold);
	void trimFromEnd(int numSamples);

private:
	int currentPosition_;
	int numChannels_;
	int numFrames_;
	int sampleRate_;

	std::vector<std::vector<double> > audio_;
	std::string fileName_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_AUDIOFILE_H_ */
