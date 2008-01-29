#include "engines/gui_engine.hpp"
#include "engines/input/command_reader.hpp"
#include "engines/log.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "messages/nothing.hpp"
#include "strategies/game_modes/menu.hpp"
#include "strategies/game_modes/play.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Game_Modes;

//	static
string Menu ::
	get_class_name ()
{
	return "Strategies :: Game_Modes :: Menu";
}

//  constructor
Menu ::
	Menu () :
	Object ("menu", true)	//	Here 'true' means 'prevent automatic destruction'.
{
	Engines :: Log :: trace (me, Menu :: get_class_name ());

	assert (is_initialized ());
}

//  destructor
Menu ::
	~Menu ()
{
	Engines :: Log :: trace (me, Menu :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Menu ::
	is_initialized ()
	const
{
	assert (Game_Mode :: is_initialized ());

	return true;
}

//	virtual
Reference <Game_Mode> Menu ::
	transit (Reference <Messages :: Nothing> message)
{
	assert (is_initialized ());
	assert (! message . points_to_object ());

	//	quit
	if (Engines :: Input :: Command_Reader :: get () -> has_command (me, "quit"))
	{
		return Reference <Game_Mode> ();
	}

	//	un-pause
	if (Engines :: Input :: Command_Reader :: get () -> has_command (me, "return"))
	{
		if (Items :: Character :: get_player_character () -> is_alive ())
		{
			Engines :: Log :: log (me) << "Game resumed" << endl;
			
			return Play :: get ();
		}
		else
		{
			Engines :: Log :: show ("You're dead and cannot return to the game.");
		}
	}

	Engines :: GUI_Engine :: get () -> set_active_gui (gui);

	return Reference <Game_Mode> (this);
}
