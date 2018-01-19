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

namespace add9daw2 {

class WindowArea;

class Folder : public WindowArea {
public:
	Folder(std::string path, double l, double t, double r, double b, MainWindow* mainWindow);
	virtual ~Folder() {}

	void draw(double x_offset, double y_offset) override;
	inline void toggleOpen() {
		open = !open;
		for (int i=0; i<numChildWindows(); i++) {
			getChildWindows(i)->setHidden(!open);
		}
	}

	bool onClick(double x, double y) override;

	double getMaxY() override;
	double getMinY() override;

private:
	bool open;

	double lowestFileY;

	std::string path;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_FOLDER_H_ */
