/*
 * MainWindow.cpp
 *
 *  Created on: Jan 17, 2018
 *      Author: aarondawson
 */

#include <time.h>
#include <ctime>
#include <sys/timeb.h>
#include <math.h>
#include <sndfile.h>
#include <portaudio.h>

#include "MainWindow.h"

// How quickly two clicks must occur in sequence in order to be considered a 'double click'
#define DOUBLE_CLICK_TIME	500
#define BLOCK_SIZE			2048
#define SAMPLE_RATE			44100
#define AMP_DB 				8.685889638065036553
#define MIN_FREQ			1
#define MAX_FREQ			44100/2

typedef struct {
	float amplitude1;
	float sampleRate;
	SNDFILE *infile1;
	SF_INFO sfinfo1;
	int channels;
	float buffer1[BLOCK_SIZE*2];
} paData;

MainWindow* mainWindow;

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

Mouse TheMouse = {0,0,0,0,0};
int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int getMilliSpan(int nTimeStart){
	int nSpan = getMilliCount() - nTimeStart;
	if(nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}

double AmpToDB(double amp) {
  return AMP_DB * log(fabs(amp));
}

/*
 *  Description:  Callback for Port Audio
 */
static int paCallback(const void *inputBuffer, void *outputBuffer,
		unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
		PaStreamCallbackFlags statusFlags, void *userData) {
	paData *data = (paData *) userData;

	float* audio = mainWindow->nextBlock(framesPerBuffer, 2);

	float *inBuf = (float*) inputBuffer;
	float *outBuf = (float*) outputBuffer;

	for (int i=0; i<framesPerBuffer; i++) {
		outBuf[2*i] = audio[2*i];
		outBuf[2*i + 1] = audio[2*i + 1];
	}

	delete[] audio;

	return paContinue;
}

/*
 * This function is responsible for starting PortAudio and OpenGL.
 */
void runPortAudio() {

	PaStream* stream;
	PaStreamParameters outputParameters;
	PaStreamParameters inputParameters;
	PaError err;
	/* Initialize PortAudio */
	Pa_Initialize();
	/* Set output stream parameters */
	outputParameters.device = Pa_GetDefaultOutputDevice();
	outputParameters.channelCount = 2;
	outputParameters.sampleFormat = paFloat32;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo(
			outputParameters.device)->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;
	/* Set input stream parameters */
	inputParameters.device = Pa_GetDefaultInputDevice();
	inputParameters.channelCount = 1;
	inputParameters.sampleFormat = paFloat32;
	inputParameters.suggestedLatency =
			Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	/* Open audio stream */
	err = Pa_OpenStream(&stream, &inputParameters, &outputParameters,
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
	TheMouse.x = (double) x / mainWindow->getWidth();
	TheMouse.y = 1.0 - (double) y / mainWindow->getHeight();
	std::cout << "Mouse Dragged: " << TheMouse.x << ", " << TheMouse.y << std::endl;
	mainWindow->onDrag(TheMouse.x, TheMouse.y);
	glutPostRedisplay();
}
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	update the mouse position
	 */
	TheMouse.x = (double) x / mainWindow->getWidth();
	TheMouse.y = 1.0 - (double) y / mainWindow->getHeight();

	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN)
	{
		TheMouse.xpress = TheMouse.x;
		TheMouse.ypress = TheMouse.y;
		// Check for double click
		int current_time = getMilliSpan(start_time);
		cout << "Mouse Pressed: " << TheMouse.x << ", " << TheMouse.y << endl;
		int time_between_clicks = current_time - last_time;
		cout << "Time elapsed: " << time_between_clicks << endl;
		last_time = getMilliSpan(start_time);

		// Double click recorded
		if (time_between_clicks < DOUBLE_CLICK_TIME) {
			cout << "Double click recorded" << endl;
			mainWindow->onDoubleClick(TheMouse.x, TheMouse.y);
		} else {
			mainWindow->onClick(TheMouse.x, TheMouse.y);
		}

		/*
		 *	This holds the location of the first mouse click
		 */
		if ( !(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb) ) {
			TheMouse.xpress = TheMouse.x;
			TheMouse.ypress = TheMouse.y;
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
		cout << "Mouse Released: " << TheMouse.x << ", " << TheMouse.y << endl;
		mainWindow->onUpClick(TheMouse.x, TheMouse.y);
	}

	//	printf("Mouse: x = %d, y = %d\n", TheMouse.x, TheMouse.y);

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}
void MouseHover(int x, int y) {
	TheMouse.x = (double) x / mainWindow->getWidth();
	TheMouse.y = 1.0 - (double) y / mainWindow->getHeight();
	cout << "Mouse Over: " << TheMouse.x << ", " << TheMouse.y << endl;
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
	mainWindow->draw(0.0, 0.0);


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
	glutInitWindowSize(mainWindow->getWidth(), mainWindow->getHeight());
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


int main(int argc, char **argv) {
	start_time = getMilliCount();
	last_time = getMilliSpan(start_time);

	mainWindow = new MainWindow(878, 1440, BLOCK_SIZE, 2);

	initialize_glut(argc, argv);
	runPortAudio();
}



