#include "engines/gui_engine.hpp"
#include "engines/input/command_reader.hpp"
#include "engines/mediator.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/character.hpp"
#include "messages/nothing.hpp"
#include "strategies/game_modes/menu.hpp"
#include "strategies/game_modes/play.hpp"
#include "world.hpp"

#include <OgreRoot.h>
#include <OgreSceneManager.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Game_Modes;

//	static
string Play ::
	get_class_name ()
{
	return "Play";
}

//  constructor
Play ::
	Play () :
	Object ("play", "singleton")
{
	Engines :: Log :: trace (me, Play :: get_class_name ());

	assert (Play :: is_initialized ());
}

//  destructor
Play ::
	~Play ()
{
	Engines :: Log :: trace (me, Play :: get_class_name (), "~");
	assert (Play :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Play ::
	is_initialized ()
	const
{
//	Engines :: Log :: trace (me, Play :: get_class_name (), "is_initialized");
	assert (Singleton <Play> :: is_initialized ());

	//	'assert' can't handle double templates.
	//	assert (Strategy <Strategies :: Game_Modes :: Game_Mode, Object> :: is_initialized ());

	return true;
}

//	virtual
Reference <Game_Mode> Play ::
	transit (Reference <Messages :: Nothing>)
{
	assert (is_initialized ());
	assert (Items :: Character :: get_player_character () -> is_alive ());

	//	quit
	if (Engines :: Input :: Command_Reader :: get () -> has_command
		(Game_Mode :: get_class_name (), "quit"))
	{
		return Reference <Game_Mode> ();
	}

	//	menu
	if (Engines :: Input :: Command_Reader :: get () -> has_command
		(Game_Mode :: get_class_name (), "menu"))
	{
		return Menu :: get ();
	}

	World :: get () -> update ();

	//	Update all characters.
	Engines :: Mediator :: get () -> call_observers <Messages :: Nothing>
		(Messages :: Nothing :: get ());

	//	If the player character is dead, go to the menu.
	if (! Items :: Character :: get_player_character () -> is_alive ())
	{
		return Menu :: get ();
	}

	Engines :: GUI_Engine :: get () -> set_active_gui (gui);

	return Reference <Game_Mode> (this);
}
