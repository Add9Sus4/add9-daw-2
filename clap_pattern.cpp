/*
 * clap_pattern.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: aarondawson
 */

#include "clap_pattern.h"

namespace add9daw2 {

ClapPattern::~ClapPattern() {
	// TODO Auto-generated destructor stub
}

std::vector<bool> ClapPattern::get_data(int length_in_samples, int samples_per_measure) {
	std::vector<bool> data;
	int quarter_note_length = samples_per_measure / 4;
	int half_note_length = samples_per_measure / 2;
	switch (clap_pattern_type_) {
		case TWO_AND_FOUR:
			for (int i=0; i<length_in_samples; i++) {
				data.push_back(i % quarter_note_length == 0 && i % half_note_length != 0);
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
