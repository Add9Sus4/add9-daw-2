/*
 * kick_pattern.cpp
 *
 *  Created on: Feb 1, 2018
 *      Author: aarondawson
 */

#include "kick_pattern.h"

namespace add9daw2 {

KickPattern::~KickPattern() {
	// TODO Auto-generated destructor stub
}

std::vector<bool> KickPattern::get_data(int length_in_samples, int samples_per_measure) {
	bool add_sample = false;
	std::vector<bool> data;
	std::vector<int> kick_locations;
	int quarter_note_length = samples_per_measure / 4;
	int counter = 0;
	switch (kick_pattern_type_) {
		case FOUR_ON_THE_FLOOR:
			for (int i=0; i<length_in_samples; i++) {
				data.push_back(i % quarter_note_length == 0); // Creates note at each beat
			}
			break;
		case DUBSTEP:
			kick_locations.push_back(0);
			kick_locations.push_back(samples_per_measure);
			if (rand() % 100 < 30) kick_locations.push_back(samples_per_measure * 1 / 4);
			if (rand() % 100 < 40) kick_locations.push_back(samples_per_measure * 3 / 8);
			if (rand() % 100 < 15) kick_locations.push_back(samples_per_measure * 7 / 16);
			if (rand() % 100 < 25) kick_locations.push_back(samples_per_measure * 5 / 8);
			if (rand() % 100 < 30) kick_locations.push_back(samples_per_measure * 3 / 4);
			if (rand() % 100 < 25) kick_locations.push_back(samples_per_measure * 7 / 8);
			if (rand() % 100 < 10) kick_locations.push_back(samples_per_measure * 15 / 16);
			if (rand() % 100 < 30) kick_locations.push_back(samples_per_measure * 5 / 4);
			if (rand() % 100 < 40) kick_locations.push_back(samples_per_measure * 11 / 8);
			if (rand() % 100 < 15) kick_locations.push_back(samples_per_measure * 23 / 16);
			if (rand() % 100 < 25) kick_locations.push_back(samples_per_measure * 13 / 8);
			if (rand() % 100 < 30) kick_locations.push_back(samples_per_measure * 7 / 4);
			if (rand() % 100 < 25) kick_locations.push_back(samples_per_measure * 15 / 8);
			if (rand() % 100 < 10) kick_locations.push_back(samples_per_measure * 31 / 16);
			for (int i=0; i<length_in_samples; i++) {
				add_sample = false;
				for (int j=0; j<kick_locations.size(); j++) {
					if (counter == kick_locations[j]) {
						add_sample = true;
					}
				}
				data.push_back(add_sample);
				if (counter++ >= samples_per_measure * 2 - 1) {
					counter = 0;
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
