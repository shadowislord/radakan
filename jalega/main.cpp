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
	shr->render_quad (
		new D3 (0.4, 0.4, 0.4),
		new D3 (- 270.0, - 80.0, 0.0),
		new D3 ( 270.0, - 80.0, 0.0),
		new D3 ( 270.0, - 205.0, 0.0),
		new D3 (- 270.0, - 205.0, 0.0)
	);
	shr->print_bitmap (new D3 (0.9, 0.9, 0.9), - 260.0, - 100.0, 5,
		"The world has been at peace for decades.");
	shr->print_bitmap (new D3 (0.9, 0.9, 0.9), - 260.0, - 115.0, 5,
		"Elves, Dwarves and Humans live in harmony, thanks to (war), the war that defeated the Orcs.");
	shr->print_bitmap (new D3 (0.9, 0.9, 0.9), - 260.0, - 130.0, 5,
		"No trace of Orcs had been seen since then.");
	shr->print_bitmap (new D3 (0.9, 0.9, 0.9), - 260.0, - 145.0, 5,
		"But somehow the resting Orcs gathered in the caves beneath the northern mountains.");
	shr->print_bitmap (new D3 (0.9, 0.9, 0.9), - 260.0, - 160.0, 5,
		"As you know, the Orcs attacked yesterday (Elven city) and captured it.");
	shr->print_bitmap (new D3 (0.9, 0.9, 0.9), - 260.0, - 175.0, 5,
		"This is why you, (You), have to have to go to speak with (Dwarven king).");
	shr->print_bitmap (new D3 (0.9, 0.9, 0.9), - 260.0, - 190.0, 5,
		"You have to convince him to join us so we can defeat together the Orcs again.");
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
	tsl->get_tile (5, 3)->set_obstacle (new Obstacle ("tree"));

	glutMainLoop ();

	return 1;
}
