/*
 * menu.cpp
 *
 *  Created on: Jan 30, 2018
 *      Author: aarondawson
 */

#include "menu.h"

namespace add9daw2 {

Rect Menu::Draw() {
	// Draw the options
	for (int i=0; i<options_.size(); i++) {
		options_[i]->Draw();
	}
	// Draw the menu border
	glColor3d(color_.r, color_.g, color_.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_, top_);
	glVertex2d(right_, top_);
	glVertex2d(right_, bottom_);
	glVertex2d(left_, bottom_);
	glVertex2d(left_, top_);
	glEnd();
	// Draw the close icon
	glBegin(GL_LINE_STRIP);
	glVertex2d(icon_->left + ICON_PADDING, icon_->top - ICON_PADDING);
	glVertex2d(icon_->left + ICON_PADDING, icon_->bottom + ICON_PADDING);
	glVertex2d(icon_->right - ICON_PADDING, icon_->bottom + ICON_PADDING);
	glVertex2d(icon_->left + ICON_PADDING, icon_->top - ICON_PADDING);
	glVertex2d(icon_->right - ICON_PADDING, icon_->top - ICON_PADDING);
	glVertex2d(icon_->left + ICON_PADDING, icon_->bottom + ICON_PADDING);
	glVertex2d(icon_->right - ICON_PADDING, icon_->bottom + ICON_PADDING);
	glVertex2d(icon_->right - ICON_PADDING, icon_->top - ICON_PADDING);
	glEnd();
	return Rect(left_, top_, right_, bottom_);
}

bool Menu::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
	// If click in the icon area
	if (mouseEventType == CLICK && mouse->x > icon_->left && mouse->x < icon_->right
			&& mouse->y < icon_->top && mouse->y > icon_->bottom) {
		set_opened(false);
		return true;
	}
	// Menu options
	for (int i=0; i<options_.size(); i++) {
		options_[i]->ReceiveMouseEvent(mouse, mouseEventType);
	}
	if (!contains(mouse)) {
		return false;
	}
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

void Menu::add_option(std::string label) {
	// If there are no options put the option at the top
	if (options_.size() == 0) {
		options_.push_back(new MenuOption(label, left_, top_, right_ - ICON_WIDTH, top_ - OPTION_HEIGHT, this));
	}
	// Otherwise, place new audio track below existing tracks
	else {
		MenuOption* last_option = options_[options_.size() - 1];
		options_.push_back(new MenuOption(label, left_, last_option->get_bottom(), right_ - ICON_WIDTH, last_option->get_bottom() - OPTION_HEIGHT, this));
	}
}

} /* namespace add9daw2 */
