/*
 * Mouse.h
 *
 *  Created on: Jan 25, 2018
 *      Author: aarondawson
 */

#ifndef MOUSE_H_
#define MOUSE_H_

namespace add9daw2 {

class AudioFile;

class Mouse {
public:
	Mouse() : x(0), y(0), lmb(0), mmb(0), rmb(0), xpress(0), ypress(0), file(0),
	playing(false) {}
	bool file_selected() { return file != 0; }
	bool is_playing() { return playing; }
	void ClearFile() { file = 0; }
	void TogglePlaying() { playing = !playing; }
	double x;		/*	the x coordinate of the mouse cursor	*/
	double y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/
	double xpress; /*	stores the x-coord of when the first button press occurred	*/
	double ypress; /*	stores the y-coord of when the first button press occurred	*/
	AudioFile* file; // If an audio file is being dragged, this will store a reference to it
	bool playing;
};

}

#endif /* MOUSE_H_ */
