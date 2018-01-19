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

	inline bool isHidden() { return hidden; }
	inline virtual bool isInWindow(double x, double y) { return x > getL() && x < getR() && y < getT() && y > getB(); }
	virtual bool onClick(double x, double y);
	virtual bool onDoubleClick(double x, double y);
	virtual bool onDrag(double x, double y);
	virtual bool onUpClick(double x, double y);

	inline double getL() { return rect->L(); }
	inline double getT() { return rect->T(); }
	inline double getR() { return rect->R(); }
	inline double getB() { return rect->B(); }
	virtual double getMaxY();
	virtual double getMinY();
	double getScrollAmount();
	inline double height() { return rect->height(); }
	inline double width() { return rect->width(); }

	inline int numChildWindows() { return child_windows.size(); }

	inline MainWindow* getMainWindow() { return main_window; }

	inline Rect *getRect() { return rect; }

	inline void addChildWindow(WindowArea* window_area) {
		child_windows.push_back(window_area);
		window_area->setParentWindow(this);
	}
	virtual void draw(double x_offset, double y_offset);
	void Font(void *font, char *text, double x, double y);
	void hideScrollBar();
	inline void setHidden(bool hidden) { this->hidden = hidden; }
	inline void setL(double l) { rect->setL(l); }
	inline void setT(double t) { rect->setT(t); }
	inline void setR(double r) { rect->setR(r); }
	inline void setB(double b) { rect->setB(b); }
	inline void setName(std::string name) { this->name = name; }
	inline void setParentWindow(WindowArea* window_area) {this->parent_window = window_area; }
	inline void setRect(double l, double t, double r, double b) { rect->set(l, t, r, b); }

	inline WindowArea* getChildWindows(int i) { return child_windows[i]; }
	inline WindowArea* getParentWindow() { return parent_window; }
protected:
	inline double normalizeCoord(double coord) { return coord * 2.0 - 1.0; }
private:
	bool hidden;

	MainWindow* main_window;

	Rect* rect;

	std::string name;

	std::vector<WindowArea*> child_windows;

	VerticalScrollBar* vertical_scrollbar;

	WindowArea* parent_window;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_WINDOWAREA_H_ */
