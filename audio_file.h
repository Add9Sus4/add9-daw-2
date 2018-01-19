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

class AudioFile : public WindowArea {
public:
	AudioFile(std::string file_name, double l, double t, double r, double b, MainWindow* main_window) :
		WindowArea(l, t, r, b, main_window), current_position_(0), num_channels_(MONO),
		 num_frames_(0), sample_rate_(44100), file_name_(file_name) {}
	virtual ~AudioFile() {}

	bool GetAudio(float* empty_block, int frames_per_buffer, int channels);
	inline bool is_mono() { return num_channels_ == MONO; }
	inline bool is_stereo() { return num_channels_ == STEREO; }
	bool OnClick(double x, double y) override;

	double* GetAudioFromChannel(int channel);
	double* GetAudioFromChannel(int channel, int start_frame, int end_frame);

	inline int get_num_channels() { return num_channels_; }
	inline int get_num_frames() { return num_frames_; }
	inline int get_sample_rate() { return sample_rate_; }

	inline std::string get_file_name() { return file_name_; }

	void Draw(double x_offset, double y_offset) override;
	void InvertPhase();
	void LoadAudio();
	void Normalize();
	void ZeroPad();
	void ZeroPad(int new_length);
	void RemoveLeadingZeros();
	void RemoveLeadingZeros(double threshold);
	void RemoveTrailingZeros();
	void RemoveTrailingZeros(double threshold);
	void TrimFromEnd(int num_samples);

private:
	int current_position_, num_channels_, num_frames_, sample_rate_;

	std::vector<std::vector<double> > audio_;
	std::string file_name_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_AUDIOFILE_H_ */
