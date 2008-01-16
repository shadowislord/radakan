#include <OgreRoot.h>
#include <OgreSceneManager.h>

#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/character.hpp"
#include "messages/message.hpp"
#include "play_gui.hpp"
#include "strategies/game_modes/menu.hpp"
#include "strategies/game_modes/play.hpp"
#include "world.hpp"

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
	Object ("play state", true)	//	Here 'true' means 'prevent automatic destruction'.
{
	Engines :: Log :: trace (me, Play :: get_class_name ());

	gui = Engines :: GUI_Engine :: get () -> create_gui <Play_GUI> ("play.xml");

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
	transit (const Reference <Messages :: Message <Object> > & message)
{
	assert (is_initialized ());
	assert (Items :: Character :: get_player_character () -> is_alive ());

	//	quit
	if (message == Messages :: Message <Object> :: terminate)
	{
		return Reference <Game_Mode> ();
	}

	assert (message == Messages :: Message <Object> :: update);

	const Reference <Messages :: Message <Items :: Character> >
		conversation_option
			= Engines :: Input_Engine :: get () -> get_conversation_option ();
	if (conversation_option . points_to_object ())
	{
		assert (conversation_option -> from == Items :: Character :: get_player_character ());
		Items :: Character :: get_player_character () -> call_observers (conversation_option);
	}

	World :: get () -> update ();

	//	menu
	if (Engines :: Input_Engine :: get () -> has_command ("menu")
		|| ! Items :: Character :: get_player_character () -> is_alive ())
	{
		return Menu :: get ();
	}

	Engines :: GUI_Engine :: get () -> set_active_gui (gui);

	return Reference <Game_Mode> (this);
}
