#include "event_engine.hpp"

using namespace std;

vector <Object *> objects;
World * tsl;
Event_Engine * event_engine;
SHR * shr;

void
	resize
	(int new_width, int new_height)
{
	shr->resize (new_width, new_height);
}

void
	quit
	()
{
	delete event_engine;
	delete tsl;
	delete shr;

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
	shr->draw_start ();
	tsl->draw (50, 100);
	shr->draw_stop ();
}

int
	main
	(int argc, char * * argv)
{
	shr = new SHR (argc, argv);
	
	glutDisplayFunc (draw);
	glutReshapeFunc (resize);
	glutKeyboardFunc (handle_input);

	event_engine = new Event_Engine (quit, glutPostRedisplay);

	tsl = new World ("test_world");
	tsl->get_tile (6, 4)->set_obstacle (new Obstacle ("tree"));

	glutMainLoop ();

	return 1;
}
