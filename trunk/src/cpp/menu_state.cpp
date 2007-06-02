#include "game.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "log.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "player_character.hpp"
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
	gui (Engines :: GUI_Engine :: get () . create_gui ("menu.xml"))
{
	Engines :: Log :: trace (me, Menu_State :: get_class_name ());

	//	Do nothing.

	assert (is_initialized ());
}

//  destructor
Menu_State ::
	~Menu_State ()
{
	Engines :: Log :: trace (me, Menu_State :: get_class_name (), "~");
	assert (is_initialized ());

	forget_dependencies ();
}

//	virtual
bool Menu_State ::
	is_initialized ()
	const
{
	assert (Algorithm :: is_initialized ());

	return true;
}

//	virtual
void Menu_State ::
	transit (const Object & message)
{
	assert (is_initialized ());

	//	quit
	if ((message == terminate)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Quit", true))
	{
		Menu_State :: destruct ();
		Play_State :: destruct ();

		return;
	}

	Engines :: GUI_Engine :: get () . set_active_state (gui);

	//	un-pause
	if (Engines :: Input_Engine :: get () . get_key ("Escape", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Return", true))
	{
		if (Items :: Player_Character :: get () . is_dead ())
		{
			Engines :: Log :: show ("You're dead and cannot return to the game.");
		}
		else
		{
			Engines :: Game :: get () . set_active_state (Play_State :: get ());
			
			Engines :: Log :: log (me) << "Game resumed" << endl;
		}
	}

	//	FPS
	if (Engines :: Input_Engine :: get () . get_gui_button ("Statistics", true))
	{
		Engines :: Log :: show
		(
			Engines :: Game :: get () . get_FPS ()
			+ " " + to_string (1000 * World :: get () . get_last_turn_lenght ())
		);
	}
}
