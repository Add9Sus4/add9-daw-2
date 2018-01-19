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
#include "WindowArea.h"

class MainWindow;

class VerticalScrollBar {
public:
	VerticalScrollBar(double l, double t, double r, double b, MainWindow* mainWindow) :
		hidden(true), hasBeenClicked(false), lastY(0.0), yOffset(0.0), diff(0.0), scrollRatio(1.0), mainWindow(mainWindow), parentWindow(NULL) {
			rect = new Rect(l + PADDING, t - PADDING, r - PADDING, b + PADDING);
		}
	virtual ~VerticalScrollBar() {}
	inline double getL() { return rect->L(); }
	inline double getT() { return rect->T(); }
	inline double getR() { return rect->R(); }
	inline double getB() { return rect->B(); }
	inline double getScrollAmount() { return (-yOffset - diff)*scrollRatio; }
	inline double height() { return getT() - getB(); }
	inline void setHidden(bool hidden) { this->hidden = hidden; }
	inline void setRect(double l, double t, double r, double b) { rect->set(l, t, r, b); }
	inline void setScrollRatio(double scrollRatio)  { this->scrollRatio = scrollRatio; }
	void draw(double x_offset, double y_offset);
	bool onClick(double x, double y);
	bool onUpClick(double x, double y);
	bool onDrag(double x, double y);
	inline bool isHidden() { return hidden; }
	inline void setParentWindow(WindowArea* windowArea) {this->parentWindow = windowArea; }
private:
	bool hidden;
	bool hasBeenClicked;
	double lastY;
	double yOffset, diff;
	double scrollRatio;
	MainWindow* mainWindow;
	WindowArea* parentWindow;
	Rect* rect;
	inline double normalizeCoord(double coord) { return coord * 2.0 - 1.0; }
};

#endif /* ADD9DAW2_SRC_VERTICALSCROLLBAR_H_ */
