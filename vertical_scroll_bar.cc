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
	if (hidden) { return; }
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
	if (hidden) { return false; }
	if (x > getL() && x < getR() && y < getT() && y > getB()) {
		std::cout << "Scroll bar clicked" << std::endl;
		has_been_clicked = true;
		lastY = y;
		diff = 0;
	}
	return true;
}
bool VerticalScrollBar::onDrag(double x, double y) {
	if (hidden) { return false; }
	double diff_temp = y - lastY;
	if (has_been_clicked) {
		if (y_offset + diff_temp > 0.0) {
			diff_temp = -y_offset;
		} else if (y_offset + diff_temp - height() < -parent_window->height()) {
			diff_temp = height() - parent_window->height() - y_offset;
		}
		setRect(getL(), parent_window->getT() + y_offset + diff_temp, getR(), parent_window->getB() + y_offset + diff_temp);
		diff = diff_temp;

	}
	return true;
}
bool VerticalScrollBar::onUpClick(double x, double y) {
	if (hidden || !has_been_clicked) { return false; }
		has_been_clicked = false;
		y_offset += diff;
		diff = 0;
	return true;
}

} // namespace add9daw2
