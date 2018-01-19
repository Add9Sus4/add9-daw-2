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

class VerticalScrollBar {
public:
	VerticalScrollBar(double l, double t, double r, double b, MainWindow* main_window) :
		has_been_clicked_(false), hidden_(true), diff_(0.0), lastY_(0.0), scroll_ratio_(1.0), y_offset_(0.0), parent_window_(NULL) {
			rect_ = new Rect(l + PADDING, t - PADDING, r - PADDING, b + PADDING);
		}
	virtual ~VerticalScrollBar() {}

	inline bool isHidden() { return hidden_; }
	bool onClick(double x, double y);
	bool onDrag(double x, double y);
	bool onUpClick(double x, double y);

	inline double getL() { return rect_->L(); }
	inline double getT() { return rect_->T(); }
	inline double getR() { return rect_->R(); }
	inline double getB() { return rect_->B(); }
	inline double getScrollAmount() { return (-y_offset_ - diff_)*scroll_ratio_; }
	inline double height() { return getT() - getB(); }

	void draw(double x_offset, double y_offset);
	inline void setHidden(bool hidden) { this->hidden_ = hidden; }
	inline void setRect(double l, double t, double r, double b) { rect_->set(l, t, r, b); }
	inline void setParentWindow(WindowArea* window_area) {this->parent_window_ = window_area; }
	inline void setScrollRatio(double scroll_ratio)  { this->scroll_ratio_ = scroll_ratio; }
private:
	bool has_been_clicked_, hidden_;

	double diff_, lastY_, scroll_ratio_, y_offset_;

	Rect* rect_;
	WindowArea* parent_window_;

	inline double normalizeCoord(double coord) { return coord * 2.0 - 1.0; }
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_VERTICALSCROLLBAR_H_ */
