/*
 * clap_pattern.h
 *
 *  Created on: Feb 6, 2018
 *      Author: aarondawson
 */

#ifndef CLAP_PATTERN_H_
#define CLAP_PATTERN_H_

#include <iostream>
#include <vector>

#include "pattern.h"

namespace add9daw2 {

typedef enum ClapPatternType {
	TWO_AND_FOUR
} ClapPatternType;

class ClapPattern: public Pattern {
public:
	ClapPattern(ClapPatternType clap_pattern_type) : Pattern(CLAP),
		clap_pattern_type_(clap_pattern_type) {}
	virtual ~ClapPattern();
	std::vector<bool> get_data(int length_in_samples, int samples_per_measure) override;
private:
	ClapPatternType clap_pattern_type_;
};

} /* namespace add9daw2 */

#endif /* CLAP_PATTERN_H_ */
