/*
 * hat_pattern.h
 *
 *  Created on: Feb 6, 2018
 *      Author: aarondawson
 */

#ifndef HAT_PATTERN_H_
#define HAT_PATTERN_H_

#include <iostream>
#include <vector>

#include "pattern.h"

namespace add9daw2 {

typedef enum HatPatternType {
	OFFBEAT,
	TRAP
} HatPatternType;

class HatPattern: public Pattern {
public:
	HatPattern(HatPatternType hat_pattern_type) : Pattern(HAT),
		hat_pattern_type_(hat_pattern_type) {}
	virtual ~HatPattern();
	std::vector<bool> get_data(int length_in_samples, int samples_per_measure) override;
private:
	HatPatternType hat_pattern_type_;
};

} /* namespace add9daw2 */

#endif /* HAT_PATTERN_H_ */
