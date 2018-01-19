/*
 * WindowArea.h
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_WINDOWAREA_H_
#define ADD9DAW2_SRC_WINDOWAREA_H_

#define PADDING						0.001
#define VERTICAL_SCROLL_BAR_WIDTH	0.01

#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

#include "Rect.h"

namespace add9daw2 {

class MainWindow;
class VerticalScrollBar;

class WindowArea {
public:
	WindowArea(double left, double top, double right, double bottom, MainWindow* main_window);
	virtual ~WindowArea() {}

	inline bool is_hidden() { return hidden_; }
	inline virtual bool is_in_window(double x, double y) { return x > get_left() && x < get_right() && y < get_top() && y > get_bottom(); }
	virtual bool OnClick(double x, double y);
	virtual bool OnDoubleClick(double x, double y);
	virtual bool OnDrag(double x, double y);
	virtual bool OnUpClick(double x, double y);

	inline double get_left() { return rect_->get_left(); }
	inline double get_top() { return rect_->get_top(); }
	inline double get_right() { return rect_->get_right(); }
	inline double get_bottom() { return rect_->get_bottom(); }
	virtual double get_max_y();
	virtual double get_min_y();
	double get_scroll_amount();
	inline double get_height() { return rect_->get_height(); }
	inline double get_width() { return rect_->get_width(); }

	inline int get_num_child_windows() { return child_windows_.size(); }

	inline MainWindow* get_main_window() { return main_window_; }

	inline Rect *get_rect() { return rect_; }

	inline void AddChildWindow(WindowArea* window_area) {
		child_windows_.push_back(window_area);
		window_area->set_parent_window(this);
	}
	virtual void Draw(double x_offset, double y_offset);
	void Font(void *font, char *text, double x, double y);
	void HideScrollBar();
	inline void set_hidden(bool hidden) { this->hidden_ = hidden; }
	inline void set_left(double left) { rect_->set_left(left); }
	inline void set_top(double top) { rect_->set_top(top); }
	inline void set_right(double right) { rect_->set_right(right); }
	inline void set_bottom(double bottom) { rect_->set_bottom(bottom); }
	inline void set_name(std::string name) { this->name_ = name; }
	inline void set_parent_window(WindowArea* window_area) {this->parent_window_ = window_area; }
	inline void set_rect(double left, double top, double right, double bottom) { rect_->set(left, top, right, bottom); }

	inline WindowArea* get_child_window(int child_window_index) { return child_windows_[child_window_index]; }
	inline WindowArea* get_parent_window() { return parent_window_; }
protected:
	inline double NormalizeCoord(double coord) { return coord * 2.0 - 1.0; }
private:
	bool hidden_;

	MainWindow* main_window_;

	Rect* rect_;

	std::string name_;

	std::vector<WindowArea*> child_windows_;

	VerticalScrollBar* vertical_scrollbar_;

	WindowArea* parent_window_;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_WINDOWAREA_H_ */
