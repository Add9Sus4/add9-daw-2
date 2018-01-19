/*
 * VerticalScrollBar.h
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_VERTICALSCROLLBAR_H_
#define ADD9DAW2_SRC_VERTICALSCROLLBAR_H_

#include <GLUT/glut.h>

#include "Rect.h"
#include "window_area.h"

class MainWindow;

namespace add9daw2 {

// Used to display sub-windows when they overflow vertically from their parent window.
class VerticalScrollBar {
public:
	VerticalScrollBar(double left, double top, double right, double bottom, MainWindow* main_window) :
		has_been_clicked_(false), hidden_(true), diff_(0.0), lastY_(0.0), scroll_ratio_(1.0), y_offset_(0.0), parent_window_(NULL) {
			rect_ = new Rect(left + PADDING, top - PADDING, right - PADDING, bottom + PADDING);
		}
	virtual ~VerticalScrollBar() {}

	// Returns true if the scrollbar is currently hidden.
	inline bool isHidden() { return hidden_; }

	// Handles click events in the scrollbar area.
	bool onClick(double x, double y);

	// Handles drag events in the scrollbar area.
	bool onDrag(double x, double y);

	// Handles mouse release events in the scrollbar area.
	bool onUpClick(double x, double y);

	// Returns the left bound of the scrollbar.
	inline double getL() { return rect_->get_left(); }

	// Returns the top bound of the scrollbar.
	inline double getT() { return rect_->get_top(); }

	// Returns the right bound of the scrollbar.
	inline double getR() { return rect_->get_right(); }

	// Returns the bottom bound of the scrollbar.
	inline double getB() { return rect_->get_bottom(); }

	// Returns the amount that windows inside the parent window should be shifted due to the current scrollbar position
	inline double getScrollAmount() { return (-y_offset_ - diff_)*scroll_ratio_; }

	// Returns the height of the scrollbar.
	inline double height() { return getT() - getB(); }

	// Handles drawing of the scrollbar.
	void draw(double x_offset, double y_offset);

	// Hides and shows the scrollbar.
	inline void setHidden(bool hidden) { this->hidden_ = hidden; }

	// Sets new bounds for the scrollbar.
	inline void setRect(double left, double top, double right, double bottom) { rect_->set(left, top, right, bottom); }

	// Sets the parent window for the scrollbar.
	inline void setParentWindow(WindowArea* window_area) {this->parent_window_ = window_area; }

	// Sets the ratio between the rate at which the scrollbar moves and the rate at which objects
	// inside the parent window move. This depends on how much the objects overflow from the parent window.
	inline void setScrollRatio(double scroll_ratio)  { this->scroll_ratio_ = scroll_ratio; }
private:
	bool has_been_clicked_, hidden_;

	double diff_, lastY_, scroll_ratio_, y_offset_;

	Rect* rect_;
	WindowArea* parent_window_;

	// Converts coordinates to those expected by OpenGL
	inline double normalizeCoord(double coord) { return coord * 2.0 - 1.0; }
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_VERTICALSCROLLBAR_H_ */
