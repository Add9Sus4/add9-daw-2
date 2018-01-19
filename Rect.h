/*
 * Rect.h
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#ifndef ADD9DAW2_SRC_RECT_H_
#define ADD9DAW2_SRC_RECT_H_

namespace add9daw2 {

struct Rect {
public:
	Rect(double left, double top, double right, double bottom) : left(left), top(top), right(right), bottom(bottom) {}
	virtual ~Rect() {}

	inline void set(double left, double top, double right, double bottom) {
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	inline void set_left(double left) { this->left = left; }
	inline void set_top(double top) { this->top = top; }
	inline void set_right(double right) { this->right = right; }
	inline void set_bottom(double bottom) { this->bottom = bottom; }

	inline double get_left() { return left; }
	inline double get_top() { return top; }
	inline double get_right() { return right; }
	inline double get_bottom() { return bottom; }
	inline double get_height() { return top - bottom; }
	inline double get_width() { return right - left; }
private:
	double left, top, right, bottom;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_RECT_H_ */
