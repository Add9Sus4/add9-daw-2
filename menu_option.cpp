/*
 * menu_option.cpp
 *
 *  Created on: Jan 30, 2018
 *      Author: aarondawson
 */

#include "arrange_window.h"
#include "kick_pattern.h"
#include "menu.h"
#include "menu_option.h"
#include "section.h"

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
	Menu* menu = (Menu*) get_parent();
	Section* section = (Section*) menu->get_parent();
	ArrangeWindow* arrange_window = (ArrangeWindow*) section->get_parent();
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Menu received click" << std::endl;
			if (label_ == "Add pattern (kick)") {
				arrange_window->AddPattern(new KickPattern(FOUR_ON_THE_FLOOR), KICK, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			} else if (label_ == "Add pattern (clap)") {
				arrange_window->AddPattern(0, CLAP, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			} else if (label_ == "Add pattern (snare)") {
				arrange_window->AddPattern(0, SNARE, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			} else if (label_ == "Add pattern (hat)") {
				arrange_window->AddPattern(0, HAT, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			} else if (label_ == "Add pattern (high impact)") {
				arrange_window->AddPattern(0, HIGH_IMPACT, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			} else if (label_ == "Add pattern (low impact)") {
				arrange_window->AddPattern(0, LOW_IMPACT, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			} else if (label_ == "Add pattern (punchy impact)") {
				arrange_window->AddPattern(0, PUNCHY_IMPACT, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			} else if (label_ == "Add pattern (sweep up)") {
				arrange_window->AddPattern(0, SWEEP_UP, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			} else if (label_ == "Add pattern (sweep down)") {
				arrange_window->AddPattern(0, SWEEP_DOWN, section->get_start_measure(), section->get_end_measure());
				menu->set_opened(false);
			}
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

} /* namespace add9daw2 */
