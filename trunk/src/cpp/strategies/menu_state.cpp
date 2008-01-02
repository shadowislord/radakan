#include "engines/game.hpp"
#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "gui.hpp"
#include "items/player_character.hpp"
#include "messages/message.hpp"
#include "strategies/menu_state.hpp"
#include "strategies/play_state.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
string Menu_State ::
	get_class_name ()
{
	return "Menu_State";
}

//  constructor
Menu_State ::
	Menu_State () :
	Object ("menu state", true),	//	Here 'true' means 'prevent automatic destruction'.
	gui (Engines :: GUI_Engine :: get () -> create_gui <GUI> ("menu.xml"))
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
	assert (Strategy <Engines :: Game> :: is_initialized ());

	return true;
}

//	virtual
Reference <Strategy <Engines :: Game> > Menu_State ::
	transit (const Reference <Messages :: Message <Engines :: Game> > & message)
{
	assert (is_initialized ());

	//	quit
	if ((message == Messages :: Message <Engines :: Game> :: terminate)
		|| Engines :: Input_Engine :: get () -> get_gui_button ("Quit"))
	{
		return Reference <Strategy <Engines :: Game> > ();
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

	return Reference <Strategy <Engines :: Game> > (this);
}
