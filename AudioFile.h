/*
 * AudioFile.h
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_AUDIOFILE_H_
#define ADD9DAW2_SRC_AUDIOFILE_H_

#define MONO	1
#define STEREO	2
#define LEFT	0
#define RIGHT	1
#define LR_AVG	2

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
		WindowArea(l, t, r, b, mainWindow), fileName_(fileName), numChannels_(MONO),
		 numFrames_(0), sampleRate_(44100), currentPosition_(0) {}
	virtual ~AudioFile() {}

	void loadAudio();

	inline std::string getFileName() { return fileName_; }

	inline int getNumChannels() { return numChannels_; }
	inline int getNumFrames() { return numFrames_; }
	inline int getSampleRate() { return sampleRate_; }

	double* getAudioFromChannel(int channel);
	double* getAudioFromChannel(int channel, int startFrame, int endFrame);

	void draw(double x_offset, double y_offset) override;

	bool onClick(double x, double y) override;

	void zeroPad(int newLength);
	void zeroPad();

	void removeLeadingZeros(double threshold);
	void removeTrailingZeros(double threshold);

	void removeLeadingZeros();
	void removeTrailingZeros();

	void trimFromEnd(int numSamples);

	void normalize();

	void invertPhase();

	bool getAudio(float* emptyBlock, int framesPerBuffer, int channels);

	inline bool isMono() { return numChannels_ == MONO; }
	inline bool isStereo() { return numChannels_ == STEREO; }

private:
	static const double ZERO_REMOVAL_THRESHOLD;

	std::vector<std::vector<double> > audio_;
	std::string fileName_;

	int numChannels_;
	int numFrames_;
	int sampleRate_;

	int currentPosition_;

};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_AUDIOFILE_H_ */
