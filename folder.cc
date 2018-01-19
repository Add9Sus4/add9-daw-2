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
			AudioFile* file = new AudioFile(file_names[i], get_left() + PADDING, get_top() - (i + 1)*FILE_NAME_HEIGHT - PADDING,
					get_right() - PADDING, get_top() - (i + 2)*FILE_NAME_HEIGHT + PADDING, main_window);
			if (file->get_file_name().compare("Error") != 0) {
				file->set_hidden(true);
				AddChildWindow(file);
			} else {
				total_errors++;
			}
		}
		lowest_file_y_ = get_top() - (file_names.size() + 1 - total_errors)*FILE_NAME_HEIGHT + PADDING;
		std::cout << "lowest file Y: " << lowest_file_y_ << std::endl;
		std::cout << "bottom of files area: " << main_window->get_files_area()->get_bottom() << std::endl;
}

void Folder::Draw(double x_offset, double y_offset) {
	// Draw the folder name
	std::string abbreviated_path = path_.substr(path_.find("SAMPLES") + 8);
	glColor3d(0.6, 0.74, 0.80);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) abbreviated_path.c_str(), NormalizeCoord(get_left() + 0.01), NormalizeCoord(get_top() - 0.01));
	if (open_) {
		// Draw the open triangle icon
		glColor3d(0.3, 0.37, 0.40);
		glBegin(GL_LINE_STRIP);
		glVertex2d(NormalizeCoord(get_left()), NormalizeCoord(get_top()));
		glVertex2d(NormalizeCoord(get_left() + 0.005), NormalizeCoord(get_top()));
		glVertex2d(NormalizeCoord(get_left() + 0.0025), NormalizeCoord(get_top() - 0.01));
		glVertex2d(NormalizeCoord(get_left()), NormalizeCoord(get_top()));
		glEnd();
		for (int i=0; i<get_num_child_windows(); i++) {
			get_child_window(i)->Draw(x_offset, y_offset);
		}
	} else {
		// Draw the closed triangle icon
		glColor3d(0.3, 0.37, 0.40);
		glBegin(GL_LINE_STRIP);
		glVertex2d(NormalizeCoord(get_left()), NormalizeCoord(get_top()));
		glVertex2d(NormalizeCoord(get_left()), NormalizeCoord(get_top() - 0.01));
		glVertex2d(NormalizeCoord(get_left() + 0.005), NormalizeCoord(get_top() - 0.005));
		glVertex2d(NormalizeCoord(get_left()), NormalizeCoord(get_top()));
		glEnd();
	}
}

bool Folder::OnClick(double x, double y) {
	double scroll_amount = get_parent_window()->get_scroll_amount();
	std::cout << "Folder clicked" << std::endl;
	// If the click was inside the triangle
	if (x > get_left() && x < get_left() + 0.005
			&& y < get_top() && y > get_top() - 0.01) {
		ToggleOpen();
		return true;
	} else {
		for (int i=0; i<get_num_child_windows(); i++) {
			get_child_window(i)->OnClick(x, y - scroll_amount);
		}
	}
	return false;

}

double Folder::get_max_y() {
	if (open_) {
		double max_y = get_top();
		for (int i=0; i<get_num_child_windows(); i++) {
			if (true) {
				if (get_child_window(i)->get_top() > max_y) {
					max_y = get_child_window(i)->get_top();
				}
			}
		}
		return max_y;
	} else {
		return get_top();
	}
}
double Folder::get_min_y() {
	if (open_) {
		double min_y = get_bottom();
		for (int i=0; i<get_num_child_windows(); i++) {
			if (true) {
				if (get_child_window(i)->get_bottom() < min_y) {
					min_y = get_child_window(i)->get_bottom();
				}
			}
		}
		return min_y;
	} else {
		return get_bottom();
	}
}

} // namespace add9daw2
