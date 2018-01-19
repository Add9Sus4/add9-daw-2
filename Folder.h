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

#include "audio_file.h"

namespace add9daw2 {

class WindowArea;

// Represents a folder in the filesystem.
// Used to access audio files that are in a particular folder.
class Folder : public WindowArea {
public:
	Folder(std::string path, double l, double t, double r, double b, MainWindow* main_window);
	virtual ~Folder() {}

	// Handles drawing of the folder, its icon, and the files it contains
	void Draw(double x_offset, double y_offset) override;

	// Opens the folder when it is closed, and vice versa.
	inline void ToggleOpen() {
		open_ = !open_;
		for (int i=0; i<get_num_child_windows(); i++) {
			get_child_window(i)->set_hidden(!open_);
		}
	}

	// Handles what happens when the folder is clicked.
	bool OnClick(double x, double y) override;

	// Returns the maximum y-coordinate of this folder, as well as all the files it contains.
	double get_max_y() override;

	// Returns the minimum y-coordinate of this folder, as well as all the files it contains.
	double get_min_y() override;

private:
	bool open_;

	double lowest_file_y_;

	std::string path_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_FOLDER_H_ */
