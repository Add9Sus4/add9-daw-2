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
		has_been_clicked(false), hidden(true), diff(0.0), lastY(0.0), scroll_ratio(1.0), y_offset(0.0), parent_window(NULL) {
			rect = new Rect(l + PADDING, t - PADDING, r - PADDING, b + PADDING);
		}
	virtual ~VerticalScrollBar() {}

	inline bool isHidden() { return hidden; }
	bool onClick(double x, double y);
	bool onDrag(double x, double y);
	bool onUpClick(double x, double y);

	inline double getL() { return rect->L(); }
	inline double getT() { return rect->T(); }
	inline double getR() { return rect->R(); }
	inline double getB() { return rect->B(); }
	inline double getScrollAmount() { return (-y_offset - diff)*scroll_ratio; }
	inline double height() { return getT() - getB(); }

	void draw(double x_offset, double y_offset);
	inline void setHidden(bool hidden) { this->hidden = hidden; }
	inline void setRect(double l, double t, double r, double b) { rect->set(l, t, r, b); }
	inline void setParentWindow(WindowArea* window_area) {this->parent_window = window_area; }
	inline void setScrollRatio(double scroll_ratio)  { this->scroll_ratio = scroll_ratio; }
private:
	bool has_been_clicked, hidden;

	double diff, lastY, scroll_ratio, y_offset;

	Rect* rect;
	WindowArea* parent_window;

	inline double normalizeCoord(double coord) { return coord * 2.0 - 1.0; }
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_VERTICALSCROLLBAR_H_ */
