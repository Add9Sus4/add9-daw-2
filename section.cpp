/*
 * section.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: aarondawson
 */

#include "section.h"

namespace add9daw2 {

Section::~Section() {
	// TODO Auto-generated destructor stub
}

void Section::set_default_values(SectionType section_type) {
	switch (section_type) {
		case INTRO:
			name_ = "Intro";
			color_ = {0.7,0.5,0.4};
			break;
		case VERSE:
			name_ = "Verse";
			color_ = {0.7,0.4,0.5};
			break;
		case BUILD:
			name_ = "Build";
			color_ = {0.5,0.7,0.4};
			break;
		case DROP:
			name_ = "Drop";
			color_ = {0.5,0.4,0.7};
			break;
		case BREAK:
			name_ = "Break";
			color_ = {0.4,0.7,0.5};
			break;
		case BRIDGE:
			name_ = "Bridge";
			color_ = {0.4,0.5,0.7};
			break;
		case OUTRO:
			name_ = "Outro";
			color_ = {0.6,0.3,0.4};
			break;
		default:
			name_ = "N/A";
			color_ = {0.3,0.6,0.4};
			break;
	}
}

Rect Section::Draw() {
	set_color();
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_ + SECTION_PADDING, top_);
	glVertex2d(right_ - SECTION_PADDING, top_);
	glVertex2d(right_ - SECTION_PADDING, bottom_);
	glVertex2d(left_ + SECTION_PADDING, bottom_);
	glVertex2d(left_ + SECTION_PADDING, top_);
	glEnd();
	Font(GLUT_BITMAP_HELVETICA_10, (char *) get_name().c_str(), left_ + 0.01, bottom_ + 0.02);
	return Rect(left_, top_, right_, bottom_);
}

bool Section::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
	if (!contains(mouse)) {
		color_factor_ = color_factor_init_;
		return false;
	}
	color_factor_ = color_factor_selected_;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Arrange window received click" << std::endl;
			break;
		case DOUBLE_CLICK:
			std::cout << "Arrange window received double click" << std::endl;
			break;
		case DRAG:
			std::cout << "Dragging in arrange window" << std::endl;
			break;
		case HOVER:
			std::cout << "Hovering in arrange window" << std::endl;
			break;
		case RELEASE:
			std::cout << "Arrange window received mouse release" << std::endl;
			break;
		default:
			break;
	}
	return true;
}

void Section::Font(void *font, char *text, double x, double y) {
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
