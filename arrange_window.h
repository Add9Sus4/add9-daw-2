/*
 * arrange_window.h
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#ifndef ARRANGE_WINDOW_H_
#define ARRANGE_WINDOW_H_

#include "glut/GLUT.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <string>
#include <vector>

#define ZOOM_AREA_HEIGHT	0.05
#define ZOOM_AREA_PADDING	0.01

#include "audio_track.h"
#include "window.h"

namespace add9daw2 {

class ArrangeWindow : public Window {
public:
	ArrangeWindow(double left, double top, double right, double bottom, Window* parent);
	virtual ~ArrangeWindow();
	Rect Draw() override;
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	inline bool is_in_zoom_area(Mouse* mouse) {
		return mouse->x > zoom_area_->left && mouse->x < zoom_area_->right &&
				mouse->y > zoom_area_->bottom && mouse->y < zoom_area_->top;
	}
	// Returns audio data from all samples that are in the arrange window
	double* GetAudio(int frames_per_buffer, int channels);
	inline double WidthOfMeasure() { return get_width_of_sample() * 44100.0 * 60.0 * 4.0 / bpm_; }
	inline double get_width_of_sample() { return width_of_sample_ + width_of_sample_ * zoom_drag_amt_; }
	void AddAudioTrack();
	void AdvancePlaybackLocator(int frames_per_buffer);
private:
	bool playback_locator_selected_ = false;
	bool zoom_area_dragging_ = false;
	double width_of_sample_ = 0.000001, bpm_ = 140.0, zoom_drag_amt_ = 0.0;
	int playback_locator_ = 1;
	void Font(void *font, char *text, double x, double y);
	std::vector<AudioTrack*> audio_tracks_;
	Mouse* mouse_;
	Color playback_locator_color_init_ = {0.15, 0.25, 0.1};
	Color playback_locator_color_selected_ = {0.3, 0.5, 0.2};
	Color playback_locator_color_;
	Color zoom_area_color_init_ = {0.3, 0.3, 0.15};
	Color zoom_area_color_selected_ = {0.6, 0.6, 0.3};
	Color zoom_area_color_;
	Rect* zoom_area_;
	inline bool is_near_playback_locator(Mouse* mouse) {
		return fabs(mouse->x - left_ - width_of_sample_ * (double) playback_locator_) <= 0.01;
	}
};

} /* namespace add9daw2 */

#endif /* ARRANGE_WINDOW_H_ */
