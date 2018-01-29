/*
 * section.cpp
 *
 *  Created on: Jan 29, 2018
 *      Author: aarondawson
 */

#include "section.h"

namespace add9daw2 {

Section::~Section() {
	// TODO Auto-generated destructor stub
}

void Section::set_default_values(SectionType section_type) {
	switch (section_type) {
		case INTRO:
			name_ = "Intro";
			color_ = {0.7,0.5,0.4};
			break;
		case VERSE:
			name_ = "Verse";
			color_ = {0.7,0.4,0.5};
			break;
		case BUILD:
			name_ = "Build";
			color_ = {0.5,0.7,0.4};
			break;
		case DROP:
			name_ = "Drop";
			color_ = {0.5,0.4,0.7};
			break;
		case BREAK:
			name_ = "Break";
			color_ = {0.4,0.7,0.5};
			break;
		case BRIDGE:
			name_ = "Bridge";
			color_ = {0.4,0.5,0.7};
			break;
		case OUTRO:
			name_ = "Outro";
			color_ = {0.6,0.3,0.4};
			break;
		default:
			name_ = "N/A";
			color_ = {0.3,0.6,0.4};
			break;
	}
}

} /* namespace add9daw2 */
