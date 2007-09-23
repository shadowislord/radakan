#include "game.hpp"
#include "gui.hpp"
#include "gui_engine.hpp"
#include "input_engine.hpp"
#include "log.hpp"
#include "menu_state.hpp"
#include "play_state.hpp"
#include "player_character.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

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
	gui (Engines :: GUI_Engine :: get () -> create_gui ("menu.xml"))
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

	prepare_for_destruction ();
}

//	virtual
bool Menu_State ::
	is_initialized ()
	const
{
	assert (Strategy :: is_initialized ());

	return true;
}

//	virtual
Reference <Strategy> Menu_State ::
	transit (Reference <const Object> message)
{
	assert (is_initialized ());

	//	quit
	if ((message == terminate)
		|| Engines :: Input_Engine :: get () -> get_gui_button ("Quit"))
	{
		return Reference <Strategy> ();
	}

	Engines :: GUI_Engine :: get () -> set_active_gui (gui);

	//	un-pause
	if (Engines :: Input_Engine :: get () -> get_key ("escape", true)
		|| Engines :: Input_Engine :: get () -> get_gui_button ("Return"))
	{
		if (Items :: Player_Character :: get () -> is_dead ())
		{
			Engines :: Log :: show ("You're dead and cannot return to the game.");
		}
		else
		{
			Engines :: Log :: log (me) << "Game resumed" << endl;
			
			return Play_State :: get ();
		}
	}

	//	FPS
	if (Engines :: Input_Engine :: get () -> get_gui_button ("Statistics"))
	{
		Engines :: Log :: show (World :: get () -> get_FPS ());
	}

	return Reference <Strategy> (this);
}
