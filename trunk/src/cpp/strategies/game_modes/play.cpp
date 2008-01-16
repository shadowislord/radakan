#include <OgreRoot.h>
#include <OgreSceneManager.h>

#include "engines/conversation_engine.hpp"
#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/characters/player_character.hpp"
#include "messages/message.hpp"
#include "movable_model.hpp"
#include "play_gui.hpp"
#include "set.hpp"
#include "strategies/game_modes/menu.hpp"
#include "strategies/game_modes/play.hpp"
#include "tile.hpp"
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
	Play
	(
		boost :: shared_ptr <Ogre :: RenderWindow> window,
		boost :: shared_ptr <Ogre :: SceneManager> scene_manager
	):
	Object ("play state", true),	//	Here 'true' means 'prevent automatic destruction'.
	camera (scene_manager -> createCamera ("camera"))
{
	Engines :: Log :: trace (me, Play :: get_class_name (), "", "~scene_manager~");

	camera -> setNearClipDistance (0.001);
	camera -> setFarClipDistance (150);

	Ogre :: Root :: getSingleton () . getRenderSystem () -> _setViewport
		(window -> addViewport (camera . get ()));

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

	//	quit
	if (message == Messages :: Message <Object> :: terminate)
	{
		return Reference <Game_Mode> ();
	}

	//	menu
	if (Items :: Characters :: Player_Character :: get () -> is_dead ()
		|| Engines :: Input_Engine :: get () -> has_command ("menu"))
	{
		return Menu :: get ();
	}

	const Reference <Messages :: Message <Items :: Characters :: Character> >
		conversation_option
			= Engines :: Input_Engine :: get () -> get_conversation_option ();
	if (conversation_option . points_to_object ())
	{
		assert (conversation_option -> from == Items :: Characters :: Player_Character :: get ());
		Items :: Characters :: Player_Character :: get () -> call_observers (conversation_option);
	}

	World :: get () -> update ();

	Engines :: GUI_Engine :: get () -> set_active_gui (gui);

	float relative_destination_movement_speed = 0;
	if (Engines :: Input_Engine :: get () -> has_command ("go forward", false))
	{
		relative_destination_movement_speed = 1;
	}
	else if (Engines :: Input_Engine :: get () -> has_command ("go backward", false))
	{
		relative_destination_movement_speed = - 0.7;
		Engines :: Log :: log (me) << "Going backwards: - 0.7" << endl;
	}
	Items :: Characters :: Player_Character :: get () -> get_movable_model () -> move
		(relative_destination_movement_speed);

	float relative_destination_turn_speed = 0;
	if (Engines :: Input_Engine :: get () -> has_command ("turn left", false))
	{
		relative_destination_turn_speed = 1;
	}
	else if (Engines :: Input_Engine :: get () -> has_command ("turn right", false))
	{
		relative_destination_turn_speed = - 1;
	}
	Items :: Characters :: Player_Character :: get () -> get_movable_model () -> turn
		(relative_destination_turn_speed);

	//	select a target
	if (Engines :: Input_Engine :: get () -> has_command ("select target"))
	{
		if (character_target . points_to_object () || item_target . points_to_object ())
		{
			item_target . reset_pointee ();
			character_target . reset_pointee ();
			Engines :: Log :: show
				("Target deselected.");
		}
		else
		{
			character_target
				= World :: get () -> get_active_state () -> characters -> get_child ();
			Engines :: Log :: show
				("Target selected: " + character_target . get_name (true));
		}
	}

	Engines :: Conversation_Engine :: get () -> list_options (character_target);

	//	hit
	if (Engines :: Input_Engine :: get () -> has_command ("hit"))
	{
		if (character_target . points_to_object ())
		{
			Items :: Characters :: Player_Character :: get () -> hit
				("aggressive", character_target);
		}
		else
		{
			Engines :: Log :: show ("No target selected.");
		}
	}

	//	Give the weapon to your target or take it back.
	if (Engines :: Input_Engine :: get () -> has_command ("give"))
	{
		if (character_target . points_to_object ())
		{
			Reference <Items :: Container_Item <Items :: Item> > player_hand
				= Items :: Characters :: Player_Character :: get () -> right_hand;
			Reference <Items :: Container_Item <Items :: Item> > npc_hand
				= character_target -> right_hand;

			if (! player_hand -> is_empty ())
			{
				player_hand -> move (player_hand -> get_child (), npc_hand);
				assert (player_hand -> is_empty ());
				assert (! npc_hand -> is_empty ());
				Engines :: Log :: show ("You gave your weapon to the ninja.");
			}
			else if (! npc_hand -> is_empty ())
			{
				npc_hand -> move (npc_hand -> get_child (), player_hand);
				assert (npc_hand -> is_empty ());
				assert (! player_hand -> is_empty ());
				Engines :: Log :: show ("You took your weapon from the ninja.");
			}
			else
			{
				Engines :: Log :: show ("Both you and the ninja don't have a weapon.");
			}
		}
		else
		{
			Engines :: Log :: show ("No target selected.");
		}
	}

	//	jump
	if (Engines :: Input_Engine :: get () -> has_command ("jump"))
	{
		Engines :: Log :: show ("Sadly, jumping is not supported yet.");
	}

	//	reset your orientation
	if (Engines :: Input_Engine :: get () -> has_command ("reset"))
	{
		Items :: Characters :: Player_Character :: get ()
			-> get_movable_model () -> reset ();
		Engines :: Log :: show ("Your orientation is reset.");
	}

	const Ogre :: Vector3 & mouse_position = Engines :: Input_Engine :: get ()
		-> get_mouse_position (Engines :: Input_Engine :: relative);

	if
	(
		Engines :: Input_Engine :: get () -> is_mouse_button_pressed
		(
			Engines :: Input_Engine :: middle_mouse_button,
			false
		)
	)
	{
		if (mouse_position . x != 0)
		{
			Items :: Characters :: Player_Character :: get () -> get_movable_model ()
				-> turn (- Ogre :: Math :: Sign (mouse_position . x));
		}

		if (mouse_position . y != 0)
		{
			Engines :: Settings :: get () -> increase_vertical_camera_angle
				(- World :: get () -> get_last_turn_length () * mouse_position . y / 10);
		}
	}

	if (mouse_position . z != 0)
	{
		Engines :: Settings :: get () -> increase_camera_distance (- 0.0005 * mouse_position . z);
	}

	camera -> setPosition
	(
		Items :: Characters :: Player_Character :: get ()
			-> get_movable_model () -> node -> getPosition ()
			+ Items :: Characters :: Player_Character :: get ()
				-> get_movable_model () -> get_top_direction ()
			* Engines :: Settings :: get () -> get_camera_distance ()
	);
	camera -> setOrientation
	(
		make_quaternion
		(
			Engines :: Settings :: get () -> get_vertical_camera_angle (),
			Items :: Characters :: Player_Character :: get ()
				-> get_movable_model () -> get_side_direction ()
		)
		* Items :: Characters :: Player_Character :: get ()
			-> get_movable_model () -> node -> getOrientation ()
	);

	return Reference <Game_Mode> (this);
}
