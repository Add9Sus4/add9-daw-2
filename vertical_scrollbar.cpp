/*
 * vertical_scrollbar.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#include "vertical_scrollbar.h"

namespace add9daw2 {

VerticalScrollbar::~VerticalScrollbar() {
	// TODO Auto-generated destructor stub
}

Rect VerticalScrollbar::Draw() {
	// If the scrollbar is hidden, don't draw it
	if (hidden_) {
		return Rect(left_, top_, right_, bottom_);
	}
	// Draw the rectangle around the scrollbar
//	glBegin(GL_LINE_STRIP);
//	glVertex2d(left_ + VERTICAL_SCROLLBAR_PADDING, top_ - VERTICAL_SCROLLBAR_PADDING);
//	glVertex2d(right_ - VERTICAL_SCROLLBAR_PADDING, top_ - VERTICAL_SCROLLBAR_PADDING);
//	glVertex2d(right_ - VERTICAL_SCROLLBAR_PADDING, bottom_ + VERTICAL_SCROLLBAR_PADDING);
//	glVertex2d(left_ + VERTICAL_SCROLLBAR_PADDING, bottom_ + VERTICAL_SCROLLBAR_PADDING);
//	glVertex2d(left_ + VERTICAL_SCROLLBAR_PADDING, top_ - VERTICAL_SCROLLBAR_PADDING);
//	glEnd();

	// Draw the drag area
	// Calculate drag area height
	glColor3d(color.r, color.g, color.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(drag_area_->left + VERTICAL_SCROLLBAR_PADDING, last_position_ - offset_y_ - VERTICAL_SCROLLBAR_PADDING);
	glVertex2d(drag_area_->right - VERTICAL_SCROLLBAR_PADDING, last_position_ - offset_y_ - VERTICAL_SCROLLBAR_PADDING);
	glVertex2d(drag_area_->right - VERTICAL_SCROLLBAR_PADDING, last_position_ - DRAG_AREA_HEIGHT - offset_y_ + VERTICAL_SCROLLBAR_PADDING);
	glVertex2d(drag_area_->left + VERTICAL_SCROLLBAR_PADDING, last_position_ - DRAG_AREA_HEIGHT - offset_y_ + VERTICAL_SCROLLBAR_PADDING);
	glVertex2d(drag_area_->left + VERTICAL_SCROLLBAR_PADDING, last_position_ - offset_y_ - VERTICAL_SCROLLBAR_PADDING);
	glEnd();
	return Rect(left_, top_, right_, bottom_);
}

bool VerticalScrollbar::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
	// Scrollbar cannot receive mouse events when it is hidden
	if (hidden_) {
		return false;
	}
	if (!inDragArea(mouse) && !dragging_) {
		ResetColor();
		return false;
	}
	color = selected;
	double potential_offset;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Scrollbar received click" << std::endl;
			break;
		case DOUBLE_CLICK:
			std::cout << "Scrollbar received double click" << std::endl;
			break;
		case DRAG:
			std::cout << "Dragging in Scrollbar " << std::endl;
			dragging_ = true;
			potential_offset = last_position_ - top_ - mouse->ypress + mouse->y;
			std::cout << "Potential offset: " << potential_offset << std::endl;
			if (potential_offset > 0.0) {
				offset_y_ = last_position_ - top_;
			} else if (potential_offset < -height() + DRAG_AREA_HEIGHT) {
				offset_y_ = height() - DRAG_AREA_HEIGHT - last_position_ + top_;
			} else {
				offset_y_ = mouse->ypress - mouse->y;
			}
			break;
		case HOVER:
			std::cout << "Hovering in Scrollbar " << std::endl;
			break;
		case RELEASE:
			std::cout << "Scrollbar received mouse release" << std::endl;
			drag_area_->top = last_position_ - offset_y_;
			drag_area_->bottom = last_position_ - DRAG_AREA_HEIGHT - offset_y_;
			last_position_ -= offset_y_;
			offset_y_ = 0.0;
			dragging_ = false;
			mouse->ClearFile();
			break;
		default:
			break;
	}
	return true;
}

} /* namespace add9daw2 */

