/*
 * Timer.h
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#ifndef TIMER_H_
#define TIMER_H_

#define DOUBLE_CLICK_TIME	500

namespace add9daw2 {

class Timer {
public:
	Timer() {
		start_time_ = get_milli_count();
		last_time_ = get_milli_span(start_time_);
		set_duration();
	}
	virtual ~Timer() {}

	inline bool is_double_click() { return duration_ < DOUBLE_CLICK_TIME; }

	inline int get_milli_count(){
		timeb tb;
		ftime(&tb);
		int n_count = tb.millitm + (tb.time & 0xfffff) * 1000;
		return n_count;
	}
	inline int get_milli_span(int n_time_start){
		int n_span = get_milli_count() - n_time_start;
		if(n_span < 0)
			n_span += 0x100000 * 1000;
		return n_span;
	}
	inline int get_time() { return start_time_; }
	inline int get_last_time() { return last_time_; }
	inline int get_duration() { return duration_; }

	inline void set_time() { last_time_ = get_milli_span(start_time_); }
	inline void set_duration() { duration_ = get_milli_span(start_time_) - last_time_; }
private:
	int last_time_, start_time_, duration_;
};

}

#endif /* TIMER_H_ */
