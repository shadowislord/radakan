#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>

#include <OgreSceneManager.h>

#include "engines/conversation_engine.hpp"
#include "engines/game.hpp"
#include "engines/gui_engine.hpp"
#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/npc.hpp"
#include "items/player_character.hpp"
#include "messages/conversation_message.hpp"
#include "movable_model.hpp"
#include "play_state_gui.hpp"
#include "strategies/menu_state.hpp"
#include "strategies/play_state.hpp"
#include "tile.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
string Play_State ::
	get_class_name ()
{
	return "Play_State";
}

//  constructor
Play_State ::
	Play_State (boost :: shared_ptr <Ogre :: SceneManager> scene_manager) :
	Object ("play state"),
	gui (Engines :: GUI_Engine :: get () -> create_gui <Play_State_GUI> ("play.xml")),
	camera (scene_manager -> createCamera ("camera"))
{
	Engines :: Log :: trace (me, Play_State :: get_class_name (), "", "~scene_manager~");

	camera -> setNearClipDistance (0.001);
	camera -> setFarClipDistance (150);

	Reference <Engines :: Conversation_Engine> conversation_engine
		(new Engines :: Conversation_Engine ());
	conversation_engine -> register_observer (gui);
	gui -> Observable <Messages :: Message <Items :: Character> > :: register_observer
		(Engines :: Input_Engine :: get ());

	assert (Play_State :: is_initialized ());
}

//  destructor
Play_State ::
	~Play_State ()
{
	Engines :: Log :: trace (me, Play_State :: get_class_name (), "~");
	assert (Play_State :: is_initialized ());

	prepare_for_destruction ();

	Engines :: Conversation_Engine :: destruct ();
}

//	virtual
bool Play_State ::
	is_initialized ()
	const
{
//	Engines :: Log :: trace (me, Play_State :: get_class_name (), "is_initialized");
	assert (Singleton <Play_State> :: is_initialized ());
	assert (Strategy <Engines :: Game> :: is_initialized ());

	return true;
}

//	virtual
Reference <Strategy <Engines :: Game> > Play_State ::
	transit (const Reference <Messages :: Message <Engines :: Game> > & message)
{
	assert (is_initialized ());

	//	quit
	if (message == Messages :: Message <Engines :: Game> :: terminate)
	{
		return Reference <Strategy <Engines :: Game> > ();
	}

	//	menu
	if (Items :: Player_Character :: get () -> is_dead ()
		|| Engines :: Input_Engine :: get () -> get_key ("escape", true)
		|| Engines :: Input_Engine :: get () -> get_gui_button ("Menu"))
	{
		return Menu_State :: get ();
	}

	const Reference <Messages :: Message <Items :: Character> > conversation_option
		= Engines :: Input_Engine :: get () -> get_conversation_option ();
	if (conversation_option . points_to_object ())
	{
		assert (conversation_option -> from == Items :: Player_Character :: get ());
		Items :: Player_Character :: get () -> call_observers (conversation_option);
	}

	World :: get () -> update ();

	Engines :: GUI_Engine :: get () -> set_active_gui (gui);

	float top_speed = 0;

	if (Engines :: Input_Engine :: get () -> get_key
		(Engines :: Settings :: get () -> forward_key, false))
	{
		top_speed = 1;
	}
	else if (Engines :: Input_Engine :: get () -> get_key
		(Engines :: Settings :: get () -> backward_key, false))
	{
		top_speed = - 0.7;
	}
	Items :: Player_Character :: get () -> get_movable_model () -> move (top_speed);

	float top_angular_speed = 0;
	if (Engines :: Input_Engine :: get () -> get_key
		(Engines :: Settings :: get () -> left_key, false))
	{
		top_angular_speed = 1;
	}
	else if (Engines :: Input_Engine :: get () -> get_key
		(Engines :: Settings :: get () -> right_key, false))
	{
		top_angular_speed = - 1;
	}

	Items :: Player_Character :: get () -> get_movable_model () -> turn (top_angular_speed);

	//	reset your orientation
	if (Engines :: Input_Engine :: get () -> get_key ("space", true)
		|| Engines :: Input_Engine :: get () -> get_gui_button ("Reset"))
	{
		Items :: Player_Character :: get () -> get_movable_model () -> reset ();
	}

	//	select a target
	if (Engines :: Input_Engine :: get () -> get_key ("t", true))
	{
		if (character_target . points_to_object () || item_target . points_to_object ())
		{
			item_target . reset_pointee ();
			character_target . reset_pointee ();
		}
		else
		{
			character_target = World :: get () -> get_active_state () -> npcs -> get_child ();
			Engines :: Log :: show ("Target: " + character_target . get_name ());
		}
	}

	if (character_target . points_to_object ())
	{
		Engines :: Conversation_Engine :: get () -> list_options
			(Items :: Player_Character :: get(), character_target);
	}

	//	hit
	if (Engines :: Input_Engine :: get () -> get_key ("h", true)
		|| Engines :: Input_Engine :: get () -> get_gui_button ("Hit"))
	{
		if (character_target . points_to_object ())
		{
			Items :: Player_Character :: get () -> hit ("agressive", character_target);

		}
		else
		{
			Engines :: Log :: show ("No target selected.");
		}
	}

	//	move the weapon
	if (Engines :: Input_Engine :: get () -> get_key ("m", true)
		|| Engines :: Input_Engine :: get () -> get_gui_button ("Move"))
	{
		if (character_target . points_to_object ())
		{
			Reference <Items :: Container_Item <Items :: Item> > player_hand
				= Items :: Player_Character :: get () -> right_hand;
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

	const Ogre :: Vector3 & mouse_position = Engines :: Input_Engine :: get ()
		-> get_mouse_position (Engines :: Input_Engine :: relative);

	if
	(
		Engines :: Input_Engine :: get () -> get_mouse_button
		(
			Engines :: Input_Engine :: middle_mouse_button,
			false
		)
	)
	{
		if (mouse_position . x != 0)
		{
			Items :: Player_Character :: get () -> get_movable_model () -> turn
				(- Ogre :: Math :: Sign (mouse_position . x));
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
		Items :: Player_Character :: get () -> get_movable_model () -> node -> getPosition ()
			+ Items :: Player_Character :: get () -> get_movable_model () -> get_top_direction ()
			* Engines :: Settings :: get () -> get_camera_distance ()
	);
	camera -> setOrientation
	(
		make_quaternion
		(
			Engines :: Settings :: get () -> get_vertical_camera_angle (),
			Items :: Player_Character :: get () -> get_movable_model () -> get_side_direction ()
		)
		* Items :: Player_Character :: get () -> get_movable_model () -> node -> getOrientation ()
	);

	return Reference <Strategy <Engines :: Game> > (this);
}
