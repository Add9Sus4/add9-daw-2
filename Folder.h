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

class Folder : public WindowArea {
public:
	Folder(std::string path, double l, double t, double r, double b, MainWindow* main_window);
	virtual ~Folder() {}

	void Draw(double x_offset, double y_offset) override;
	inline void ToggleOpen() {
		open_ = !open_;
		for (int i=0; i<get_num_child_windows(); i++) {
			get_child_window(i)->set_hidden(!open_);
		}
	}

	bool OnClick(double x, double y) override;

	double get_max_y() override;
	double get_min_y() override;

private:
	bool open_;

	double lowest_file_y_;

	std::string path_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_FOLDER_H_ */
