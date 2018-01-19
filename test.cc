/*
 * MainWindow.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#include <math.h>
#include <portaudio.h>
#include <sndfile.h>
#include <sys/timeb.h>
#include <time.h>

#include <ctime>

#include "main_window.h"

// How quickly two clicks must occur in sequence in order to be considered a 'double click'
#define DOUBLE_CLICK_TIME	500
#define BLOCK_SIZE			2048
#define SAMPLE_RATE			44100
#define AMP_DB 				8.685889638065036553
#define MIN_FREQ			1
#define MAX_FREQ			44100/2

namespace add9daw2 {

MainWindow* main_window;

int start_time;
int last_time;

GLfloat g_mouse_x;
GLfloat g_mouse_y;
typedef struct Mouse
{
	double x;		/*	the x coordinate of the mouse cursor	*/
	double y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/

	/*
	 *	These two variables are a bit odd. Basically I have added these to help replicate
	 *	the way that most user interface systems work. When a button press occurs, if no
	 *	other button is held down then the co-ordinates of where that click occured are stored.
	 *	If other buttons are pressed when another button is pressed it will not update these
	 *	values.
	 *
	 *	This allows us to "Set the Focus" to a specific portion of the screen. For example,
	 *	in maya, clicking the Alt+LMB in a view allows you to move the mouse about and alter
	 *	just that view. Essentually that viewport takes control of the mouse, therefore it is
	 *	useful to know where the first click occured....
	 */
	double xpress; /*	stores the x-coord of when the first button press occurred	*/
	double ypress; /*	stores the y-coord of when the first button press occurred	*/
} Mouse;

Mouse the_mouse = {0,0,0,0,0};
int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int n_count = tb.millitm + (tb.time & 0xfffff) * 1000;
	return n_count;
}

int getMilliSpan(int n_time_start){
	int n_span = getMilliCount() - n_time_start;
	if(n_span < 0)
		n_span += 0x100000 * 1000;
	return n_span;
}

double AmpToDB(double amp) {
  return AMP_DB * log(fabs(amp));
}

/*
 *  Description:  Callback for Port Audio
 */
static int paCallback(const void *input_buffer, void *output_buffer,
		unsigned long frames_per_buffer, const PaStreamCallbackTimeInfo* time_info,
		PaStreamCallbackFlags status_flags, void *user_data) {
//	paData *data = (paData *) userData;

	float* audio = main_window->nextBlock(frames_per_buffer, 2);

//	float *inBuf = (float*) inputBuffer;
	float *out_buf = (float*) output_buffer;

	for (int i=0; i<frames_per_buffer; i++) {
		out_buf[2*i] = audio[2*i];
		out_buf[2*i + 1] = audio[2*i + 1];
	}

	delete[] audio;

	return paContinue;
}

/*
 * This function is responsible for starting PortAudio and OpenGL.
 */
void runPortAudio() {

	PaStream* stream;
	PaStreamParameters output_parameters;
	PaStreamParameters input_parameters;
	PaError err;
	/* Initialize PortAudio */
	Pa_Initialize();
	/* Set output stream parameters */
	output_parameters.device = Pa_GetDefaultOutputDevice();
	output_parameters.channelCount = 2;
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
			SAMPLE_RATE, BLOCK_SIZE, paNoFlag, paCallback, NULL);

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


void displayFunc(); // definition

void MouseMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	//	int dx = x - TheMouse.x;
	//	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	the_mouse.x = (double) x / main_window->getWidth();
	the_mouse.y = 1.0 - (double) y / main_window->getHeight();
	std::cout << "Mouse Dragged: " << the_mouse.x << ", " << the_mouse.y << std::endl;
	main_window->onDrag(the_mouse.x, the_mouse.y);
	glutPostRedisplay();
}
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	update the mouse position
	 */
	the_mouse.x = (double) x / main_window->getWidth();
	the_mouse.y = 1.0 - (double) y / main_window->getHeight();

	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN)
	{
		the_mouse.xpress = the_mouse.x;
		the_mouse.ypress = the_mouse.y;
		// Check for double click
		int current_time = getMilliSpan(start_time);
		std::cout << "Mouse Pressed: " << the_mouse.x << ", " << the_mouse.y << std::endl;
		int time_between_clicks = current_time - last_time;
		std::cout << "Time elapsed: " << time_between_clicks << std::endl;
		last_time = getMilliSpan(start_time);

		// Double click recorded
		if (time_between_clicks < DOUBLE_CLICK_TIME) {
			std::cout << "Double click recorded" << std::endl;
			main_window->onDoubleClick(the_mouse.x, the_mouse.y);
		} else {
			main_window->onClick(the_mouse.x, the_mouse.y);
		}

		/*
		 *	This holds the location of the first mouse click
		 */
		if ( !(the_mouse.lmb || the_mouse.mmb || the_mouse.rmb) ) {
			the_mouse.xpress = the_mouse.x;
			the_mouse.ypress = the_mouse.y;
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
		std::cout << "Mouse Released: " << the_mouse.x << ", " << the_mouse.y << std::endl;
		main_window->onUpClick(the_mouse.x, the_mouse.y);
	}

	//	printf("Mouse: x = %d, y = %d\n", TheMouse.x, TheMouse.y);

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}
void MouseHover(int x, int y) {
	the_mouse.x = (double) x / main_window->getWidth();
	the_mouse.y = 1.0 - (double) y / main_window->getHeight();
	std::cout << "Mouse Over: " << the_mouse.x << ", " << the_mouse.y << std::endl;
	displayFunc();
}
void displayFunc() {

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
	main_window->draw(0.0, 0.0);


	// After drawing
	glutSwapBuffers();
	glFlush();
}

//-----------------------------------------------------------------------------
// Name: initialize_graphics( )
// Desc: sets initial OpenGL states and initializes any application data
//-----------------------------------------------------------------------------
void initialize_graphics() {

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

//-----------------------------------------------------------------------------
// Name: initialize_glut( )
// Desc: Initializes Glut with the global vars
//-----------------------------------------------------------------------------
void initialize_glut(int argc, char *argv[]) {
	// initialize GLUT
	glutInit(&argc, argv);
	// double buffer, use rgb color, enable depth buffer
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// initialize the window size
	glutInitWindowSize(main_window->getWidth(), main_window->getHeight());
	// set the window postion
	glutInitWindowPosition(0, 0);
	// create the window
	glutCreateWindow("Add9 DAW");
	// full screen
	//	if (g_fullscreen)
	//		glutFullScreen();

	// set the idle function - called when idle
	//	glutIdleFunc(idleFunc);
	//	// set the display function - called when redrawing
	glutDisplayFunc(displayFunc);
	//	// set the reshape function - called when client area changes
	//	glutReshapeFunc(reshapeFunc);
	//	// set the keyboard function - called on keyboard events
	//	glutKeyboardFunc(keyboardFunc);
	//	// get mouse position
	//	glutPassiveMotionFunc(passiveMotionFunc);
	//
	//	glutKeyboardUpFunc(keyboardUpFunc);
	//
	glutMouseFunc(MouseButton);

	glutMotionFunc(MouseMotion);

	glutPassiveMotionFunc(MouseHover);

	// do our own initialization
	initialize_graphics();
}

} // namespace add9daw2

int main(int argc, char **argv) {
	using namespace add9daw2;
	start_time = getMilliCount();
	last_time = getMilliSpan(start_time);

	main_window = new MainWindow(878, 1440, BLOCK_SIZE, 2);

	initialize_glut(argc, argv);
	runPortAudio();
}
