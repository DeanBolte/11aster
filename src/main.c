/*
*	main contains all the functions that directly connect with or assist in the functionality of OpenGL
*
*	Main Contributer: Dean
* 
*	Please read LICENSE.txt
*/

#include "../vs/engine.h"

#define KEY_ESC 27

int port_width = 0;
int port_height = 0;

// Frametime data
float previous_frametime;

// Keyboard input on press
void inputDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_ESC:
		exit(EXIT_SUCCESS);
		break;
	case 'a':
		inputKeyboard("left", 1);
		break;
	case 'w':
		inputKeyboard("up", 1);
		break;
	case 'd':
		inputKeyboard("right", 1);
		break;
	case 's':
		inputKeyboard("down", 1);
		break;
	case 32:
		inputKeyboard("space", 1);
		break;
	case 'p':
		inputKeyboard("test", 1);
		break;
	default:
		break;
	}
}

// Keyboard input on release
void inputUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		inputKeyboard("left", 0);
		break;
	case 'w':
		inputKeyboard("up", 0);
		break;
	case 'd':
		inputKeyboard("right", 0);
		break;
	case 's':
		inputKeyboard("down", 0);
		break;
	case 32:
		inputKeyboard("space", 0);
		break;
	case 'p':
		inputKeyboard("test", 0);
		break;
	default:
		break;
	}
}

// Mouse state change function
void mouseInput(int button, int state, int x, int y) {
	// represents whether a mouse button is being pressed
	int pressed = 0;
	if (state == GLUT_DOWN) {
		pressed = 1;
	}
	else {
		pressed = 0;
	}

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		inputMouse("shoot", pressed);
		break;
	default:
		break;
	}
}

// Resize on reshaping of screen
void on_reshape(int w, int h) {
	glViewport(0, 0, w, h);

	port_width = w;  
	port_height = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();  
	glOrtho(0.0, w, 0.0, h, -1.0, 1.0); 
}

// Display frame
void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Highlight Colour
	glColor3f(highColour.r, highColour.g, highColour.b);
	// Background Colour
	glClearColor(lowColour.r, lowColour.g, lowColour.b, 1.0);

	// Call Game Engine to handle rendering
	render();

	// Error Checking
	int err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("display: %s\n", gluErrorString(err));
	}

	glutSwapBuffers();
}

// Idle frame
void on_idle() { 
	// Calculate delta between frames
	float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;  
	float delta = currentTime - previous_frametime;

	// Call to Game Engine to update
	update(delta, port_width, port_height);
	previous_frametime = currentTime;

	glutPostRedisplay(); 
}

// Initialises the opengl application
void init_app(int* argcp, char** argv) {
	// Set up display
	glutInit(argcp, argv); 
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);  
	glutInitWindowSize(1024, 768);
	glutCreateWindow("s3784635 Assignment 1");
	glutReshapeFunc(on_reshape);

	// input functionality
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(inputDown);
	glutKeyboardUpFunc(inputUp);
	glutMouseFunc(mouseInput);

	// OpenGL control functions
	glutDisplayFunc(on_display);
	glutIdleFunc(on_idle); 

	// Initialise screen size
	port_width = 1024;
	port_height = 768;

	// Initialise Game Engine
	init(port_width, port_height);

	// Get time of init for calculating delta
	previous_frametime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
}

int main(int argc, char** argv)
{
	init_app(&argc, argv);
	glutMainLoop();
	return EXIT_SUCCESS;
}