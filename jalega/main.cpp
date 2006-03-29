#include "shr.hpp"
#include "world.hpp"
#include "event_engine.hpp"

using namespace std;

int main_window;	//	GLUT window number
vector <Object *> objects;
World * tsl;
Event_Engine * event_engine;
SHR * shr;
Texture * font_texture_a;

void
	resize
	(int new_width, int new_height)
{
	GLdouble size;
	GLdouble aspect;

	// Use the whole window.
	glViewport (0, 0, new_width, new_height);

	// We are going to do some 2-D orthographic drawing.
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	size = (GLdouble)((new_width >= new_height) ? new_width : new_height) / 2.0;
	if (new_width <= new_height)
	{
		aspect = (GLdouble) new_height / (GLdouble) new_width;
		glOrtho (-size, size, -size * aspect, size * aspect, -10000.0, 10000.0);
	}
	else
	{
		aspect = (GLdouble) new_width / (GLdouble) new_height;
		glOrtho (-size * aspect, size * aspect, -size, size, -10000.0, 10000.0);
	}

	// Make the world and window coordinates coincide so that 1.0 in
	// model space equals one pixel in window space.
	glScaled (aspect, aspect, 1.0);

	// Now determine where to draw things.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void
	quit
	()
{
	delete event_engine;
	delete tsl;
	delete shr;

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

void
	handle_input
	(GLubyte key, GLint x, GLint y)
{
	string a = "";
	a.push_back (key);
	event_engine->process (new Input_Event (a, x, y));
}

void
	draw
	()
{
	shr->draw ();
}

int
	main
	(int argc, char * * argv)
{
	shr = new SHR ();
	
	glutInit (&argc, argv);
	glutInitWindowSize (800, 600);

	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	main_window = glutCreateWindow ("tslrpg");

	glutDisplayFunc (draw);
	glutReshapeFunc (resize);
	glutKeyboardFunc (handle_input);
	
	glClearDepth (1.0);
	glDepthFunc (GL_LESS);

	font_texture_a = new Texture ("fonts/test.tga");
	shr->build_font (font_texture_a);
	glShadeModel (GL_SMOOTH);				// Enable Smooth Shading

	event_engine = new Event_Engine (quit, glutPostRedisplay);

	tsl = new World ("test_world");
	tsl->get_tile (6, 4)->set_obstacle (new Obstacle ("tree"));

	glutMainLoop ();

	return 1;
}
