/*
 * folder.h
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#ifndef FOLDER_H_
#define FOLDER_H_

#define FOLDER_PADDING	0.005
#define FOLDER_HEIGHT	0.05

#include <dirent.h>
#include "glut/GLUT.h"
#include <math.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "audio_file.h"
#include "window.h"

namespace add9daw2 {

class Folder : public Window {
public:
	Folder(double left, double top, double right, double bottom, Window* parent);
	virtual ~Folder();

	inline bool is_open() { return open_; }
	void LoadFileNames(std::string path);
	Rect Draw() override;
	// Draws this folder immediately below the given rect
	Rect DrawBelow(Rect rect, double translate_amount);
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	void Font(void *font, char *text, double x, double y);
	inline std::vector<AudioFile*> get_files() { return files_; }
private:
	bool open_;
	std::vector<std::string> file_names_;
	std::vector<AudioFile*> files_;
	std::string path_;
	Window* parent_;
};

} /* namespace add9daw2 */

#endif /* FOLDER_H_ */
