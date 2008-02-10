#include "engines/log.hpp"
#include "engines/gui_engine.hpp"
#include "engines/settings.hpp"
#include "gui.hpp"
#include "messages/nothing.hpp"
#include "strategies/game_modes/game_mode.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Game_Modes;

//	static
string Game_Mode ::
	get_class_name ()
{
	return "Strategies :: Game_Modes :: Game_Mode";
}

//  constructor
Game_Mode ::
	Game_Mode () :
	Object ("The name doesn't matter. This is an abstract base class."),
	gui (Engines :: GUI_Engine :: get () -> create_gui (me . get_name (true) + ".xml"))
{
	Engines :: Settings :: get () -> load_key_bindings (me . get_name (true));

	assert (is_initialized ());
}

//  destructor
Game_Mode ::
	~Game_Mode ()
{
	Engines :: Log :: trace (me, Game_Mode :: get_class_name (), "~");
	assert (is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Game_Mode ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Game_Mode, Object> :: is_initialized ());
	
	return true;
}
