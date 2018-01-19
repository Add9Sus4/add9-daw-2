/*
 * Folder.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#include "Folder.h"
#include "MainWindow.h"
#include "WindowArea.h"

namespace add9daw2 {

Folder::Folder(std::string path, double l, double t, double r, double b, MainWindow* mainWindow) : WindowArea(l, t, r, b, mainWindow), path(path), open(false) {
	std::vector<std::string> fileNames;
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
				fileNames.push_back(entry->d_name);
			}
		}
		int total_errors = 0;
		for (int i=0; i<fileNames.size(); i++) {
			AudioFile* file = new AudioFile(fileNames[i], getL() + PADDING, getT() - (i + 1)*FILE_NAME_HEIGHT - PADDING,
					getR() - PADDING, getT() - (i + 2)*FILE_NAME_HEIGHT + PADDING, mainWindow);
			if (file->getFileName().compare("Error") != 0) {
				file->setHidden(true);
				addChildWindow(file);
			} else {
				total_errors++;
			}
		}
		lowestFileY = getT() - (fileNames.size() + 1 - total_errors)*FILE_NAME_HEIGHT + PADDING;
		std::cout << "lowest file Y: " << lowestFileY << std::endl;
		std::cout << "bottom of files area: " << mainWindow->getFilesArea()->getB() << std::endl;
}

void Folder::draw(double x_offset, double y_offset) {
	// Draw the folder name
	std::string abbrevPath = path.substr(path.find("SAMPLES") + 8);
	glColor3d(0.6, 0.74, 0.80);
	Font(GLUT_BITMAP_HELVETICA_10, (char*) abbrevPath.c_str(), normalizeCoord(getL() + 0.01), normalizeCoord(getT() - 0.01));
	if (open) {
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
	double scrollAmount = getParentWindow()->getScrollAmount();
	std::cout << "Folder clicked" << std::endl;
	// If the click was inside the triangle
	if (x > getL() && x < getL() + 0.005
			&& y < getT() && y > getT() - 0.01) {
		toggleOpen();
		return true;
	} else {
		for (int i=0; i<numChildWindows(); i++) {
			getChildWindows(i)->onClick(x, y - scrollAmount);
		}
	}
	return false;

}

double Folder::getMaxY() {
	if (open) {
		double maxY = getT();
		for (int i=0; i<numChildWindows(); i++) {
			if (true) {
				if (getChildWindows(i)->getT() > maxY) {
					maxY = getChildWindows(i)->getT();
				}
			}
		}
		return maxY;
	} else {
		return getT();
	}
}
double Folder::getMinY() {
	if (open) {
		double minY = getB();
		for (int i=0; i<numChildWindows(); i++) {
			if (true) {
				if (getChildWindows(i)->getB() < minY) {
					minY = getChildWindows(i)->getB();
				}
			}
		}
		return minY;
	} else {
		return getB();
	}
}

} // namespace add9daw2
