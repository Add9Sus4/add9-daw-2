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
	Rect(double l, double t, double r, double b) : l(l), t(t), r(r), b(b) {}
	virtual ~Rect() {}

	inline void set(double l, double t, double r, double b) {
		this->l = l;
		this->t = t;
		this->r = r;
		this->b = b;
	}
	inline void setL(double l) { this->l = l; }
	inline void setT(double t) { this->t = t; }
	inline void setR(double r) { this->r = r; }
	inline void setB(double b) { this->b = b; }

	inline double L() { return l; }
	inline double T() { return t; }
	inline double R() { return r; }
	inline double B() { return b; }
	inline double height() { return t - b; }
	inline double width() { return r - l; }
private:
	double l, t, r, b;
};

} // namespace add9daw2

#endif /* ADD9DAW2_SRC_RECT_H_ */
