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

WindowArea::WindowArea(double left, double top, double right, double bottom, MainWindow* main_window) :
		hidden_(false), main_window_(main_window), parent_window_(NULL) {
	srand(time(NULL));
	rect_ = new Rect(left + PADDING, top - PADDING, right - PADDING, bottom + PADDING);
	vertical_scrollbar_ = new VerticalScrollBar(right - VERTICAL_SCROLL_BAR_WIDTH + PADDING, top - PADDING, right - PADDING, bottom + PADDING, main_window);
	vertical_scrollbar_->setParentWindow(this);
}

bool WindowArea::OnClick(double x, double y) {
	if (hidden_) { return false; }
	vertical_scrollbar_->onClick(x, y);
	// Was the click in the window?
	if (x > get_left() && x < get_right() && y > get_bottom() && y < get_top()) {
//		cout << "Click recorded at " << x << ", " << y << endl;
//		if (!verticalScrollBar->isHidden()) {
//			cout << "Scrollbar at " << verticalScrollBar->getL() << ", " << verticalScrollBar->getT()
//					<< ", " << verticalScrollBar->getR() << ", " << verticalScrollBar->getB() << endl;
//		}

		// Check to see if a window inside this window was clicked on
		bool child_clicked = false;
		for (int i=0; i<child_windows_.size(); i++) {
			if (child_windows_[i]->OnClick(x, y)) {
				child_clicked = true;
			}
		}
		return !child_clicked;
	} else {
		return false;
	}
}
bool WindowArea::OnDoubleClick(double x, double y) {
	return false;
}
bool WindowArea::OnDrag(double x, double y) {
	vertical_scrollbar_->onDrag(x, y);
	return true;
}
bool WindowArea::OnUpClick(double x, double y) {
	vertical_scrollbar_->onUpClick(x, y);
	return true;
}

double WindowArea::get_max_y() {
	double max_y = get_top();
	for (int i=0; i<child_windows_.size(); i++) {
		if (true) {
			if (child_windows_[i]->get_top() > max_y) {
				max_y = child_windows_[i]->get_top();
			}
		}
	}
	return max_y;
}
double WindowArea::get_min_y() {
	double min_y = get_bottom();
	for (int i=0; i<child_windows_.size(); i++) {
		if (true) {
			if (child_windows_[i]->get_bottom() < min_y) {
				min_y = child_windows_[i]->get_bottom();
			}
		}
	}
	return min_y;
}
double WindowArea::get_scroll_amount() { return vertical_scrollbar_->getScrollAmount(); }

void WindowArea::Draw(double x_offset, double y_offset) {
	if (hidden_) { return; }
	// First, calculate if any child windows extend outside this window
	double overflow_top = 0.0;
	double overflow_bottom = 0.0;
	for (int i=0; i<child_windows_.size(); i++) {
		if (child_windows_[i]->get_max_y() - get_top() > overflow_top) {
			overflow_top = child_windows_[i]->get_max_y() - get_top();
		}
		if (get_bottom() - child_windows_[i]->get_min_y() > overflow_bottom) {
			overflow_bottom = get_bottom() - child_windows_[i]->get_min_y();
		}
	}

	// If there is vertical overflow, show vertical scroll bar
	if (overflow_top > 0.0 || overflow_bottom > 0.0) {
		std::cout << "overFlow: " << overflow_bottom << std::endl;
		vertical_scrollbar_->setHidden(false);
		double scrollbar_height = get_height() * get_height() / (get_height() + overflow_bottom); // Set height of scroll bar
		vertical_scrollbar_->setScrollRatio(overflow_bottom / (get_height() - scrollbar_height - PADDING*2));
		vertical_scrollbar_->setRect(vertical_scrollbar_->getL(), vertical_scrollbar_->getT(),
				vertical_scrollbar_->getR(), vertical_scrollbar_->getT() - scrollbar_height);
	} else {
		vertical_scrollbar_->setHidden(true);
	}
	// Draw vertical scroll bar
	vertical_scrollbar_->draw(x_offset, y_offset);
	glColor3d(0.86,0.92,0.94);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) name_.c_str(), NormalizeCoord(get_left() + 0.005 + x_offset), NormalizeCoord(get_top() - 0.015 + y_offset));
	glColor3d(0.3, 0.37, 0.40);
	glBegin(GL_LINE_STRIP);
	glVertex2d(NormalizeCoord(get_left() + x_offset), NormalizeCoord(get_top() + y_offset));
	glVertex2d(NormalizeCoord(get_right() + x_offset), NormalizeCoord(get_top() + y_offset));
	glVertex2d(NormalizeCoord(get_right() + x_offset), NormalizeCoord(get_bottom() + y_offset));
	glVertex2d(NormalizeCoord(get_left() + x_offset), NormalizeCoord(get_bottom() + y_offset));
	glVertex2d(NormalizeCoord(get_left() + x_offset), NormalizeCoord(get_top() + y_offset));
	glEnd();
	// Draw child windows
	for (int i=0; i<child_windows_.size(); i++) {
		child_windows_[i]->Draw(0.0, vertical_scrollbar_->getScrollAmount());
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
void WindowArea::HideScrollBar() { vertical_scrollbar_->setHidden(true); }

} // namespace add9daw2
