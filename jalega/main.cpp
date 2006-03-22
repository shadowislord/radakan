#include "shr.hpp"
#include "world.hpp"

using namespace std;


#define MODE_BITMAP true
#define MODE_STROKE false

static bool mode;
static int font_index;


int main_window;	//	The number of our GLUT window
vector <Object *> objects;
World * tsl;

// A general OpenGL initialization function.  Sets all of the initial parameters.
void init_gl (int width, int height)		// We call this right after our OpenGL window is created.
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

void
print_bitmap_string(void* font, char* s)
{
	if (s && strlen(s)) {
		while (*s) {
			glutBitmapCharacter(font, *s);
			s++;
		}
	}
}

void
print_stroke_string(void* font, char* s)
{
	if (s && strlen(s)) {
		while (*s) {
			glutStrokeCharacter(font, *s);
			s++;
		}
	}
}
void 
my_init()
{
	mode = MODE_BITMAP;
	font_index = 0;
}

void
	draw_stuff ()
{
	char string[8][256];
	unsigned int i, j;
	unsigned int count;
	void** bitmap_fonts[7] = {
		GLUT_BITMAP_9_BY_15,
		GLUT_BITMAP_8_BY_13,
		GLUT_BITMAP_TIMES_ROMAN_10,
		GLUT_BITMAP_TIMES_ROMAN_24,
		GLUT_BITMAP_HELVETICA_10,
		GLUT_BITMAP_HELVETICA_12,
		GLUT_BITMAP_HELVETICA_18
	};

	char* bitmap_font_names[7] = {
		"GLUT_BITMAP_9_BY_15",
		"GLUT_BITMAP_8_BY_13",
		"GLUT_BITMAP_TIMES_ROMAN_10",
		"GLUT_BITMAP_TIMES_ROMAN_24",
		"GLUT_BITMAP_HELVETICA_10",
		"GLUT_BITMAP_HELVETICA_12",
		"GLUT_BITMAP_HELVETICA_18"
	};

	void ** stroke_fonts[2] = {
		GLUT_STROKE_ROMAN,
		GLUT_STROKE_MONO_ROMAN
	};

	char * stroke_font_names[2] =
	{
		"GLUT_STROKE_ROMAN",
		"GLUT_STROKE_MONO_ROMAN"
	};

	GLfloat x, y, ystep, yild, stroke_scale;

	// Set up some strings with the characters to draw.
	count = 0;
	for (i=1; i < 32; i++) { // Skip zero - it's the null terminator!
		string[0][count] = i;
		count++;
	}
	string[0][count] = '\0';

	count = 0;
	for (i=32; i < 64; i++) {
		string[1][count] = i;
		count++;
	}
	string[1][count] = '\0';

	count = 0;
	for (i=64; i < 96; i++) {
		string[2][count] = i;
		count++;
	}
	string[2][count] = '\0';

	count = 0;
	for (i=96; i < 128; i++) {
		string[3][count] = i;
		count++;
	}
	string[3][count] = '\0';

	count = 0;
	for (i=128; i < 160; i++) {
		string[4][count] = i;
		count++;
	}
	string[4][count] = '\0';

	count = 0;
	for (i=160; i < 192; i++) {
		string[5][count] = i;
		count++;
	}
	string[5][count] = '\0';

	count = 0;
	for (i=192; i < 224; i++) {
		string[6][count] = i;
		count++;
	}
	string[6][count] = '\0';

	count = 0;
	for (i=224; i < 256; i++) {
		string[7][count] = i;
		count++;
	}
	string[7][count] = '\0';


	// Draw the strings, according to the current mode and font.
	glColor4f (0.0, 1.0, 0.0, 0.0);
	x = -225.0;
	y = 70.0;
	ystep = 100.0;
	yild = 20.0;
	if (mode == MODE_BITMAP)
	{
		glRasterPos2f(-150, y+1.25*yild);
		print_bitmap_string(
			bitmap_fonts[font_index], bitmap_font_names[font_index]);
		for (j=0; j<7; j++) {
			glRasterPos2f(x, y);
			print_bitmap_string(bitmap_fonts[font_index], string[j]);
			y -= yild;
		}
	}
	else
	{
		stroke_scale = 0.1f;
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); {
			glTranslatef(x, y+1.25*yild, 0.0);
			glScalef(stroke_scale, stroke_scale, stroke_scale);
			print_stroke_string(
				stroke_fonts[font_index], stroke_font_names[font_index]);
		} glPopMatrix();
		glPushMatrix(); {
			glTranslatef(x, y, 0.0);
			for (j=0; j<4; j++) {
				glPushMatrix(); {
					glScalef(stroke_scale, stroke_scale, stroke_scale);
					print_stroke_string(stroke_fonts[font_index], string[j]);
				} glPopMatrix();
				glTranslatef(0.0, -yild, 0.0);
			}
			glTranslatef(0.0, -ystep, 0.0);
		} glPopMatrix();
	}
}

// The main drawing function.
void DrawGLScene()
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity ();						// Reset The View

	glTranslatef(-1.5f, 0.0f, -6.0f);			// Move Left 1.5 Units And Into The Screen 6.0
	
	draw_stuff();

	//	draw a triangle
	glBegin (GL_POLYGON);					// start drawing a polygon
		glVertex3f ( 0.0f, 1.0f, 0.0f);		// Top
		glVertex3f ( 1.0f,-1.0f, 0.0f);		// Bottom Right
		glVertex3f (-1.0f,-1.0f, 0.0f);		// Bottom Left
	glEnd ();								// we're done with the polygon

  glTranslatef(3.0f,0.0f,0.0f);				  // Move Right 3 Units
	
	// draw a square (quadrilateral)
	glBegin(GL_QUADS);						// start drawing a polygon (4 sided)
		glVertex3f(-1.0f, 1.0f, 0.0f);		// Top Left
		glVertex3f( 1.0f, 1.0f, 0.0f);		// Top Right
		glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
		glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
	glEnd();								// done with the polygon



	// swap buffers to display, since we're double buffered.
	glutSwapBuffers ();
}

void
keyPressed(GLubyte key, GLint x, GLint y)
{

	// avoid thrashing this procedure
	usleep (100);
	switch (key)
	{
		case 27:	// Esc - Quit
		{
			delete tsl;
	
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
		case ' ':	 // Space - toggles mode.
			mode = (mode == MODE_BITMAP) ? MODE_STROKE : MODE_BITMAP;
			font_index = 0;
			glutPostRedisplay();
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			if (mode == MODE_BITMAP || key == '1' || key == '2')
			{
				font_index = key - '1';
			}
			glutPostRedisplay();
			break;

		default:
			break;
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

	my_init();

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

	tsl = new World ("test_world");
	Obstacle * obstacle = new Obstacle ("abc");
	
	tsl->get_tile (6, 4)->set_obstacle (obstacle);

	// Start Event Processing Engine
	glutMainLoop ();

	abort ();
	return 1;
}
