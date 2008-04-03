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
	handle_keyboard_input
	(GLubyte key, int x, int y)
{
	string a (1, key);
	event_engine->process (new Input_Event (a, x, y));
}

void
	handle_mouse_input
	(int button, int state, int x, int y)
{
	event_engine->process (new Input_Event (button, state, x, y));
}

void
	draw
	()
{
	shr->draw_start ();
	tsl->draw (shr);
	shr->render_quad (
		D3 (0.4, 0.4, 0.4),
		D3 (- 270, - 80, 0),
		D3 ( 270, - 80, 0),
		D3 ( 270, - 205, 0),
		D3 (- 270, - 205, 0)
	);
	shr->print_bitmap (D3 (0.9, 0.9, 0.9), - 260, - 100, 5,
		"The world has been at peace for decades.");
	shr->print_bitmap (D3 (0.9, 0.9, 0.9), - 260, - 115, 5,
		"Elves, Dwarves and Humans live in harmony, thanks to (war), the war that defeated the Orcs.");
	shr->print_bitmap (D3 (0.9, 0.9, 0.9), - 260, - 130, 5,
		"No trace of Orcs had been seen since then.");
	shr->print_bitmap (D3 (0.9, 0.9, 0.9), - 260, - 145, 5,
		"But somehow the resting Orcs gathered in the caves beneath the northern mountains.");
	shr->print_bitmap (D3 (0.9, 0.9, 0.9), - 260, - 160, 5,
		"As you know, the Orcs attacked yesterday (Elven city) and captured it.");
	shr->print_bitmap (D3 (0.9, 0.9, 0.9), - 260, - 175, 5,
		"This is why you, (You), have to have to go to speak with (Dwarven king).");
	shr->print_bitmap (D3 (0.9, 0.9, 0.9), - 260, - 190, 5,
		"You have to convince him to join us so we can defeat together the Orcs again.");
	shr->draw_stop ();
}

int
	main
	(int argc, char * * argv)
{
	shr = new SHR (argc, argv);

	event_engine = new Event_Engine (quit);

	glutDisplayFunc (draw);
	glutReshapeFunc (resize);
	glutKeyboardFunc (handle_keyboard_input);
	glutMouseFunc(handle_mouse_input);
	
	tsl = new World ("world", D3 (), D3 (500, 350, 0));
	tsl->add_obstacle (new Character ("hero", D3 (52, 27, 0), D3 (25, 25, 25)));

	glutMainLoop ();

	return 1;
}
