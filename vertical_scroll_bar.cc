/*
 * VerticalScrollBar.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#include "vertical_scroll_bar.h"

#include "main_window.h"

namespace add9daw2 {

void VerticalScrollBar::draw(double x_offset, double y_offset) {
	if (hidden_) { return; }
	glColor3d(0.3, 0.37, 0.40);
	glBegin(GL_LINE_STRIP);
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getT() + y_offset));
	glVertex2d(normalizeCoord(getR() + x_offset), normalizeCoord(getT() + y_offset));
	glVertex2d(normalizeCoord(getR() + x_offset), normalizeCoord(getB() + y_offset));
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getB() + y_offset));
	glVertex2d(normalizeCoord(getL() + x_offset), normalizeCoord(getT() + y_offset));
	glEnd();

}
bool VerticalScrollBar::onClick(double x, double y) {
	if (hidden_) { return false; }
	if (x > getL() && x < getR() && y < getT() && y > getB()) {
		std::cout << "Scroll bar clicked" << std::endl;
		has_been_clicked_ = true;
		lastY_ = y;
		diff_ = 0;
	}
	return true;
}
bool VerticalScrollBar::onDrag(double x, double y) {
	if (hidden_) { return false; }
	double diff_temp = y - lastY_;
	if (has_been_clicked_) {
		if (y_offset_ + diff_temp > 0.0) {
			diff_temp = -y_offset_;
		} else if (y_offset_ + diff_temp - height() < -parent_window_->height()) {
			diff_temp = height() - parent_window_->height() - y_offset_;
		}
		setRect(getL(), parent_window_->getT() + y_offset_ + diff_temp, getR(), parent_window_->getB() + y_offset_ + diff_temp);
		diff_ = diff_temp;

	}
	return true;
}
bool VerticalScrollBar::onUpClick(double x, double y) {
	if (hidden_ || !has_been_clicked_) { return false; }
		has_been_clicked_ = false;
		y_offset_ += diff_;
		diff_ = 0;
	return true;
}

} // namespace add9daw2
