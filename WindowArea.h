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

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include "Rect.h"

using namespace std;

class MainWindow;
class VerticalScrollBar;

class WindowArea {
public:
	WindowArea(double l, double t, double r, double b, MainWindow* mainWindow);
	virtual ~WindowArea() {}
	inline void setL(double l) { rect->setL(l); }
	inline void setT(double t) { rect->setT(t); }
	inline void setR(double r) { rect->setR(r); }
	inline void setB(double b) { rect->setB(b); }
	inline void setRect(double l, double t, double r, double b) { rect->set(l, t, r, b); }
	inline void addChildWindow(WindowArea* windowArea) {
		childWindows.push_back(windowArea);
		windowArea->setParentWindow(this);
	}
	void Font(void *font, char *text, double x, double y);
	inline void setName(string name) { this->name = name; }
	inline void setHidden(bool hidden) { this->hidden = hidden; }
	inline void setParentWindow(WindowArea* windowArea) {this->parentWindow = windowArea; }
	void hideScrollBar();

	virtual void draw(double x_offset, double y_offset);
	virtual bool onClick(double x, double y);
	virtual bool onUpClick(double x, double y);
	virtual bool onDoubleClick(double x, double y);
	virtual bool onDrag(double x, double y);
	inline virtual bool isInWindow(double x, double y) { return x > getL() && x < getR() && y < getT() && y > getB(); }
	inline bool isHidden() { return hidden; }

	inline double getL() { return rect->L(); }
	inline double getT() { return rect->T(); }
	inline double getR() { return rect->R(); }
	inline double getB() { return rect->B(); }
	inline double width() { return rect->width(); }
	inline double height() { return rect->height(); }
	double getScrollAmount();
	virtual double getMaxY();
	virtual double getMinY();
	inline Rect *getRect() { return rect; }
	WindowArea* parentWindow;
	VerticalScrollBar* verticalScrollBar;
	vector<WindowArea*> childWindows;
protected:
	bool hidden;
	inline double normalizeCoord(double coord) { return coord * 2.0 - 1.0; }
	MainWindow* mainWindow;
private:
	Rect* rect;
	string name;
};

#endif /* ADD9DAW2_SRC_WINDOWAREA_H_ */
