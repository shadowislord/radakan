#include "alive_state.hpp"
#include "chat_state.hpp"
#include "conversation_engine.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "settings.hpp"
#include "play_state.hpp"
#include "player_character.hpp"

#include <CEGUIExceptions.h>
#include <elements/CEGUIListbox.h>
#include <elements/CEGUIListboxTextItem.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;

//	static
const string Play_State ::
	get_class_name ()
{
	return "Play_State";
}

//  constructor
Play_State ::
	Play_State (Ogre :: SceneManager & scene_manager) :
	Object ("play state"),
	gui (Engines :: GUI_Engine :: get () . create_gui ("play.xml")),
	camera (* scene_manager . createCamera ("camera"))
{
	Engines :: Log :: trace (me, Play_State :: get_class_name (), "", "~scene_manager~");

	camera . setNearClipDistance (0.001);
	camera . setFarClipDistance (150);

	new Engines :: Conversation_Engine ();

	assert (Play_State :: is_initialized ());
}

//  destructor
Play_State ::
	~Play_State ()
{
	Engines :: Log :: trace (me, Play_State :: get_class_name (), "~");
	assert (Play_State :: is_initialized ());

	prepare_for_destruction ();

	delete & Engines :: Conversation_Engine :: get ();
}

//	virtual
bool Play_State ::
	is_initialized ()
	const
{
//	Engines :: Log :: trace (me, Play_State :: get_class_name (), "is_initialized");
	assert (Singleton <Play_State> :: is_initialized ());
	assert (Strategy :: is_initialized ());

	return true;
}

//	virtual
Strategy * Play_State ::
	transit (const Object & message)
{
	assert (is_initialized ());

	//	quit
	if (message == terminate)
	{
		return NULL;
	}

	//	menu
	if (Items :: Player_Character :: get () . is_dead ()
		|| Engines :: Input_Engine :: get () . get_key ("escape", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Menu"))
	{
		return & Menu_State :: get ();
	}

	Messages :: Conversation_Message * conversation_option = Engines :: Input_Engine :: get () . get_conversation_option ();
	if (conversation_option != NULL)
	{
		conversation_option -> from . call_observers (* conversation_option);
	}

	World :: get () . update ();

	Engines :: GUI_Engine :: get () . set_active_gui (gui);

	float top_speed = 0;

	if (Engines :: Input_Engine :: get () . get_key
		(Engines :: Settings :: get () . forward_key, false))
	{
		top_speed = 1;
	}
	else if (Engines :: Input_Engine :: get () . get_key
		(Engines :: Settings :: get () . backward_key, false))
	{
		top_speed = - 0.7;
	}
	Items :: Player_Character :: get () . get_movable_model () . move (top_speed);

	float top_angular_speed = 0;
	if (Engines :: Input_Engine :: get () . get_key
		(Engines :: Settings :: get () . left_key, false))
	{
		top_angular_speed = 1;
	}
	else if (Engines :: Input_Engine :: get () . get_key
		(Engines :: Settings :: get () . right_key, false))
	{
		top_angular_speed = - 1;
	}

	Items :: Player_Character :: get () . get_movable_model () . turn (top_angular_speed);

	//	reset your orientation
	if (Engines :: Input_Engine :: get () . get_key ("space", false)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Reset"))
	{
		Items :: Player_Character :: get () . get_movable_model () . reset ();
	}

	Items :: NPC & closest_npc = * World :: get () . get_active_state () . npcs . get_child ();

	Set <Messages :: Conversation_Message> & conversation_messages = Engines :: Conversation_Engine :: get () . get_options (Items :: Player_Character :: get(), closest_npc);
	gui . call (conversation_messages);

	//	hit
	if (Engines :: Input_Engine :: get () . get_key ("h", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Hit"))
	{
		Items :: Player_Character :: get () . hit ("agressive", closest_npc);
	}

	//	move the weapon
	if (Engines :: Input_Engine :: get () . get_key ("m", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Move"))
	{
		Items :: Multislot <Items :: Item> & player_hands
			= Items :: Player_Character :: get () . hands;
		Items :: Multislot <Items :: Item> & npc_hands
			= closest_npc . hands;

		if (! player_hands . is_empty ())
		{
			player_hands . move (* player_hands . get_child (), npc_hands);
			assert (player_hands . is_empty ());
			assert (! npc_hands . is_empty ());
			Engines :: Log :: show ("You gave your weapon to the ninja.");
		}
		else if (! npc_hands . is_empty ())
		{
			npc_hands . move (* npc_hands . get_child (), player_hands);
			assert (npc_hands . is_empty ());
			assert (! player_hands . is_empty ());
			Engines :: Log :: show ("You took your weapon from the ninja.");
		}
		else
		{
			Engines :: Log :: show ("Both you and the ninja don't have a weapon.");
		}
	}

	const Ogre :: Vector3 & mouse_position = Engines :: Input_Engine :: get ()
		. get_mouse_position (Engines :: Input_Engine :: relative);

	if (Engines :: Input_Engine :: get () . get_mouse_button
		(Engines :: Input_Engine :: middle_mouse_button, false))
	{
		if (mouse_position . x != 0)
		{
			Items :: Player_Character :: get () . get_movable_model () . turn
				(- Ogre :: Math :: Sign (mouse_position . x));
		}

		if (mouse_position . y != 0)
		{
			Engines :: Settings :: get () . increase_vertical_camera_angle
				(- World :: get () . get_last_turn_lenght () * mouse_position . y / 10);
		}
	}

	if (mouse_position . z != 0)
	{
		Engines :: Settings :: get () . increase_camera_distance (- 0.0005 * mouse_position . z);
	}

	camera . setPosition
	(
		Items :: Player_Character :: get () . get_movable_model () . node . getPosition ()
			+ Items :: Player_Character :: get () . get_movable_model () . get_top_direction ()
			* Engines :: Settings :: get () . get_camera_distance ()
	);
	camera . setOrientation
	(
		make_quaternion
		(
			Engines :: Settings :: get () . get_vertical_camera_angle (),
			Items :: Player_Character :: get () . get_movable_model () . get_side_direction ()
		)
		* Items :: Player_Character :: get () . get_movable_model () . node . getOrientation ()
	);

	return this;
}
