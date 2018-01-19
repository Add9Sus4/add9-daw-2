/*
 * WindowArea.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#include "window_area.h"

#include "main_window.h"
#include "vertical_scroll_bar.h"

namespace add9daw2 {

WindowArea::WindowArea(double l, double t, double r, double b, MainWindow* main_window) : hidden(false), main_window(main_window), parent_window(NULL) {
	srand(time(NULL));
	rect = new Rect(l + PADDING, t - PADDING, r - PADDING, b + PADDING);
	vertical_scrollbar = new VerticalScrollBar(r - VERTICAL_SCROLL_BAR_WIDTH + PADDING, t - PADDING, r - PADDING, b + PADDING, main_window);
	vertical_scrollbar->setParentWindow(this);
}

bool WindowArea::onClick(double x, double y) {
	if (hidden) { return false; }
	vertical_scrollbar->onClick(x, y);
	// Was the click in the window?
	if (x > getL() && x < getR() && y > getB() && y < getT()) {
//		cout << "Click recorded at " << x << ", " << y << endl;
//		if (!verticalScrollBar->isHidden()) {
//			cout << "Scrollbar at " << verticalScrollBar->getL() << ", " << verticalScrollBar->getT()
//					<< ", " << verticalScrollBar->getR() << ", " << verticalScrollBar->getB() << endl;
//		}

		// Check to see if a window inside this window was clicked on
		bool child_clicked = false;
		for (int i=0; i<child_windows.size(); i++) {
			if (child_windows[i]->onClick(x, y)) {
				child_clicked = true;
			}
		}
		return !child_clicked;
	} else {
		return false;
	}
}
bool WindowArea::onDoubleClick(double x, double y) {
	return false;
}
bool WindowArea::onDrag(double x, double y) {
	vertical_scrollbar->onDrag(x, y);
	return true;
}
bool WindowArea::onUpClick(double x, double y) {
	vertical_scrollbar->onUpClick(x, y);
	return true;
}

double WindowArea::getMaxY() {
	double max_y = getT();
	for (int i=0; i<child_windows.size(); i++) {
		if (true) {
			if (child_windows[i]->getT() > max_y) {
				max_y = child_windows[i]->getT();
			}
		}
	}
	return max_y;
}
double WindowArea::getMinY() {
	double min_y = getB();
	for (int i=0; i<child_windows.size(); i++) {
		if (true) {
			if (child_windows[i]->getB() < min_y) {
				min_y = child_windows[i]->getB();
			}
		}
	}
	return min_y;
}
double WindowArea::getScrollAmount() { return vertical_scrollbar->getScrollAmount(); }

void WindowArea::draw(double x_offset, double y_offset) {
	if (hidden) { return; }
	// First, calculate if any child windows extend outside this window
	double overflow_top = 0.0;
	double overflow_bottom = 0.0;
	for (int i=0; i<child_windows.size(); i++) {
		if (child_windows[i]->getMaxY() - getT() > overflow_top) {
			overflow_top = child_windows[i]->getMaxY() - getT();
		}
		if (getB() - child_windows[i]->getMinY() > overflow_bottom) {
			overflow_bottom = getB() - child_windows[i]->getMinY();
		}
	}

	// If there is vertical overflow, show vertical scroll bar
	if (overflow_top > 0.0 || overflow_bottom > 0.0) {
		std::cout << "overFlow: " << overflow_bottom << std::endl;
		vertical_scrollbar->setHidden(false);
		double scrollbar_height = height() * height() / (height() + overflow_bottom); // Set height of scroll bar
		vertical_scrollbar->setScrollRatio(overflow_bottom / (height() - scrollbar_height - PADDING*2));
		vertical_scrollbar->setRect(vertical_scrollbar->getL(), vertical_scrollbar->getT(),
				vertical_scrollbar->getR(), vertical_scrollbar->getT() - scrollbar_height);
	} else {
		vertical_scrollbar->setHidden(true);
	}
	// Draw vertical scroll bar
	vertical_scrollbar->draw(x_offset, y_offset);
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
	for (int i=0; i<child_windows.size(); i++) {
		child_windows[i]->draw(0.0, vertical_scrollbar->getScrollAmount());
	}
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
void WindowArea::hideScrollBar() { vertical_scrollbar->setHidden(true); }

} // namespace add9daw2
