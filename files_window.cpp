/*
 * files_window.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#include "files_window.h"

namespace add9daw2 {

FilesWindow::FilesWindow(double left, double top, double right, double bottom, Window* parent) :
		Window(left, top, right, bottom, parent), overflow_y_(bottom) {
	Folder* kick_folder = new Folder(left, top, right - VERTICAL_SCROLLBAR_WIDTH, top - FOLDER_HEIGHT, this);
	Folder* snare_folder = new Folder(left, top - FOLDER_HEIGHT, right - VERTICAL_SCROLLBAR_WIDTH, top - FOLDER_HEIGHT*2, this);
	Folder* hat_folder = new Folder(left, top - FOLDER_HEIGHT*2, right - VERTICAL_SCROLLBAR_WIDTH, top - FOLDER_HEIGHT*3, this);
	Folder* tom_folder = new Folder(left, top - FOLDER_HEIGHT*3, right - VERTICAL_SCROLLBAR_WIDTH, top - FOLDER_HEIGHT*4, this);
	kick_folder->LoadFileNames(KICK_DIR_PATH);
	snare_folder->LoadFileNames(SNARE_DIR_PATH);
	hat_folder->LoadFileNames(HAT_DIR_PATH);
	tom_folder->LoadFileNames(TOM_DIR_PATH);
	folders_.push_back(kick_folder);
	folders_.push_back(snare_folder);
	folders_.push_back(hat_folder);
	folders_.push_back(tom_folder);

	vertical_scrollbar_ = new VerticalScrollbar(right - VERTICAL_SCROLLBAR_WIDTH, top, right, bottom, this);
}

FilesWindow::~FilesWindow() {
	// TODO Auto-generated destructor stub
}

Rect FilesWindow::Draw() {
	// Draw the files window area
	glColor3d(color.r, color.g, color.b);
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_, top_);
	glVertex2d(right_, top_);
	glVertex2d(right_, bottom_);
	glVertex2d(left_, bottom_);
	glVertex2d(left_, top_);
	glEnd();
	// Translate based on scroll amount
	double scroll_ratio = overflow_y_ / (height() - DRAG_AREA_HEIGHT);
	double translate_amount = -vertical_scrollbar_->get_scroll_amount() * -scroll_ratio;
	// Draw all folders
	Rect folder_position = {left_, top_, right_, top_};
	for (int i=0; i<folders_.size(); i++) {
		if (folders_[i]->is_open()) {
			folder_position = folders_[i]->DrawBelow(folder_position, translate_amount);
		} else {
			folder_position = folders_[i]->DrawBelow(folder_position, 0.0);
		}
	}
	overflow_y_ = folder_position.bottom;
	if (overflow_y_ < bottom_) {
		vertical_scrollbar_->set_hidden(false);
	} else {
		vertical_scrollbar_->set_hidden(true);
	}
	// Draw vertical scrollbar
	vertical_scrollbar_->Draw();
	return Rect(left_, top_, right_, bottom_);
}

bool FilesWindow::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
	vertical_scrollbar_->ReceiveMouseEvent(mouse, mouseEventType);
	if (!contains(mouse)) {
		for (int i=0; i<folders_.size(); i++) {
			folders_[i]->ReceiveMouseEvent(mouse, mouseEventType);
		}
		ResetColor();
		return false;
	}
	// Check if a folder is targeted
	bool folder_targeted = false;
	for (int i=0; i<folders_.size(); i++) {
		bool targeted = folders_[i]->ReceiveMouseEvent(mouse, mouseEventType);
		if (targeted) {
			folder_targeted = true;
		}
	}
	if (folder_targeted) {
		ResetColor();
		return false;
	}
	// Check if the scrollbar is targeted
	if (vertical_scrollbar_->ReceiveMouseEvent(mouse, mouseEventType)) {
		ResetColor();
		return false;
	}
	color = selected;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Files window received click" << std::endl;
			break;
		case DOUBLE_CLICK:
			std::cout << "Files window received double click" << std::endl;
			break;
		case DRAG:
			std::cout << "Dragging in files window" << std::endl;
			break;
		case HOVER:
			std::cout << "Hovering in files window" << std::endl;
			break;
		case RELEASE:
			std::cout << "Files window received mouse release" << std::endl;
			mouse->ClearFile();
			break;
		default:
			break;
	}
	return true;
}

void FilesWindow::ResetScrollbar() {
	vertical_scrollbar_->reset();
}

void FilesWindow::ResetAll() {
	ResetScrollbar();
	for (int i=0; i<folders_.size(); i++) {
		std::vector<AudioFile*> files = folders_[i]->get_files();
		for (int j=0; j<files.size(); j++) {
			files[j]->set_translate_amount(0.0);
		}
	}
}

} /* namespace add9daw2 */


