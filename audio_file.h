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
#define FILE_HEIGHT				0.03
#define FILE_PADDING			0

#include <dirent.h>
#include <GLUT/glut.h>
#include <math.h>
#include <sndfile.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "window.h"

namespace add9daw2 {

inline int CalculateNextPowerOfTwo(int number) {
		unsigned int x = number;
		while (x != (x & (~x + 1))) {
			x++;
		}
		return x;
	}

// Loads, stores, and manipulates digital audio.
// Use this class to get and use audio data from a file.
class AudioFile : public Window {
public:
	AudioFile(std::string file_name, std::string dir, double left, double top, double right, double bottom, Window* parent) :
			Window(left, top, right, bottom, parent),
			loaded_(false),
			current_position_(0.0),
			num_channels_(MONO),
			num_frames_(0),
			sample_rate_(44100),
			name_(file_name),
			dir_(dir) {}
	virtual ~AudioFile() {}

	// Fills a block with audio data.
	bool FillBlockWithAudio(float* empty_block, int frames_per_buffer, int channels);

	// Returns true if the file only has one channel.
	inline bool is_mono() { return num_channels_ == MONO; }

	// Returns true if the file has exactly two channels.
	inline bool is_stereo() { return num_channels_ == STEREO; }

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
	inline std::string get_name() { return name_; }

	Rect Draw() override;

	// Used to draw the audio file when it is being dragged
	void DrawGhost(double x, double y, double width_of_sample, double bpm);

	Rect DrawBelow(Rect rect, double translate_amount);

	// Used to draw the audio file in a clip in the arrange window
	void DrawInClip(double left, double top, double right, double bottom);

	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;

	void Font(void *font, char *text, double x, double y);

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
	bool loaded_;

	int current_position_, num_channels_, num_frames_, sample_rate_;

	std::vector<std::vector<double> > audio_;
	std::string name_;
	std::string dir_;

	Color audio_file_ghost_ = {0.3, 0.2, 0.25};
	Color audio_file_clip_ = {0.6, 0.4, 0.5};

};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_AUDIOFILE_H_ */
