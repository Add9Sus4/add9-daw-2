/*
 * snare_pattern.cpp
 *
 *  Created on: Feb 6, 2018
 *      Author: aarondawson
 */

#include "snare_pattern.h"

namespace add9daw2 {

SnarePattern::~SnarePattern() {
	// TODO Auto-generated destructor stub
}

std::vector<bool> SnarePattern::get_data(int length_in_samples, int samples_per_measure) {
	std::vector<bool> data;
	int quarter_note_length = samples_per_measure / 4;
	int eighth_note_length = samples_per_measure / 8;
	int sixteenth_note_length = samples_per_measure / 16;
	switch (snare_pattern_type_) {
		case EIGHT_BAR_BUILD:
			for (int i=0; i<length_in_samples; i++) {
				// First four measures, quarter notes
				if (i < samples_per_measure * 4) {
					data.push_back(i % quarter_note_length == 0);
				}
				// Next two measures, eighth notes
				else if (i < samples_per_measure * 6) {
					data.push_back(i % eighth_note_length == 0);
				}
				// Next measure and a half, sixteenth notes {
				else if (i < samples_per_measure * 7.5) {
					data.push_back(i % sixteenth_note_length == 0);
				}
				else {
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
