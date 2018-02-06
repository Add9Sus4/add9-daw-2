/*
 * snare_pattern.h
 *
 *  Created on: Feb 6, 2018
 *      Author: aarondawson
 */

#ifndef SNARE_PATTERN_H_
#define SNARE_PATTERN_H_

#include <iostream>
#include <vector>

#include "pattern.h"

namespace add9daw2 {

typedef enum SnarePatternType {
	EIGHT_BAR_BUILD
} SnarePatternType;

class SnarePattern: public Pattern {
public:
	SnarePattern(SnarePatternType snare_pattern_type) : Pattern(SNARE),
		snare_pattern_type_(snare_pattern_type) {}
	virtual ~SnarePattern();
	std::vector<bool> get_data(int length_in_samples, int samples_per_measure) override;
private:
	SnarePatternType snare_pattern_type_;
};

} /* namespace add9daw2 */

#endif /* SNARE_PATTERN_H_ */
