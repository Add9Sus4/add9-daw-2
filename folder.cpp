/*
 * folder.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#include "folder.h"

namespace add9daw2 {

Folder::Folder(double left, double top, double right, double bottom, Window* parent) :
		Window(left, top, right, bottom, parent),
		open_(false), parent_(parent) {

}

Folder::~Folder() {
	// TODO Auto-generated destructor stub
}

void Folder::LoadFileNames(std::string path) {
	path_ = path;
		std::cout << "Loading files from " << path << "..." << std::endl;
	DIR *directory;
	struct dirent *entry;

	// Return if the directory cannot be opened
	if ((directory = opendir(path.c_str())) == NULL) {
		std::cout << "Cannot open directory " << directory << std::endl;
		return;
	}

	chdir(path.c_str());
	while ((entry = readdir(directory)) != NULL) {
		if((strcasestr(entry->d_name, ".wav") != NULL ||
					strcasestr(entry->d_name, ".aiff") != NULL ||
					strcasestr(entry->d_name, ".aif") != NULL) &&
					strcasestr(entry->d_name, ".asd") == NULL) {
			file_names_.push_back(entry->d_name);
		}
	}
	for (int i=0; i<file_names_.size(); i++) {
		files_.push_back(new AudioFile(file_names_[i], path, left_, top_, right_, bottom_, this));
	}
}

Rect Folder::Draw() {
	glColor3d(color_.r, color_.g, color_.b);
	// Draw the rectangle around the folder
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_ + FOLDER_PADDING, top_ - FOLDER_PADDING);
	glVertex2d(right_ - FOLDER_PADDING, top_ - FOLDER_PADDING);
	glVertex2d(right_ - FOLDER_PADDING, bottom_ + FOLDER_PADDING);
	glVertex2d(left_ + FOLDER_PADDING, bottom_ + FOLDER_PADDING);
	glVertex2d(left_ + FOLDER_PADDING, top_ - FOLDER_PADDING);
	glEnd();
	// Draw the folder name
	std::string abbreviated_path = path_.substr(path_.find("SAMPLES") + 8);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) abbreviated_path.c_str(), left_ + 0.035, top_ - 0.035);
	// Display files, if open
	double bottom = bottom_;
	if (open_) {
		for (int i=0; i<files_.size(); i++) {
			Font(GLUT_BITMAP_HELVETICA_10, (char*) files_[i]->get_name().c_str(), left_ + 0.035, top_ - 0.08 - 0.035*i);
			bottom = top_ - 0.08 - 0.035*i;
		}
	}
	// Return the bottom of the folder if closed; otherwise, return the bottom of the last file in the list.
	return Rect(left_, top_, right_, bottom);
}

Rect Folder::DrawBelow(Rect rect, double translate_amount) {
	// If the folder is open, it does not move with the scrollbar
	double folder_translate_amount = translate_amount;
	if (open_) {
		folder_translate_amount = 0;
	}
	top_ = rect.bottom;
	bottom_ = rect.bottom - FOLDER_HEIGHT;
	// Hide if the folder is below the bottom of the arrange window
	if (bottom_ + folder_translate_amount < get_parent()->get_bottom()) {
		set_hidden(true);
	} else {
		set_hidden(false);
	}
	// If the folder is hidden, return the rect but don't draw the folder.
	if (hidden_) {
		return Rect(left_, top_, right_, bottom_);
	}
	glColor3d(color_.r, color_.g, color_.b);
	// Draw the rectangle around the folder
	glBegin(GL_LINE_STRIP);
	glVertex2d(left_ + FOLDER_PADDING, top_ - FOLDER_PADDING + folder_translate_amount);
	glVertex2d(right_ - FOLDER_PADDING, top_ - FOLDER_PADDING + folder_translate_amount);
	glVertex2d(right_ - FOLDER_PADDING, bottom_ + FOLDER_PADDING + folder_translate_amount);
	glVertex2d(left_ + FOLDER_PADDING, bottom_ + FOLDER_PADDING + folder_translate_amount);
	glVertex2d(left_ + FOLDER_PADDING, top_ - FOLDER_PADDING + folder_translate_amount);
	glEnd();
	// Draw the folder name
	std::string abbreviated_path = path_.substr(path_.find("SAMPLES") + 8);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) abbreviated_path.c_str(), left_ + 0.035, top_ - 0.035 + folder_translate_amount);
	// Display files, if open
	double bottom = bottom_;
	if (open_) {
		Rect file_position = {left_, top_, right_, bottom_};
		for (int i=0; i<files_.size(); i++) {
			 file_position = files_[i]->DrawBelow(file_position, translate_amount);
			 bottom = file_position.bottom;
		}
	}
	// Return the bottom of the folder if closed; otherwise, return the bottom of the last file in the list.
	return Rect(left_, top_, right_, bottom);
}

bool Folder::ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) {
	// Mouse cannot interact with hidden folders
	if (hidden_) {
		return false;
	}
	// If the mouse is not in the area
	if (!contains(mouse)) {
		// If the folder is closed, return false
		if (!open_) {
			ResetColor();
			return false;
		}
		// If the folder is open, check if the mouse is in the area of one of the audio files
		else {
			bool targeted_audio = false;
			for (int i=0; i<files_.size(); i++) {
				bool targeted = files_[i]->ReceiveMouseEvent(mouse, mouseEventType);
				if (targeted) {
					targeted_audio = true;
				}
			}
			// If the mouse is in the area of one of the audio files, return true
			if (targeted_audio) {
				ResetColor();
				return true;
			} else {
				return false;
			}
		}
	}
	// If the folder is open
	color_ = color_selected_;
	switch (mouseEventType) {
		case CLICK:
			std::cout << "Folder " << path_ << " received click" << std::endl;
			// Closing this folder, reset everything
			if (open_) {
				get_parent()->ResetScrollbar();
				for (int i=0; i<files_.size(); i++) {
					files_[i]->set_translate_amount(0.0);
				}
				open_ = false;
			}
			// Opening this folder, close all other ones and reset them
			else {
				get_parent()->ResetAll();
				open_ = true;
			}
			break;
		case DOUBLE_CLICK:
			std::cout << "Folder " << path_ << " received double click" << std::endl;
			// Closing this folder, reset everything
			if (open_) {
				get_parent()->ResetScrollbar();
				for (int i=0; i<files_.size(); i++) {
					files_[i]->set_translate_amount(0.0);
				}
				open_ = false;
			}
			// Opening this folder, close all other ones and reset them
			else {
				get_parent()->ResetAll();
				open_ = true;
			}
			break;
		case DRAG:
			std::cout << "Dragging in folder " << path_ << std::endl;
			break;
		case HOVER:
			std::cout << "Hovering in folder " << path_ << std::endl;
			break;
		case RELEASE:
			std::cout << "Folder " << path_ << " received mouse release" << std::endl;
			mouse->ClearFile();
			break;
		default:
			break;
	}
	return true;
}

void Folder::Font(void *font, char *text, double x, double y) {
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
