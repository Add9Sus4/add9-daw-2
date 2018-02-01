/*
 * Window.h
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <iostream>

#include "color.h"
#include "mouse.h"

namespace add9daw2 {

class ArrangeWindow;

typedef enum MouseEventType {
	CLICK,
	DOUBLE_CLICK,
	DRAG,
	HOVER,
	RELEASE
} MouseEventType;

typedef struct Rect {
	Rect(double left, double top, double right, double bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	double height() { return top - bottom; }
	double width() { return right - left; }
	double left, top, right, bottom;
} Rect;

class Window {
public:
	Window(double left, double top, double right, double bottom, Window* parent) :
		hidden_(false), left_(left), top_(top), right_(right), bottom_(bottom),
		translate_amount_(0.0), parent_(parent) {
		color_ = color_init_;
	}
	virtual ~Window();

	// Returns true if the mouse is within this window
	virtual inline bool contains(Mouse* mouse) {
		return mouse->x >= left_ && mouse->x <= right_ && mouse->y - translate_amount_ <= top_ && mouse->y - translate_amount_ >= bottom_;
	}

	// For the arrange window
	virtual inline void AdjustBounds(int start_measure, int end_measure) {}
	inline void set_bottom(double bottom) { bottom_ = bottom; }
	inline void set_left(double left) { left_ = left; }
	inline void set_right(double right) { right_ = right; }
	inline void set_top(double top) { top_ = top; }
	inline void set_bounds(double left, double top, double right, double bottom) {
		left_ = left;
		top_ = top;
		right_ = right;
		bottom_ = bottom;
	}
	inline void set_hidden(bool hidden) { hidden_ = hidden; }
	inline void set_parent(Window* parent) { parent_ = parent; }



	// Draws the window; returns a Rect defining the bounds of the window.
	virtual Rect Draw() = 0;
	virtual bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) = 0;
	inline void ResetColor() { color_ = color_init_; }
	inline virtual void ResetScrollbar() {}
	inline virtual void ResetAll() {}
	inline void set_translate_amount(double translate_amount) { translate_amount_ = translate_amount; }

	inline double get_bottom() { return bottom_; }
	inline double get_top() { return top_; }
	inline double get_left() { return left_; }
	inline double get_right() { return right_; }

	inline double height() { return top_ - bottom_; }
	inline double width() { return right_ - left_; }
	virtual double get_overflow_y() { return 0; }

	inline Window* get_parent() { return parent_; }
protected:
	bool hidden_;
	double left_, top_, right_, bottom_, translate_amount_;
	Color color_;
	Color color_selected_ = {0.6, 0.7, 0.8};
	Color color_init_ = {0.3, 0.35, 0.4};
	Window* parent_;
};

} /* namespace add9daw2 */

#endif /* WINDOW_H_ */
