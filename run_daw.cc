/*
 * MainWindow.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#define BLOCK_SIZE			2048
#define SAMPLE_RATE			44100

#include <glut/GLUT.h>
#include <math.h>
#include <portaudio.h>
#include <sndfile.h>
#include <sys/timeb.h>
#include <time.h>

#include <ctime>
#include <iostream>
#include <vector>

#include "arrange_window.h"
#include "files_window.h"
#include "mouse.h"
#include "timer.h"
namespace add9daw2 {

Mouse* mouse;
ArrangeWindow* arrangeWindow;
FilesWindow* filesWindow;
Timer* timer;
int height = 878, width = 1440;

int output_channels = STEREO;


double ConvertXCoord(int x) {
	return (double) x * 2.0 / (double) width - 1.0;
}

double ConvertYCoord(int y) {
	return 1.0 - (double) y * 2.0 / (double) height;
}

void KeyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
		case ' ':
			std::cout << "Spacebar pressed" << std::endl;
			mouse->TogglePlaying();
			break;
		default:
			break;
	}
}
void MouseMotion(int x, int y)
{
	mouse->x = ConvertXCoord(x);
	mouse->y = ConvertYCoord(y);
	std::cout << "Mouse Dragged: " << mouse->x << ", " << mouse->y << std::endl;
	arrangeWindow->ReceiveMouseEvent(mouse, DRAG);
	filesWindow->ReceiveMouseEvent(mouse, DRAG);
	glutPostRedisplay();
}
void MouseButton(int button,int state,int x, int y)
{
	mouse->x = ConvertXCoord(x);
	mouse->y = ConvertYCoord(y);
	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN)
	{
		mouse->xpress = mouse->x;
		mouse->ypress = mouse->y;
		// Check for double click
		timer->set_duration();
		std::cout << "Mouse Pressed: " << mouse->x << ", " << mouse->y << std::endl;
		int duration = timer->get_duration();
		std::cout << "Time elapsed: " << duration << std::endl;
		timer->set_time();

		// Double click recorded
		if (timer->is_double_click()) {
			std::cout << "Double click recorded" << std::endl;
			arrangeWindow->ReceiveMouseEvent(mouse, DOUBLE_CLICK);
			filesWindow->ReceiveMouseEvent(mouse, DOUBLE_CLICK);
		} else {
			arrangeWindow->ReceiveMouseEvent(mouse, CLICK);
			filesWindow->ReceiveMouseEvent(mouse, CLICK);
		}

		/*
		 *	This holds the location of the first mouse click
		 */
		if ( !(mouse->lmb || mouse->mmb || mouse->rmb) ) {
			mouse->xpress = mouse->x;
			mouse->ypress = mouse->y;
		}

		/*
		 *	Which button was pressed?
		 */
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			break;
		}
	} else {
		std::cout << "Mouse Released: " << mouse->x << ", " << mouse->y << std::endl;
		arrangeWindow->ReceiveMouseEvent(mouse, RELEASE);
		filesWindow->ReceiveMouseEvent(mouse, RELEASE);
	}

	//	printf("Mouse: x = %d, y = %d\n", TheMouse.x, TheMouse.y);

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}
void DisplayFunc() {

	// Before drawing
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw stuff here
	arrangeWindow->Draw();
	filesWindow->Draw();

	// After drawing
	glutSwapBuffers();
	glFlush();
}
void IdleFunc() {
	if (mouse->is_playing())  {
		DisplayFunc();
	}
}
void MouseHover(int x, int y) {
	mouse->x = ConvertXCoord(x);
	mouse->y = ConvertYCoord(y);
	std::cout << "Mouse Over: " << mouse->x << ", " << mouse->y << std::endl;
	arrangeWindow->ReceiveMouseEvent(mouse, HOVER);
	filesWindow->ReceiveMouseEvent(mouse, HOVER);
	DisplayFunc();
}
void InitializeGraphics() {

	glClearColor(0.12f, 0.15f, 0.18f, 0.0f);					// Background Color
	// set the shading model to 'smooth'
	glShadeModel( GL_SMOOTH);
	// enable depth
	glEnable( GL_DEPTH_TEST);
	// set the front faces of polygons
	glFrontFace( GL_CCW);
	// set fill mode
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	// enable lighting
	glEnable( GL_LIGHTING);
	// enable lighting for front
	glLightModeli( GL_FRONT_AND_BACK, GL_TRUE);
	// material have diffuse and ambient lighting
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	// enable color
	glEnable( GL_COLOR_MATERIAL);
	// normalize (for scaling)
	glEnable( GL_NORMALIZE);
	// line width
	glLineWidth(1.0);

	// enable light 0
	glEnable( GL_LIGHT0);

	glEnable( GL_LIGHT1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}
void InitializeGlut(int argc, char *argv[]) {
	mouse = new Mouse();
	timer = new Timer();
	arrangeWindow = new ArrangeWindow(-0.6, 0.8, 0.7, -0.7, NULL);
	filesWindow = new FilesWindow(-0.99, 0.8, -0.61, -0.7, NULL);
	// initialize GLUT
	glutInit(&argc, argv);
	// double buffer, use rgb color, enable depth buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// initialize the window size
	glutInitWindowSize(width, height);
	// set the window postion
	glutInitWindowPosition(0, 0);
	// create the window
	glutCreateWindow("Add9 DAW");
	// full screen
	//	if (g_fullscreen)
	//		glutFullScreen();

	// set the idle function - called when idle
	glutIdleFunc(IdleFunc);
	//	// set the display function - called when redrawing
	glutDisplayFunc(DisplayFunc);
	//	// set the reshape function - called when client area changes
	//	glutReshapeFunc(reshapeFunc);
	//	// set the keyboard function - called on keyboard events
		glutKeyboardFunc(KeyboardFunc);
	//
	//	glutKeyboardUpFunc(keyboardUpFunc);
	//
	glutMouseFunc(MouseButton);

	glutMotionFunc(MouseMotion);

	glutPassiveMotionFunc(MouseHover);

	// do our own initialization
	InitializeGraphics();
}


static int PaCallback(const void *input_buffer, void *output_buffer,
		unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info,
		PaStreamCallbackFlags status_flags, void *user_data) {
	float *out_buf = (float*) output_buffer;
	if (mouse->is_playing()) {
		double* audio_data = arrangeWindow->GetAudio(frames_per_buffer, output_channels);

		for (int i=0; i<frames_per_buffer*output_channels; i++) {
			out_buf[i] = audio_data[i];
		}
		delete[] audio_data;
		arrangeWindow->AdvancePlaybackLocator(frames_per_buffer);
	} else {
		for (int i=0; i<frames_per_buffer*output_channels; i++) {
			out_buf[i] = 0.0;
		}
	}
//	for (int i=0; i<frames_per_buffer*output_channels; i++) {
//		std::cout << "audio[" << i << "]: " << audio_data[i] << std::endl;
//	}
//	}
//	paData *data = (paData *) userData;

//	double *in_buf = (double*) input_buffer;

	return paContinue;
}

void RunPortAudio() {

	PaStream* stream;
	PaStreamParameters output_parameters;
	PaStreamParameters input_parameters;
	PaError err;
	/* Initialize PortAudio */
	Pa_Initialize();
	/* Set output stream parameters */
	output_parameters.device = Pa_GetDefaultOutputDevice();
	output_parameters.channelCount = output_channels;
	output_parameters.sampleFormat = paFloat32;
	output_parameters.suggestedLatency = Pa_GetDeviceInfo(
			output_parameters.device)->defaultLowOutputLatency;
	output_parameters.hostApiSpecificStreamInfo = NULL;
	/* Set input stream parameters */
	input_parameters.device = Pa_GetDefaultInputDevice();
	input_parameters.channelCount = 1;
	input_parameters.sampleFormat = paFloat32;
	input_parameters.suggestedLatency =
			Pa_GetDeviceInfo(input_parameters.device)->defaultLowInputLatency;
	input_parameters.hostApiSpecificStreamInfo = NULL;
	/* Open audio stream */
	err = Pa_OpenStream(&stream, &input_parameters, &output_parameters,
			SAMPLE_RATE, BLOCK_SIZE, paNoFlag, PaCallback, NULL);

	if (err != paNoError) {
		printf("PortAudio error: open stream: %s\n", Pa_GetErrorText(err));
	}
	/* Start audio stream */
	err = Pa_StartStream(stream);
	if (err != paNoError) {
		printf("PortAudio error: start stream: %s\n", Pa_GetErrorText(err));
	}

	glutMainLoop();

	/* Get user input */
	char ch = '0';
	while (ch != 'q') {
		printf("Press 'q' to finish execution: ");
		ch = getchar();
	}

	err = Pa_StopStream(stream);
	/* Stop audio stream */
	if (err != paNoError) {
		printf("PortAudio error: stop stream: %s\n", Pa_GetErrorText(err));
	}
	/* Close audio stream */
	err = Pa_CloseStream(stream);
	if (err != paNoError) {
		printf("PortAudio error: close stream: %s\n", Pa_GetErrorText(err));
	}
	/* Terminate audio stream */
	err = Pa_Terminate();
	if (err != paNoError) {
		printf("PortAudio error: terminate: %s\n", Pa_GetErrorText(err));
	}

}


}  // namespace add9daw2


int main(int argc, char **argv) {
	using namespace add9daw2;
	srand(time(NULL));
	InitializeGlut(argc, argv);
	RunPortAudio();
}
