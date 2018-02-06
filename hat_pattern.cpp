/*
 * hat_pattern.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: aarondawson
 */

#include "hat_pattern.h"

namespace add9daw2 {

HatPattern::~HatPattern() {
	// TODO Auto-generated destructor stub
}

std::vector<bool> HatPattern::get_data(int length_in_samples, int samples_per_measure) {
	std::vector<bool> data;
	int quarter_note_length = samples_per_measure / 4;
	int eighth_note_length = samples_per_measure / 8;
	int sixteenth_note_length = samples_per_measure / 16;
	switch (hat_pattern_type_) {
		case OFFBEAT:
			for (int i=0; i<length_in_samples; i++) {
				data.push_back(i % eighth_note_length == 0 && i % quarter_note_length != 0);
			}
			break;
		case TRAP:
			for (int i=0; i<length_in_samples; i++) {
				if (i % sixteenth_note_length) {
					data.push_back(rand()%100 > 50);
				} else {
					data.push_back(false);
				}
			}
			break;
		default:
			for (int i=0; i<length_in_samples; i++) {
				data.push_back(false);
			}
			break;
	}
	return data;
}

} /* namespace add9daw2 */
