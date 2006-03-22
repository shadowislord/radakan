#include "freetype.hpp"
#include "shr.hpp"
#include "world.hpp"

// ASCII code for the escape key. 
#define ESCAPE 27

using namespace std;

int main_window;	//	The number of our GLUT window
vector <Object *> objects;
World * tsl;
Font * font;

// A general OpenGL initialization function.  Sets all of the initial parameters.
void InitGL (int width, int height)		// We call this right after our OpenGL window is created.
{
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
	glClearDepth (1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc (GL_LESS);				// The Type Of Depth Test To Do
	glEnable (GL_DEPTH_TEST);			// Enables Depth Testing
	glShadeModel (GL_SMOOTH);			// Enables Smooth Color Shading

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();				// Reset The Projection Matrix

	gluPerspective (45.0f, GLfloat (width) / GLfloat (height), 0.1f, 100.0f);	// Calculate The Aspect Ratio Of The Window
	glMatrixMode (GL_MODELVIEW);
}

// The function called when our window is resized (which shouldn't happen, because we're fullscreen)
void ReSizeGLScene (int new_width, int new_height)
{
	assert (new_height != 0);					// Prevent A Divide By Zero
	glViewport(0, 0, new_width, new_height);	// Reset The Current Viewport And Perspective Transformation

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();

	gluPerspective (45.0f, GLfloat (new_width) / GLfloat (new_height), 0.1f, 100.0f);
	glMatrixMode (GL_MODELVIEW);
}

// The main drawing function.
void DrawGLScene()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity ();						// Reset The View

	glTranslatef(-1.5f, 0.0f, -6.0f);			// Move Left 1.5 Units And Into The Screen 6.0

	//	draw a triangle
	glBegin (GL_POLYGON);					// start drawing a polygon
		glVertex3f ( 0.0f, 1.0f, 0.0f);		// Top
		glVertex3f ( 1.0f,-1.0f, 0.0f);		// Bottom Right
		glVertex3f (-1.0f,-1.0f, 0.0f);		// Bottom Left
	glEnd ();								// we're done with the polygon

  glTranslatef(3.0f,0.0f,0.0f);		        // Move Right 3 Units
	
	// draw a square (quadrilateral)
	glBegin(GL_QUADS);						// start drawing a polygon (4 sided)
		glVertex3f(-1.0f, 1.0f, 0.0f);		// Top Left
		glVertex3f( 1.0f, 1.0f, 0.0f);		// Top Right
		glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
	glEnd();								// done with the polygon

	font->print (100, 300, "Tinuswdfnhwdfnjwnjwgfjwgjnwgfwgjfwg");

	// swap buffers to display, since we're double buffered.
	glutSwapBuffers ();
}

// The function called whenever a key is pressed.
void keyPressed (unsigned char key, int x, int y) 
{
	// avoid thrashing this procedure
	usleep (100);

	// If escape is pressed, kill everything.
	if (key == ESCAPE)
	{
		delete tsl;
		delete font;

		// shut down our window
		glutDestroyWindow (main_window);

		bool objects_left = false;
		for (unsigned int i = 0; i < objects.size (); i++)
		{
			if (objects.at (i) != NULL)
			{
				if (! objects_left)
				{
					cout << "Objects left: " << endl;
					objects_left = true;
				}
				cout << i << ": " << * objects.at (i) << endl;
			}
		}
		if (! objects_left)
		{
			cout << "tslrpg finished successfully. " << endl;
			exit (0);
		}
		cerr << "ERROR: tslrpg finished with orphaned objects. " << endl;
		exit (1);

    }
}

int main (int argc, char **argv)
{
//	Initialize GLUT - info at http://reality.sgi.com/mjk/spec3/spec3.html
	glutInit (&argc, argv);

//	Select type of Display mode:
//	Double buffer
//	RGBA color
//	Alpha components supported
//	Depth buffer
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	// get a 640 x 480 window
	glutInitWindowSize (800, 600);

	// the window starts at the upper left corner of the screen
	glutInitWindowPosition (0, 0);

	// Open a window
	main_window = glutCreateWindow ("OpenGL");

	// Register the function to do all our OpenGL drawing.
	glutDisplayFunc (&DrawGLScene);

	// Go fullscreen.  This is the soonest we could possibly go fullscreen.
	glutFullScreen();

	// Even if there are no events, redraw our gl scene.
	glutIdleFunc (&DrawGLScene);

	// Register the function called when our window is resized.
	glutReshapeFunc (&ReSizeGLScene);

	// Register the function called when the keyboard is pressed.
	glutKeyboardFunc (&keyPressed);

	// Initialize our window.
	InitGL (800, 600);

	font = new Font ("fonts/test.ttf", 30);

	tsl = new World ("test_world");
	Obstacle * obstacle = new Obstacle ("abc");
	
	tsl->get_tile (6, 4)->set_obstacle (obstacle);

	// Start Event Processing Engine
	glutMainLoop ();

	abort ();
	return 1;
}
