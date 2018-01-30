/*
 * menu.h
 *
 *  Created on: Jan 30, 2018
 *      Author: aarondawson
 */

#ifndef MENU_H_
#define MENU_H_

#define OPTION_HEIGHT	0.06

#include <iostream>
#include <string>
#include <vector>

#include "menu_option.h"
#include "window.h"

namespace add9daw2 {

class Menu : public Window {
public:
	Menu(double left, double top, double right, double bottom, Window* parent) :
		Window(left, top, right, bottom, parent) {}
	virtual ~Menu() { options_.clear(); }
	Rect Draw() override;
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	void add_option(std::string label);
private:
	std::vector<MenuOption*> options_;
};

} /* namespace add9daw2 */

#endif /* MENU_H_ */
