/*
 * menu.h
 *
 *  Created on: Jan 30, 2018
 *      Author: aarondawson
 */

#ifndef MENU_H_
#define MENU_H_

#define OPTION_HEIGHT	0.06
#define ICON_PADDING	0.005

#include <iostream>
#include <string>
#include <vector>

#include "menu_option.h"
#include "window.h"

namespace add9daw2 {

class Menu : public Window {
public:
	Menu(double left, double top, double right, double bottom, Window* parent) :
		Window(left, top, right, bottom, parent) {
		icon_ = new Rect(right - ICON_WIDTH, top, right, top - ICON_WIDTH);
	}
	virtual ~Menu() { options_.clear(); }
	Rect Draw() override;
	bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType) override;
	void add_option(std::string label);
	inline void set_opened(bool opened) { opened_ = opened; }
	inline bool is_opened() { return opened_; }
private:
	std::vector<MenuOption*> options_;
	Rect* icon_;
	bool opened_ = true;
};

} /* namespace add9daw2 */

#endif /* MENU_H_ */
