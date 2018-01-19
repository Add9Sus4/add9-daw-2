/*
 * VerticalScrollBar.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#include "VerticalScrollBar.h"
#include "MainWindow.h"

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
		hasBeenClicked = true;
		lastY = y;
		diff = 0;
	}
	return true;
}
bool VerticalScrollBar::onDrag(double x, double y) {
	if (hidden) { return false; }
	double diffTemp = y - lastY;
	if (hasBeenClicked) {
		if (yOffset + diffTemp > 0.0) {
			diffTemp = -yOffset;
		} else if (yOffset + diffTemp - height() < -parentWindow->height()) {
			diffTemp = height() - parentWindow->height() - yOffset;
		}
		setRect(getL(), parentWindow->getT() + yOffset + diffTemp, getR(), parentWindow->getB() + yOffset + diffTemp);
		diff = diffTemp;

	}
	return true;
}
bool VerticalScrollBar::onUpClick(double x, double y) {
	if (hidden || !hasBeenClicked) { return false; }
		hasBeenClicked = false;
		yOffset += diff;
		diff = 0;
	return true;
}

} // namespace add9daw2
