/*
 * files_window.h
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#ifndef FILES_WINDOW_H_
#define FILES_WINDOW_H_

#include "glut/GLUT.h"

#include <iostream>
#include <vector>

#include "folder.h"
#include "resources.h"
#include "vertical_scrollbar.h"
#include "window.h"

namespace add9daw2 {

class FilesWindow : public Window {
public:
	FilesWindow(double left, double top, double right, double bottom, Window* parent);
	virtual ~FilesWindow();
	Rect Draw() override;
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	inline double get_overflow_y() override { return overflow_y_; }
	void ResetScrollbar() override;
	void ResetAll() override;
private:
	std::vector<Folder*> folders_;
	VerticalScrollbar* vertical_scrollbar_;
	double overflow_y_; // Lowest y coordinate of any object in this window
};

} /* namespace add9daw2 */

#endif /* FILES_WINDOW_H_ */
