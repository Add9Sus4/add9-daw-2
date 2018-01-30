/*
 * menu_option.cpp
 *
 *  Created on: Jan 30, 2018
 *      Author: aarondawson
 */

#include "menu_option.h"

namespace add9daw2 {

Rect MenuOption::Draw() {
	// Draw border
	// Draw the menu border
	glColor3d(color_.r, color_.g, color_.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_ + OPTION_PADDING, top_ - OPTION_PADDING);
	glVertex2d(right_ - OPTION_PADDING, top_ - OPTION_PADDING);
	glVertex2d(right_ - OPTION_PADDING, bottom_ + OPTION_PADDING);
	glVertex2d(left_ + OPTION_PADDING, bottom_ + OPTION_PADDING);
	glVertex2d(left_ + OPTION_PADDING, top_ - OPTION_PADDING);
	glEnd();
	// Draw label
	Font(GLUT_BITMAP_HELVETICA_10, (char*) label_.c_str(), left_ + 0.035, top_ - 0.035);
	return Rect(left_, top_, right_, bottom_);
}

bool MenuOption::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
	if (!contains(mouse)) {
		color_ = option_color_init_;
		return false;
	}
	color_ = option_color_selected_;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Menu received click" << std::endl;
			break;
		case DOUBLE_CLICK:
			std::cout << "Menu received double click" << std::endl;
			break;
		case DRAG:
			std::cout << "Dragging in menu" << std::endl;
			break;
		case HOVER:
			std::cout << "Hovering in menu" << std::endl;
			break;
		case RELEASE:
			std::cout << "Menu received mouse release" << std::endl;
			break;
		default:
			break;
	}
	return true;
}

void MenuOption::Font(void *font, char *text, double x, double y) {
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

} /* namespace add9daw2 */
