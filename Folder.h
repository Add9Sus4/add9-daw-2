/*
 * Folder.h
 *
 *  Created on: Jan 18, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_FOLDER_H_
#define ADD9DAW2_SRC_FOLDER_H_

#define FILE_NAME_HEIGHT	0.02

#include <dirent.h>
#include <GLUT/glut.h>
#include "AudioFile.h"
#include "WindowArea.h"

using namespace std;

class Folder : public WindowArea {
public:
	Folder(string path, double l, double t, double r, double b, MainWindow* mainWindow);
	void draw(double x_offset, double y_offset);
	inline void toggleOpen() {
		open = !open;
		for (int i=0; i<childWindows.size(); i++) {
			childWindows[i]->setHidden(!open);
		}
	}
	bool onClick(double x, double y);
	double getMaxY();
	double getMinY();
	virtual ~Folder() {}
private:
	string path;
	bool open;
	double lowestFileY;
};

#endif /* ADD9DAW2_SRC_FOLDER_H_ */
