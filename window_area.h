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

// Base class for all sub-windows in the DAW
// Handles most abstract window actions and properties
class WindowArea {
public:
	WindowArea(double left, double top, double right, double bottom, MainWindow* main_window);
	virtual ~WindowArea() {}

	inline bool is_hidden() { return hidden_; }

	// Used to determine if a coordinate is within the bounds of this window area.
	// Helpful for handling mouse events with specific x and y coordinates.
	inline virtual bool is_coordinate_in_window(double x, double y) { return x > get_left() && x < get_right() && y < get_top() && y > get_bottom(); }

	// Handles click events in this window.
	virtual bool OnClick(double x, double y);

	// Handles double click events in this window.
	virtual bool OnDoubleClick(double x, double y);

	// Handles drag events in this window.
	virtual bool OnDrag(double x, double y);

	// Handles mouse release events in this window.
	virtual bool OnUpClick(double x, double y);

	// Returns the left bound of this window.
	inline double get_left() { return rect_->get_left(); }

	// Returns the top bound of this window.
	inline double get_top() { return rect_->get_top(); }

	// Returns the right bound of this window.
	inline double get_right() { return rect_->get_right(); }

	// Returns the bottom bound of this window.
	inline double get_bottom() { return rect_->get_bottom(); }

	// Returns the highest y-coordinate of this window and all its sub-windows.
	virtual double get_max_y();

	// Returns the lowest y-coordinate of this window and all its sub-windows.
	virtual double get_min_y();

	// Returns the amount that child windows should be shifted due to the scrollbar position.
	double get_scroll_amount();

	// Returns the height of this window.
	inline double get_height() { return rect_->get_height(); }

	// Returns the width of this window.
	inline double get_width() { return rect_->get_width(); }

	// Returns the number of immediate child windows this window contains.
	inline int get_num_child_windows() { return child_windows_.size(); }

	// Returns a reference to the main window.
	inline MainWindow* get_main_window() { return main_window_; }

	// Returns a reference to a rect containing the current bounds of this window.
	inline Rect *get_rect() { return rect_; }

	// Adds a sub-window to this window.
	inline void AddChildWindow(WindowArea* window_area) {
		child_windows_.push_back(window_area);
		window_area->set_parent_window(this);
	}

	// Handles the drawing of this window and its sub-windows, as well as the scrollbar.
	virtual void Draw(double x_offset, double y_offset);

	// Draws text on the screen.
	void Font(void *font, char *text, double x, double y);

	// Hides the scrollbar.
	void HideScrollBar();

	// Hides and shows this window.
	inline void set_hidden(bool hidden) { this->hidden_ = hidden; }

	// Sets a new left bound for this window.
	inline void set_left(double left) { rect_->set_left(left); }

	// Sets a new top bound for this window.
	inline void set_top(double top) { rect_->set_top(top); }

	// Sets a new right bound for this window.
	inline void set_right(double right) { rect_->set_right(right); }

	// Sets a new bottom bound for this window.
	inline void set_bottom(double bottom) { rect_->set_bottom(bottom); }

	// Sets a name for this window.
	inline void set_name(std::string name) { this->name_ = name; }

	// Sets a reference to the parent window of this window.
	inline void set_parent_window(WindowArea* window_area) {this->parent_window_ = window_area; }

	// Sets new bounds for this window.
	inline void set_rect(double left, double top, double right, double bottom) { rect_->set(left, top, right, bottom); }

	// Gets a particular child window of this window, specified by a chosen index.
	inline WindowArea* get_child_window(int child_window_index) { return child_windows_[child_window_index]; }

	// Returns a reference to the parent window of this window.
	inline WindowArea* get_parent_window() { return parent_window_; }
protected:
	// Converts coordinates to those expected by OpenGL
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
