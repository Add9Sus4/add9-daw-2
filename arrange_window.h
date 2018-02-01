/*
 * arrange_window.h
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#ifndef ARRANGE_WINDOW_H_
#define ARRANGE_WINDOW_H_

#include "glut/GLUT.h"

#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <string>
#include <vector>

#define ZOOM_AREA_HEIGHT	0.05
#define ZOOM_AREA_PADDING	0.01

#include "audio_track.h"
#include "section.h"
#include "window.h"

namespace add9daw2 {

class ArrangeWindow : public Window {
public:
	ArrangeWindow(double left, double top, double right, double bottom, Window* parent);
	virtual ~ArrangeWindow();
	inline bool contains(Mouse* mouse) override {
			return mouse->x >= left_ && mouse->x <= right_ && mouse->y - translate_amount_ <= top_ && mouse->y - translate_amount_ >= bottom_;
	}
	Rect Draw() override;
	void AdjustBounds(int start_measure, int end_measure) override;
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	inline bool is_in_zoom_area(Mouse* mouse) {
		return mouse->x > zoom_area_->left && mouse->x < zoom_area_->right &&
				mouse->y > zoom_area_->bottom && mouse->y < zoom_area_->top;
	}
	// Returns audio data from all samples that are in the arrange window
	double* GetAudio(int frames_per_buffer, int channels);
	inline double WidthOfMeasure() { return get_width_of_sample() * 44100.0 * 60.0 * 4.0 / bpm_; }
	inline double get_width_of_sample() { return width_of_sample_ + width_of_sample_ * zoom_drag_z_amt_; }
	inline int num_samples_per_measure() { return 4.0 * 60.0 * 44100 / bpm_; }
	void AddAudioTrack(SampleType sample_type);
	void AddPattern(SampleType sample_type, int start_measure, int end_measure);
	inline void AddSection(Section* section) { sections_.push_back(section); }
	void AdvancePlaybackLocator(int frames_per_buffer);
	inline void set_x_offset(double x_offset) { x_offset_ = x_offset; }
	inline double get_x_offset() {
		return x_offset_ + zoom_drag_x_amt_; }
	inline void set_zoom_drag_x_amt(double zoom) { zoom_drag_x_amt_ = zoom; }
	inline void test() { std::cout << "Test" << std::endl; }
private:
	bool playback_locator_selected_ = false;
	bool zoom_area_dragging_ = false;
	double width_of_sample_ = 0.0000003, bpm_ = 140.0, zoom_drag_z_amt_ = 0.0;

	int playback_locator_ = 1;
	std::vector<AudioTrack*> audio_tracks_;
	Mouse* mouse_;
	Color playback_locator_color_init_ = {0.15, 0.25, 0.1};
	Color playback_locator_color_selected_ = {0.3, 0.5, 0.2};
	Color playback_locator_color_;
	Color zoom_area_color_init_ = {0.3, 0.3, 0.15};
	Color zoom_area_color_selected_ = {0.6, 0.6, 0.3};
	Color zoom_area_color_;
	Rect* zoom_area_;
	std::vector<Section*> sections_;
	double x_offset_ = 0.0;
	double zoom_drag_x_amt_ = 0.0;
	inline bool is_near_playback_locator(Mouse* mouse) {
		return fabs(mouse->x - left_ - width_of_sample_ * (double) playback_locator_ + x_offset_) <= 0.01;
	}
};

} /* namespace add9daw2 */

#endif /* ARRANGE_WINDOW_H_ */
