/*
 * kick_pattern.h
 *
 *  Created on: Feb 1, 2018
 *      Author: aarondawson
 */

#ifndef KICK_PATTERN_H_
#define KICK_PATTERN_H_

#include <iostream>
#include <vector>

#include "pattern.h"

namespace add9daw2 {

typedef enum KickPatternType {
	FOUR_ON_THE_FLOOR,
	DUBSTEP
} KickPatternType;

class KickPattern: public Pattern {
public:
	KickPattern(KickPatternType kick_pattern_type) : Pattern(KICK),
		kick_pattern_type_(kick_pattern_type) {}
	virtual ~KickPattern();
	std::vector<bool> get_data(int length_in_samples, int samples_per_measure) override;
private:
	KickPatternType kick_pattern_type_;
};

} /* namespace add9daw2 */

#endif /* KICK_PATTERN_H_ */
