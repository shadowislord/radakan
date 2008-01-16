#include "engines/input_engine.hpp"
#include "engines/log.hpp"
#include "engines/settings.hpp"
#include "items/character.hpp"
#include "items/container_item.hpp"
#include "items/weapon.hpp"
#include "messages/message.hpp"
#include "movable_model.hpp"
#include "set.hpp"
#include "strategies/behaviors/player.hpp"
#include "tile.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Behaviors;

//	static
string Player ::
	get_class_name ()
{
	return "Player";
}

//  constructor
Player ::
	Player (Reference <Items :: Character> new_character) :
	Object (new_character . get_name () + "'s player"),
	Behavior (new_character)
{
	//	Do nothing.

	bool check = character -> back -> add (Reference <Items :: Container_Item <Items :: Item> > (new Items :: Container_Item <Items :: Item> ("Backpack", 3, Ogre :: Vector3 (0.3, 0.5, 0.2), Reference <Mesh_Data> ())));
	assert (check);

	check = character -> right_hand -> add (Reference <Items :: Item> (new Items :: Weapon ("Sword", 4, Ogre :: Vector3 (0.1, 0.2, 0.3), Reference <Mesh_Data> (), 5, 6, 7, 8, 9, 10)));
	assert (check);

	assert (Player :: is_initialized ());
}

//  destructor
Player ::
	~Player ()
{
	Engines :: Log :: trace (me, Player :: get_class_name (), "~");
	assert (Player :: is_initialized ());

	prepare_for_destruction ();
}

//	virtual
bool Player ::
	is_initialized ()
	const
{
	//	'assert' can't handle double templates.
	//	assert (Strategy <Behavior, Items :: Character> :: is_initialized ());

	return true;
}

//	virtual
Reference <Behavior> Player ::
	transit
		(const Reference <Messages :: Message <Items :: Character> > & message)
{
	assert (is_initialized ());

	if (message == Messages :: Message <Items :: Character> :: terminate)
	{
		return Reference <Behavior> ();
	}
	else if (message == Messages :: Message <Items :: Character> :: update)
	{
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
		character -> get_movable_model () -> move
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
		character -> get_movable_model () -> turn
			(relative_destination_turn_speed);

		//	Select a target.
		if (Engines :: Input_Engine :: get () -> has_command ("select target"))
		{
			if (character -> character_target . points_to_object ()
				|| character -> item_target . points_to_object ())
			{
				character -> item_target . reset_pointee ();
				character -> character_target . reset_pointee ();
				Engines :: Log :: show ("Target deselected.");
			}
			else
			{
				character -> character_target
					= World :: get () -> get_active_state () -> characters -> get_child ();
				if (character -> character_target == character)
				{
					character -> character_target
						= World :: get () -> get_active_state () -> characters
							-> get_another_child ();
				}
				Engines :: Log :: show
					("Target selected: " + character -> character_target . get_name (true));
			}
		}

		//	Hit your target.
		if (Engines :: Input_Engine :: get () -> has_command ("hit"))
		{
			if (character -> character_target . points_to_object ())
			{
				character -> hit ("aggressive", character -> character_target);
			}
			else
			{
				Engines :: Log :: show ("No target selected.");
			}
		}

		//	Give the weapon to your target or take it back.
		if (Engines :: Input_Engine :: get () -> has_command ("give"))
		{
			if (character -> character_target . points_to_object ())
			{
				Reference <Items :: Container_Item <Items :: Item> > player_hand
					= character -> right_hand;
				Reference <Items :: Container_Item <Items :: Item> > npc_hand
					= character -> character_target -> right_hand;

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
			character -> get_movable_model () -> reset ();
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
				character -> get_movable_model ()
					-> turn (- Ogre :: Math :: Sign (mouse_position . x));
			}

			if (mouse_position . y != 0)
			{
				Engines :: Settings :: get () -> increase_vertical_camera_angle
					(- World :: get () -> get_last_turn_length () * mouse_position . y
						/ 10);
			}
		}

		if (mouse_position . z != 0)
		{
			Engines :: Settings :: get () -> increase_camera_distance
				(- 0.0005 * mouse_position . z);
		}
	}
	else
	{
		//	The NPCs message are showed in the log, to let the player know.
		Engines :: Log :: show
			(message -> from . get_name (true) + ": " + message . get_name (true));

		if (message . get_name (true) == "\"Bye.\"")
		{
			//	Deselect the NPC that was conversating with you.
			character -> character_target . reset_pointee ();
		}
	}
	
	return Reference <Behavior> (this);
}
