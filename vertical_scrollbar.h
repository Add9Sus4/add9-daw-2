/*
 * vertical_scrollbar.h
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#ifndef VERTICAL_SCROLLBAR_H_
#define VERTICAL_SCROLLBAR_H_

#define VERTICAL_SCROLLBAR_WIDTH	0.05
#define VERTICAL_SCROLLBAR_PADDING	0.01
#define DRAG_AREA_HEIGHT			0.2

#include "glut/GLUT.h"

#include <iostream>

#include "window.h"

namespace add9daw2 {

class VerticalScrollbar : public Window {
public:
	VerticalScrollbar(double left, double top, double right, double bottom, Window* parent) :
		Window(left, top, right, bottom, parent), offset_y_(0.0), last_position_(top), dragging_(false) {
		drag_area_ = new Rect(left_, top_, right_, top_ - DRAG_AREA_HEIGHT);
	};
	virtual ~VerticalScrollbar();
	Rect Draw() override;
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	inline double get_offset_y() { return offset_y_; }
	inline double get_scroll_amount() {
		return last_position_ - top_ - offset_y_; }
	inline bool inDragArea(Mouse* mouse) {
		return mouse->x > drag_area_->left && mouse->x < drag_area_->right
				&& mouse->y > drag_area_->bottom && mouse->y < drag_area_->top;
	}
	inline void set_drag_area(double left, double top, double right, double bottom) {
		drag_area_->left = left;
		drag_area_->top = top;
		drag_area_->right = right;
		drag_area_->bottom = bottom;
	}
	inline void reset() {
		offset_y_ = 0.0;
		last_position_ = top_;
		dragging_ = false;
		set_drag_area(left_, top_, right_, top_ - DRAG_AREA_HEIGHT);
	}
private:
	Rect* drag_area_;
	double offset_y_, last_position_;
	bool dragging_;
};

} /* namespace add9daw2 */

#endif /* VERTICAL_SCROLLBAR_H_ */
