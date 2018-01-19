/*
 * Folder.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#include "folder.h"

#include "main_window.h"
#include "window_area.h"

namespace add9daw2 {

Folder::Folder(std::string path, double l, double t, double r, double b, MainWindow* main_window) : WindowArea(l, t, r, b, main_window), open_(false), path_(path) {
	std::vector<std::string> file_names;
	//	std::cout << "Loading files from " << dirName << "..." << std::endl;
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
				file_names.push_back(entry->d_name);
			}
		}
		int total_errors = 0;
		for (int i=0; i<file_names.size(); i++) {
			AudioFile* file = new AudioFile(file_names[i], getL() + PADDING, getT() - (i + 1)*FILE_NAME_HEIGHT - PADDING,
					getR() - PADDING, getT() - (i + 2)*FILE_NAME_HEIGHT + PADDING, main_window);
			if (file->getFileName().compare("Error") != 0) {
				file->setHidden(true);
				addChildWindow(file);
			} else {
				total_errors++;
			}
		}
		lowest_file_y_ = getT() - (file_names.size() + 1 - total_errors)*FILE_NAME_HEIGHT + PADDING;
		std::cout << "lowest file Y: " << lowest_file_y_ << std::endl;
		std::cout << "bottom of files area: " << main_window->getFilesArea()->getB() << std::endl;
}

void Folder::draw(double x_offset, double y_offset) {
	// Draw the folder name
	std::string abbreviated_path = path_.substr(path_.find("SAMPLES") + 8);
	glColor3d(0.6, 0.74, 0.80);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) abbreviated_path.c_str(), normalizeCoord(getL() + 0.01), normalizeCoord(getT() - 0.01));
	if (open_) {
		// Draw the open triangle icon
		glColor3d(0.3, 0.37, 0.40);
		glBegin(GL_LINE_STRIP);
		glVertex2d(normalizeCoord(getL()), normalizeCoord(getT()));
		glVertex2d(normalizeCoord(getL() + 0.005), normalizeCoord(getT()));
		glVertex2d(normalizeCoord(getL() + 0.0025), normalizeCoord(getT() - 0.01));
		glVertex2d(normalizeCoord(getL()), normalizeCoord(getT()));
		glEnd();
		for (int i=0; i<numChildWindows(); i++) {
			getChildWindows(i)->draw(x_offset, y_offset);
		}
	} else {
		// Draw the closed triangle icon
		glColor3d(0.3, 0.37, 0.40);
		glBegin(GL_LINE_STRIP);
		glVertex2d(normalizeCoord(getL()), normalizeCoord(getT()));
		glVertex2d(normalizeCoord(getL()), normalizeCoord(getT() - 0.01));
		glVertex2d(normalizeCoord(getL() + 0.005), normalizeCoord(getT() - 0.005));
		glVertex2d(normalizeCoord(getL()), normalizeCoord(getT()));
		glEnd();
	}
}

bool Folder::onClick(double x, double y) {
	double scroll_amount = getParentWindow()->getScrollAmount();
	std::cout << "Folder clicked" << std::endl;
	// If the click was inside the triangle
	if (x > getL() && x < getL() + 0.005
			&& y < getT() && y > getT() - 0.01) {
		toggleOpen();
		return true;
	} else {
		for (int i=0; i<numChildWindows(); i++) {
			getChildWindows(i)->onClick(x, y - scroll_amount);
		}
	}
	return false;

}

double Folder::getMaxY() {
	if (open_) {
		double max_y = getT();
		for (int i=0; i<numChildWindows(); i++) {
			if (true) {
				if (getChildWindows(i)->getT() > max_y) {
					max_y = getChildWindows(i)->getT();
				}
			}
		}
		return max_y;
	} else {
		return getT();
	}
}
double Folder::getMinY() {
	if (open_) {
		double min_y = getB();
		for (int i=0; i<numChildWindows(); i++) {
			if (true) {
				if (getChildWindows(i)->getB() < min_y) {
					min_y = getChildWindows(i)->getB();
				}
			}
		}
		return min_y;
	} else {
		return getB();
	}
}

} // namespace add9daw2