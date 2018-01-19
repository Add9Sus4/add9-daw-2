/*
 * WindowArea.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#include "MainWindow.h"
#include "WindowArea.h"
#include "VerticalScrollBar.h"

namespace add9daw2 {

WindowArea::WindowArea(double l, double t, double r, double b, MainWindow* mainWindow) : hidden(false) {
	srand(time(NULL));
	rect = new Rect(l + PADDING, t - PADDING, r - PADDING, b + PADDING);
	this->mainWindow = mainWindow;
	verticalScrollBar = new VerticalScrollBar(r - VERTICAL_SCROLL_BAR_WIDTH + PADDING, t - PADDING, r - PADDING, b + PADDING, mainWindow);
	verticalScrollBar->setParentWindow(this);
	parentWindow = NULL;
}

void WindowArea::Font(void *font, char *text, double x, double y) {
	glDisable(GL_LIGHTING);
	char buf[20];
	snprintf(buf, 20, "%s", text);
	glRasterPos2d(x, y);
	while ( *text != '\0') {
		glutBitmapCharacter(font, *text);
		++text;
	}
	glEnable(GL_LIGHTING);
}
void WindowArea::hideScrollBar() { verticalScrollBar->setHidden(true); }

void WindowArea::draw(double x_offset, double y_offset) {
	if (hidden) { return; }
	// First, calculate if any child windows extend outside this window
	double overFlowTop = 0.0;
	double overFlowBottom = 0.0;
	for (int i=0; i<childWindows.size(); i++) {
		if (childWindows[i]->getMaxY() - getT() > overFlowTop) {
			overFlowTop = childWindows[i]->getMaxY() - getT();
		}
		if (getB() - childWindows[i]->getMinY() > overFlowBottom) {
			overFlowBottom = getB() - childWindows[i]->getMinY();
		}
	}

	// If there is vertical overflow, show vertical scroll bar
	if (overFlowTop > 0.0 || overFlowBottom > 0.0) {
		std::cout << "overFlow: " << overFlowBottom << std::endl;
		verticalScrollBar->setHidden(false);
		double scrollBarHeight = height() * height() / (height() + overFlowBottom); // Set height of scroll bar
		verticalScrollBar->setScrollRatio(overFlowBottom / (height() - scrollBarHeight - PADDING*2));
		verticalScrollBar->setRect(verticalScrollBar->getL(), verticalScrollBar->getT(),
				verticalScrollBar->getR(), verticalScrollBar->getT() - scrollBarHeight);
	} else {
		verticalScrollBar->setHidden(true);
	}
	// Draw vertical scroll bar
	verticalScrollBar->draw(x_offset, y_offset);
	glColor3d(0.86,0.92,0.94);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) name.c_str(), normalizeCoord(getL() + 0.005 + x_offset), normalizeCoord(getT() - 0.015 + y_offset));
	glColor3d(0.3, 0.37, 0.40);
	glBegin(GL_LINE_STRIP);
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getT() + y_offset));
	glVertex2d(normalizeCoord(getR() + x_offset), normalizeCoord(getT() + y_offset));
	glVertex2d(normalizeCoord(getR() + x_offset), normalizeCoord(getB() + y_offset));
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getB() + y_offset));
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getT() + y_offset));
	glEnd();
	// Draw child windows
	for (int i=0; i<childWindows.size(); i++) {
		childWindows[i]->draw(0.0, verticalScrollBar->getScrollAmount());
	}
}
bool WindowArea::onClick(double x, double y) {
	if (hidden) { return false; }
	verticalScrollBar->onClick(x, y);
	// Was the click in the window?
	if (x > getL() && x < getR() && y > getB() && y < getT()) {
//		cout << "Click recorded at " << x << ", " << y << endl;
//		if (!verticalScrollBar->isHidden()) {
//			cout << "Scrollbar at " << verticalScrollBar->getL() << ", " << verticalScrollBar->getT()
//					<< ", " << verticalScrollBar->getR() << ", " << verticalScrollBar->getB() << endl;
//		}

		// Check to see if a window inside this window was clicked on
		bool childClicked = false;
		for (int i=0; i<childWindows.size(); i++) {
			if (childWindows[i]->onClick(x, y)) {
				childClicked = true;
			}
		}
		return !childClicked;
	} else {
		return false;
	}
}
bool WindowArea::onUpClick(double x, double y) {
	verticalScrollBar->onUpClick(x, y);
	return true;
}
bool WindowArea::onDoubleClick(double x, double y) {
	return false;
}
bool WindowArea::onDrag(double x, double y) {
	verticalScrollBar->onDrag(x, y);
	return true;
}

double WindowArea::getScrollAmount() { return verticalScrollBar->getScrollAmount(); }
double WindowArea::getMaxY() {
	double maxY = getT();
	for (int i=0; i<childWindows.size(); i++) {
		if (true) {
			if (childWindows[i]->getT() > maxY) {
				maxY = childWindows[i]->getT();
			}
		}
	}
	return maxY;
}
double WindowArea::getMinY() {
	double minY = getB();
	for (int i=0; i<childWindows.size(); i++) {
		if (true) {
			if (childWindows[i]->getB() < minY) {
				minY = childWindows[i]->getB();
			}
		}
	}
	return minY;
}

} // namespace add9daw2
