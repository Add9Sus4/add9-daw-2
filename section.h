/*
 * section.h
 *
 *  Created on: Jan 29, 2018
 *      Author: aarondawson
 */

#ifndef SECTION_H_
#define SECTION_H_

#define SECTION_PADDING	0.001

#include "glut/GLUT.h"
#include <iostream>
#include <string>

#include "color.h"
#include "menu.h"
#include "window.h"

namespace add9daw2 {

typedef enum SectionType {
	INTRO,
	VERSE,
	BUILD,
	DROP,
	BREAK,
	BRIDGE,
	OUTRO
} SectionType;

class Section : public Window {
public:
	Section(SectionType section_type, int start_measure, int end_measure, Window* parent) :
		Window(parent->get_left(), parent->get_top(), parent->get_right(), parent->get_bottom(), parent),
		section_type_(section_type),
		start_measure_(start_measure),
		end_measure_(end_measure) {
		set_default_values(section_type);
		parent_ = parent;
		left_ = parent->get_left();
		top_ = parent->get_top();
		right_ = parent->get_right();
		bottom_ = parent->get_bottom();
		color_factor_ = color_factor_init_;
	}
//	inline bool contains(Mouse* mouse) override {
//		return mouse->x >= left_ && mouse->x <= right_ && mouse->y - translate_amount_ <= top_ && mouse->y - translate_amount_ >= bottom_;
//	}
	inline SectionType get_section_type() { return section_type_; }
	inline void set_section_type(SectionType section_type) { section_type_ = section_type; }
	inline int get_num_measures() { return end_measure_ - start_measure_; }
	inline int get_start_measure() { return start_measure_; }
	inline int get_end_measure() { return end_measure_; }
	inline void set_start_measure(int start_measure) { start_measure_ = start_measure; }
	inline void set_end_measure(int end_measure) { end_measure_ = end_measure; }
	void set_default_values(SectionType section_type);
	inline void set_color() { glColor3d(color_.r*color_factor_, color_.g*color_factor_, color_.b*color_factor_); }
	inline std::string get_name() { return name_; }
	virtual ~Section();
	virtual Rect Draw();
	virtual bool ReceiveMouseEvent(Mouse* mouse, MouseEventType mouseEventType);
	inline void SetBounds(double left, double top, double right, double bottom) {
		left_ = left;
		top_ = top;
		right_ = right;
		bottom_ = bottom;
	}
	inline void set_not_selected() { color_factor_ = color_factor_init_; }
	inline bool has_menu_open() { return menu_ != 0; }
private:
	SectionType section_type_;
	int start_measure_, end_measure_;
	std::string name_;
	double color_factor_init_ = 0.5;
	double color_factor_selected_ = 1.0;
	double color_factor_;
	Menu* menu_ = 0;
};

} /* namespace add9daw2 */

#endif /* SECTION_H_ */
