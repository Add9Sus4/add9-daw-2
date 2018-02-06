/*
 * pattern.h
 *
 *  Created on: Feb 1, 2018
 *      Author: aarondawson
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include <iostream>
#include <vector>

namespace add9daw2 {

typedef enum SampleType {
	CLAP,
	CRASH,
	EIGHT_O_EIGHT,
	HAT,
	HIGH_IMPACT,
	KICK,
	LOW_IMPACT,
	PERC,
	PITCH_DOWN,
	PITCH_RISER,
	PRYDA,
	PUNCHY_IMPACT,
	RIDE,
	SHAKER,
	SNAP,
	SNARE,
	SUB_DROP,
	SWEEP_DOWN,
	SWEEP_UP,
	TOM,
	WAR_DRUM
} SampleType;

class Pattern {
public:
	Pattern(SampleType sample_type) : sample_type_(sample_type) {}
	virtual ~Pattern() {}
	// A vector of sample start positions. The vector will contain n values, each representing
	// one sample within the pattern. If the value is false, no audio clip will be created,
	// while a value of true will create an audio clip at position n.
	virtual std::vector<bool> get_data(int length_in_samples, int samples_per_measure) = 0;
private:
	SampleType sample_type_;
};

} /* namespace add9daw2 */

#endif /* PATTERN_H_ */
