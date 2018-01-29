/*
 * section.h
 *
 *  Created on: Jan 29, 2018
 *      Author: aarondawson
 */

#ifndef SECTION_H_
#define SECTION_H_

#include "glut/GLUT.h"
#include <iostream>
#include <string>

#include "Color.h"

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

class Section {
public:
	Section(SectionType section_type, int start_measure, int end_measure) :
		section_type_(section_type),
		start_measure_(start_measure),
		end_measure_(end_measure) {
		set_default_values(section_type);
	}
	inline SectionType get_section_type() { return section_type_; }
	inline void set_section_type(SectionType section_type) { section_type_ = section_type; }
	inline int get_num_measures() { return end_measure_ - start_measure_; }
	inline int get_start_measure() { return start_measure_; }
	inline int get_end_measure() { return end_measure_; }
	inline void set_start_measure(int start_measure) { start_measure_ = start_measure; }
	inline void set_end_measure(int end_measure) { end_measure_ = end_measure; }
	void set_default_values(SectionType section_type);
	inline void set_color() { glColor3d(color_.r, color_.g, color_.b); }
	inline std::string get_name() { return name_; }
	virtual ~Section();
private:
	SectionType section_type_;
	int start_measure_, end_measure_;
	std::string name_;
	Color color_;
};

} /* namespace add9daw2 */

#endif /* SECTION_H_ */
