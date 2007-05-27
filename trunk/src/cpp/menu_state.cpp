#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "log.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "quit_state.hpp"
#include "world.hpp"

using namespace std;
using namespace TSL;
using namespace TSL :: Algorithms;

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
	gui (Engines :: GUI_Engine :: get () . create_gui ("menu.cfg"))
{
	Engines :: Log :: trace <Menu_State> (me);

	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Menu_State ::
	~Menu_State ()
{
	Engines :: Log :: trace <Menu_State> (me, "~");
	assert (is_initialized ());

	//	Do nothing.
}

//	virtual
bool Menu_State ::
	is_initialized ()
	const
{
	assert (Algorithm <Engines :: Game> :: is_initialized ());

	return true;
}

//	virtual
Algorithm <Engines :: Game> & Menu_State ::
	transit (Engines :: Game & owner, const Object & message)
{
	assert (is_initialized ());

	//	quit
	if ((message == terminate)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Quit", true))
	{
		return Quit_State :: get ();
	}

	//	un-pause
	if (Engines :: Input_Engine :: get () . get_key ("Escape", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Return", true))
	{
		Engines :: Log :: log (me) << "Game resumed" << endl;
		
		return Play_State :: get ();
	}

	//	FPS
	if (Engines :: Input_Engine :: get () . get_gui_button ("Statistics", true))
	{
		Engines :: Log :: show (owner . get_FPS () + " " + to_string (1000 / World :: get () . get_last_turn_lenght ()));
	}
	
	//	Return me or a child state.
	return owner . get_active_state ();
}

//	virtual
void Menu_State ::
	enter (Engines :: Game & owner)
{
	assert (is_initialized ());

	Engines :: GUI_Engine :: get () . set_active_state (gui);
	
	Engines :: Log :: show ("Menu (game paused)");
}
