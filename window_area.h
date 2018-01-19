/*
 * WindowArea.h
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_WINDOWAREA_H_
#define ADD9DAW2_SRC_WINDOWAREA_H_

#define PADDING						0.001
#define VERTICAL_SCROLL_BAR_WIDTH	0.01

#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

#include "Rect.h"

namespace add9daw2 {

class MainWindow;
class VerticalScrollBar;

class WindowArea {
public:
	WindowArea(double l, double t, double r, double b, MainWindow* main_window);
	virtual ~WindowArea() {}

	inline bool isHidden() { return hidden_; }
	inline virtual bool isInWindow(double x, double y) { return x > getL() && x < getR() && y < getT() && y > getB(); }
	virtual bool onClick(double x, double y);
	virtual bool onDoubleClick(double x, double y);
	virtual bool onDrag(double x, double y);
	virtual bool onUpClick(double x, double y);

	inline double getL() { return rect_->L(); }
	inline double getT() { return rect_->T(); }
	inline double getR() { return rect_->R(); }
	inline double getB() { return rect_->B(); }
	virtual double getMaxY();
	virtual double getMinY();
	double getScrollAmount();
	inline double height() { return rect_->height(); }
	inline double width() { return rect_->width(); }

	inline int numChildWindows() { return child_windows_.size(); }

	inline MainWindow* getMainWindow() { return main_window_; }

	inline Rect *getRect() { return rect_; }

	inline void addChildWindow(WindowArea* window_area) {
		child_windows_.push_back(window_area);
		window_area->setParentWindow(this);
	}
	virtual void draw(double x_offset, double y_offset);
	void Font(void *font, char *text, double x, double y);
	void hideScrollBar();
	inline void setHidden(bool hidden) { this->hidden_ = hidden; }
	inline void setL(double l) { rect_->setL(l); }
	inline void setT(double t) { rect_->setT(t); }
	inline void setR(double r) { rect_->setR(r); }
	inline void setB(double b) { rect_->setB(b); }
	inline void setName(std::string name) { this->name_ = name; }
	inline void setParentWindow(WindowArea* window_area) {this->parent_window_ = window_area; }
	inline void setRect(double l, double t, double r, double b) { rect_->set(l, t, r, b); }

	inline WindowArea* getChildWindows(int i) { return child_windows_[i]; }
	inline WindowArea* getParentWindow() { return parent_window_; }
protected:
	inline double normalizeCoord(double coord) { return coord * 2.0 - 1.0; }
private:
	bool hidden_;

	MainWindow* main_window_;

	Rect* rect_;

	std::string name_;

	std::vector<WindowArea*> child_windows_;

	VerticalScrollBar* vertical_scrollbar_;

	WindowArea* parent_window_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_WINDOWAREA_H_ */
