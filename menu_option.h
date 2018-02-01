/*
 * menu_option.h
 *
 *  Created on: Jan 30, 2018
 *      Author: aarondawson
 */

#ifndef MENU_OPTION_H_
#define MENU_OPTION_H_

#define ICON_WIDTH		0.03
#define OPTION_PADDING	0.01

#include <glut/GLUT.h>
#include <iostream>
#include <string>

#include "color.h"
#include "window.h"

namespace add9daw2 {

class MenuOption : public Window {
public:
	MenuOption(std::string label, double left, double top, double right, double bottom, Window* parent) :
	Window(left, top, right, bottom, parent), label_(label) {}
	virtual ~MenuOption() {}
	Rect Draw() override;
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
private:
	std::string label_;
	Color option_color_init_ {0.1, 0.4, 0.3};
	Color option_color_selected_ {0.2, 0.8, 0.6};
};

} /* namespace add9daw2 */

#endif /* MENU_OPTION_H_ */
