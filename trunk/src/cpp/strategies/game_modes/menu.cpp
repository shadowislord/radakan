#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "gui.hpp"
#include "items/character.hpp"
#include "messages/message.hpp"
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
	return "Menu";
}

//  constructor
Menu ::
	Menu () :
	Object ("menu", true)	//	Here 'true' means 'prevent automatic destruction'.
{
	Engines :: Log :: trace (me, Menu :: get_class_name ());

	gui = Engines :: GUI_Engine :: get () -> create_gui <GUI> ("menu.xml");

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
	transit (const Reference <Messages :: Message <Object> > & message)
{
	assert (is_initialized ());

	//	quit
	if ((message == Messages :: Message <Object> :: terminate)
		|| Engines :: Input_Engine :: get () -> has_command ("quit"))
	{
		return Reference <Game_Mode> ();
	}

	Engines :: GUI_Engine :: get () -> set_active_gui (gui);

	//	un-pause
	if (Engines :: Input_Engine :: get () -> has_command ("return"))
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

	//	FPS
	if (Engines :: Input_Engine :: get () -> has_command ("statistics"))
	{
		Engines :: Log :: show (World :: get () -> get_FPS ());
	}

	return Reference <Game_Mode> (this);
}
