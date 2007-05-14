#include "menu_state.hpp"
#include "world.hpp"
#include "quit_state.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"

using namespace std;
using namespace TSL;

//	static
const string Menu_State ::
	class_name ("Menu_State");

//  constructor
Menu_State ::
	Menu_State () :
	Object ("menu state"),
	gui (GUI_Engine :: get () . create_gui ("menu.cfg"))
{
	log (debugging) << "Menu_State ()" << endl;
	assert (Algorithm <Game> :: is_initialized ());

	assert (Menu_State :: is_initialized ());
}

//  destructor
Menu_State ::
	~Menu_State ()
{
	log (debugging) << "~" << class_name << " ()" << endl;

	assert (Algorithm <Game> :: is_initialized ());
}

//	virtual
bool Menu_State ::
	is_initialized ()
	const
{
	return Algorithm <Game> :: is_initialized ();
}

//	virtual
Algorithm <Game> & Menu_State ::
	transit (Game & owner)
{
	assert (is_initialized ());

	//	un-pause
	if (Input_Engine :: get () . get_key ("Escape", true)
					|| Input_Engine :: get () . get_gui_button ("Return", true))
	{
		log () << "Game resumed" << endl;
		
		return World :: get ();
	}

	//	quit
	if (Input_Engine :: get () . get_gui_button ("Quit", true))
	{
		return Quit_State :: get ();
	}

	//	FPS
	if (Input_Engine :: get () . get_gui_button ("Statistics", true))
	{
		show (owner . get_FPS ());
	}
	
	return owner . get_active_state ();
}

//	virtual
void Menu_State ::
	enter (Game & owner)
{
	assert (is_initialized ());

	GUI_Engine :: get () . activate (gui);
	
	show ("Menu (game paused)");
}
