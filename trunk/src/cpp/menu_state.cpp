#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "log.hpp"
#include "menu_state.hpp"
#include "quit_state.hpp"
#include "world.hpp"

using namespace std;
using namespace TSL;

//	static
const string Menu_State ::
	get_class_name ()
{
	return "Menu_State";
}

//  constructor
Menu_State ::
	Menu_State () :
	Object ("menu state"),
	gui (GUI_Engine :: get () . create_gui ("menu.cfg"))
{
	Log :: trace <Menu_State> (me);
	assert (Algorithm <Game> :: is_initialized ());

	assert (Menu_State :: is_initialized ());
}

//  destructor
Menu_State ::
	~Menu_State ()
{
	Log :: trace <Menu_State> (me, "~");

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
		Log :: log (me) << "Game resumed" << endl;
		
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
		Log :: show (owner . get_FPS ());
	}
	
	return owner . get_active_state ();
}

//	virtual
void Menu_State ::
	enter (Game & owner)
{
	assert (is_initialized ());

	GUI_Engine :: get () . activate (gui);
	
	Log :: show ("Menu (game paused)");
}
