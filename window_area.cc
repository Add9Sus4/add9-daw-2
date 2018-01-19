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

WindowArea::WindowArea(double l, double t, double r, double b, MainWindow* main_window) : hidden_(false), main_window_(main_window), parent_window_(NULL) {
	srand(time(NULL));
	rect_ = new Rect(l + PADDING, t - PADDING, r - PADDING, b + PADDING);
	vertical_scrollbar_ = new VerticalScrollBar(r - VERTICAL_SCROLL_BAR_WIDTH + PADDING, t - PADDING, r - PADDING, b + PADDING, main_window);
	vertical_scrollbar_->setParentWindow(this);
}

bool WindowArea::onClick(double x, double y) {
	if (hidden_) { return false; }
	vertical_scrollbar_->onClick(x, y);
	// Was the click in the window?
	if (x > getL() && x < getR() && y > getB() && y < getT()) {
//		cout << "Click recorded at " << x << ", " << y << endl;
//		if (!verticalScrollBar->isHidden()) {
//			cout << "Scrollbar at " << verticalScrollBar->getL() << ", " << verticalScrollBar->getT()
//					<< ", " << verticalScrollBar->getR() << ", " << verticalScrollBar->getB() << endl;
//		}

		// Check to see if a window inside this window was clicked on
		bool child_clicked = false;
		for (int i=0; i<child_windows_.size(); i++) {
			if (child_windows_[i]->onClick(x, y)) {
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
	vertical_scrollbar_->onDrag(x, y);
	return true;
}
bool WindowArea::onUpClick(double x, double y) {
	vertical_scrollbar_->onUpClick(x, y);
	return true;
}

double WindowArea::getMaxY() {
	double max_y = getT();
	for (int i=0; i<child_windows_.size(); i++) {
		if (true) {
			if (child_windows_[i]->getT() > max_y) {
				max_y = child_windows_[i]->getT();
			}
		}
	}
	return max_y;
}
double WindowArea::getMinY() {
	double min_y = getB();
	for (int i=0; i<child_windows_.size(); i++) {
		if (true) {
			if (child_windows_[i]->getB() < min_y) {
				min_y = child_windows_[i]->getB();
			}
		}
	}
	return min_y;
}
double WindowArea::getScrollAmount() { return vertical_scrollbar_->getScrollAmount(); }

void WindowArea::draw(double x_offset, double y_offset) {
	if (hidden_) { return; }
	// First, calculate if any child windows extend outside this window
	double overflow_top = 0.0;
	double overflow_bottom = 0.0;
	for (int i=0; i<child_windows_.size(); i++) {
		if (child_windows_[i]->getMaxY() - getT() > overflow_top) {
			overflow_top = child_windows_[i]->getMaxY() - getT();
		}
		if (getB() - child_windows_[i]->getMinY() > overflow_bottom) {
			overflow_bottom = getB() - child_windows_[i]->getMinY();
		}
	}

	// If there is vertical overflow, show vertical scroll bar
	if (overflow_top > 0.0 || overflow_bottom > 0.0) {
		std::cout << "overFlow: " << overflow_bottom << std::endl;
		vertical_scrollbar_->setHidden(false);
		double scrollbar_height = height() * height() / (height() + overflow_bottom); // Set height of scroll bar
		vertical_scrollbar_->setScrollRatio(overflow_bottom / (height() - scrollbar_height - PADDING*2));
		vertical_scrollbar_->setRect(vertical_scrollbar_->getL(), vertical_scrollbar_->getT(),
				vertical_scrollbar_->getR(), vertical_scrollbar_->getT() - scrollbar_height);
	} else {
		vertical_scrollbar_->setHidden(true);
	}
	// Draw vertical scroll bar
	vertical_scrollbar_->draw(x_offset, y_offset);
	glColor3d(0.86,0.92,0.94);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) name_.c_str(), normalizeCoord(getL() + 0.005 + x_offset), normalizeCoord(getT() - 0.015 + y_offset));
	glColor3d(0.3, 0.37, 0.40);
	glBegin(GL_LINE_STRIP);
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getT() + y_offset));
	glVertex2d(normalizeCoord(getR() + x_offset), normalizeCoord(getT() + y_offset));
	glVertex2d(normalizeCoord(getR() + x_offset), normalizeCoord(getB() + y_offset));
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getB() + y_offset));
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getT() + y_offset));
	glEnd();
	// Draw child windows
	for (int i=0; i<child_windows_.size(); i++) {
		child_windows_[i]->draw(0.0, vertical_scrollbar_->getScrollAmount());
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
void WindowArea::hideScrollBar() { vertical_scrollbar_->setHidden(true); }

} // namespace add9daw2
