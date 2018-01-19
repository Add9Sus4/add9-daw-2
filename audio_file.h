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

#include "window_area.h"

namespace add9daw2 {

inline int CalculateNextPowerOfTwo(int number) {
		unsigned int x = number;
		while (x != (x & (~x + 1))) {
			x++;
		}
		return x;
	}

class MainWindow;

// Loads, stores, and manipulates digital audio.
// Use this class to get and use audio data from a file.
class AudioFile : public WindowArea {
public:
	AudioFile(std::string file_name, double l, double t, double r, double b, MainWindow* main_window) :
		WindowArea(l, t, r, b, main_window), current_position_(0), num_channels_(MONO),
		 num_frames_(0), sample_rate_(44100), file_name_(file_name) {}
	virtual ~AudioFile() {}

	// Fills a block with audio data.
	bool FillBlockWithAudio(float* empty_block, int frames_per_buffer, int channels);

	// Returns true if the file only has one channel.
	inline bool is_mono() { return num_channels_ == MONO; }

	// Returns true if the file has exactly two channels.
	inline bool is_stereo() { return num_channels_ == STEREO; }

	// Handles what happens when the audio file is clicked.
	bool OnClick(double x, double y) override;

	// Returns the entire amount of audio from the specified channel.
	double* GetAudioFromChannel(int channel);

	// Returns audio from the specified channel, within the specified bounds.
	double* GetAudioFromChannel(int channel, int start_frame, int end_frame);

	// Returns the number of channels in the audio file.
	inline int get_num_channels() { return num_channels_; }

	// Returns the number of frames in the audio file.
	inline int get_num_frames() { return num_frames_; }

	// Returns the sample rate of the audio file.
	inline int get_sample_rate() { return sample_rate_; }

	// Returns the file name of the audio file.
	inline std::string get_file_name() { return file_name_; }

	// Draws the file name on the screen.
	void Draw(double x_offset, double y_offset) override;

	// Flips the phase of the audio.
	void InvertPhase();

	// Actually loads the audio from the file. Until this is called,
	// audio cannot be played or manipulated from this class.
	void LoadAudio();

	// Adjusts amplitude of audio data so that its maximum is 1.0 or -1.0.
	void Normalize();

	// Increases the number of audio frames to the next power of 2, and
	// fills the additional space with zeros.
	void ZeroPad();

	// Increases the number of audio frames to the specified length, and
	// fills the additional space with zeros.
	void ZeroPad(int new_length);

	// Removes silence from the beginning of the file, using the default threshold.
	void RemoveLeadingZeros();

	// Removes silence from the beginning of the file, using the specified threshold.
	void RemoveLeadingZeros(double threshold);

	// Removes silence from the end of the file, using the default threshold.
	void RemoveTrailingZeros();

	// Removes silence from the beginning of the file, using the specified threshold.
	void RemoveTrailingZeros(double threshold);

	// Removes the specified number of samples from the end of the file
	void TrimFromEnd(int num_samples);

private:
	int current_position_, num_channels_, num_frames_, sample_rate_;

	std::vector<std::vector<double> > audio_;
	std::string file_name_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_AUDIOFILE_H_ */
