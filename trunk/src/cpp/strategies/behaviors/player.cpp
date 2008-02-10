#include "engines/input/command_reader.hpp"
#include "engines/input/mouse_reader.hpp"
#include "engines/log.hpp"
#include "engines/mediator.hpp"
#include "engines/render_engine.hpp"
#include "engines/settings.hpp"
#include "items/character.hpp"
#include "items/container_item.hpp"
#include "items/weapon.hpp"
#include "messages/communications/converse.hpp"
#include "messages/communications/fight.hpp"
#include "movable_model.hpp"
#include "set.hpp"
#include "strategies/behaviors/ai.hpp"
#include "strategies/behaviors/player.hpp"
#include "tile.hpp"
#include "world.hpp"

#include <tinyxml.h>

#include <OgreSceneNode.h>

using namespace std;
using namespace Radakan;
using namespace Radakan :: Strategies;
using namespace Radakan :: Strategies :: Behaviors;

//	static
string Player ::
	get_class_name ()
{
	return "Strategies :: Behaviors :: Player";
}

//  constructor
Player ::
	Player (Reference <Items :: Character> new_character) :
	Object (new_character . get_name () + "'s player"),
	Behavior (new_character),
	camera_distance (0.72),
	vertical_camera_angle (0, - Ogre :: Math :: HALF_PI, Ogre :: Math :: HALF_PI)
{
	//	Do nothing.

	bool check = character -> back -> add (Reference <Items :: Container_Item <Items :: Item> > (new Items :: Container_Item <Items :: Item> ("Backpack", 3, Mathematics :: Vector_3D (0.3, 0.5, 0.2), Reference <Mesh_Data> ())));
	assert (check);

	check = character -> right_hand -> add (Reference <Items :: Item> (new Items :: Weapon ("Sword", 4, Mathematics :: Vector_3D (0.1, 0.2, 0.3), Reference <Mesh_Data> (), 5, 6, 7, 8, 9, 10)));
	assert (check);

	Engines :: Settings :: get () -> load_key_bindings ("play");
	
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
	//	assert (Strategy <Behavior, Messages :: Nothing> :: is_initialized ());

	return true;
}

//	virtual
void Player ::
	call (Reference <Messages :: Communications :: Communication> message)
{
	//	The NPCs message are showed in the log, to let the player know.
	Engines :: Log :: show
		(message -> from . get_name (true) + ": " + message . get_name (true));

	if (message . get_name (true) == "\"Bye.\"")
	{
		//	Deselect the NPC that was conversating with you.
		character_target . reset_pointee ();
	}
}

//	virtual
Reference <Behavior> Player ::
	transit (Reference <Messages :: Nothing>)
{
	assert (is_initialized ());

	float relative_destination_movement_speed = 0;
	if (Engines :: Input :: Command_Reader :: get ()
		-> has_command (get_class_name (), "go forward", false))
	{
		relative_destination_movement_speed = 1;
		Engines :: Log :: log (me) << "Going forward" << endl;
	}
	else if (Engines :: Input :: Command_Reader :: get ()
		-> has_command (get_class_name (), "go backward", false))
	{
		relative_destination_movement_speed = - 0.7;
		Engines :: Log :: log (me) << "Going backwards: - 0.7" << endl;
	}
	character -> get_movable_model () -> move
		(relative_destination_movement_speed);

	float relative_destination_turn_speed = 0;
	if (Engines :: Input :: Command_Reader :: get ()
		-> has_command (get_class_name (), "turn left", false))
	{
		relative_destination_turn_speed = 1;
		Engines :: Log :: log (me) << "Turn left" << endl;
	}
	else if (Engines :: Input :: Command_Reader :: get ()
		-> has_command (get_class_name (), "turn right", false))
	{
		relative_destination_turn_speed = - 1;
		Engines :: Log :: log (me) << "Turn right" << endl;
	}
	character -> get_movable_model () -> turn (relative_destination_turn_speed);

	//	Select a target.
	if (Engines :: Input :: Command_Reader :: get ()
		-> has_command (get_class_name (), "select target"))
	{
		if (character_target . points_to_object ()
			|| item_target . points_to_object ())
		{
			character_target . reset_pointee ();
			item_target . reset_pointee ();
			Engines :: Log :: show ("Target deselected.");
		}
		else
		{
			/*	TODO Fix this.
			character_target
				= World :: get () -> get_close_models () -> get_child () -> ;*/

			Engines :: Log :: show
				("Target selected: " + character_target . get_name (true));
		}
	}

	//	Give the weapon to your target or take it back.
	if (Engines :: Input :: Command_Reader :: get () -> has_command
		(get_class_name (), "give"))
	{
		if (character_target . points_to_object ())
		{
			Reference <Items :: Container_Item <Items :: Item> > player_hand
				= character -> right_hand;
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
	if (Engines :: Input :: Command_Reader :: get () -> has_command
		(get_class_name (), "jump"))
	{
		Engines :: Log :: show ("Sadly, jumping is not supported yet.");
	}

	//	reset your orientation
	if (Engines :: Input :: Command_Reader :: get () -> has_command
		(get_class_name (), "reset"))
	{
		character -> get_movable_model () -> reset ();
		Engines :: Log :: show ("Your orientation is reset.");
	}

	const Mathematics :: Vector_3D & mouse_position = Engines :: Input :: Mouse_Reader :: get ()
		-> get_relative_mouse_position ();

	if
	(
		Engines :: Input :: Mouse_Reader :: get () -> is_mouse_button_pressed
		(
			Engines :: Input :: Mouse_Reader :: get_middle_mouse_button (),
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
			vertical_camera_angle -= mouse_position . y
				/ (10 * Engines :: Render_Engine :: get () -> get_FPS ());
		}
	}

	if (mouse_position . z != 0)
	{
		camera_distance += - 0.0005 * mouse_position . z;
	}
	
	return Reference <Behavior> (this);
}

bool Player ::
	evaluate_condition (const TiXmlElement * element)
{
	assert (is_initialized ());
	assert (element != NULL);
	
	bool result = true;

	for (const TiXmlAttribute * attribute = element -> FirstAttribute (); attribute != NULL;
		attribute = attribute -> Next ())
	{
		result = result && evaluate_expression (attribute);
	}

	return result;
}

bool Player ::
	evaluate_expression (const TiXmlAttribute * attribute)
{
	assert (is_initialized ());
	assert (attribute != NULL);

	const string & name = attribute -> NameTStr ();
	const string & value = attribute -> ValueStr ();
	Engines :: Log :: log (me) << "evaluate: " << name  << " ? " << value << endl;
	
	if ((name == "knows") || (name == "has"))
	{
		Engines :: Log :: log (me)
			<< "result: false (I don't know and I don't have anything.)" << endl;
		
		return false;
	}

	assert ((value . at (0) == '<') || (value . at (0) == '>'));
	
	bool larger = (value . at (0) == '>');
	string temp = value;
	temp . erase (temp . begin ());
	float numeric_value = to_float (temp);

	bool is_smaller = (numeric_value < character -> get_skill (name));

	Engines :: Log :: log (me) << "result: " << to_string (is_smaller != larger) << endl;
	return (is_smaller != larger);
}

const Mathematics :: Vector_3D Player ::
	get_camera_position ()
	const
{
	assert (is_initialized ());

	return camera_distance * character -> get_movable_model () -> get_top_direction ()
		+ character -> get_movable_model () -> get_position ();
}

const Mathematics :: Quaternion Player ::
	get_camera_orientation ()
	const
{
	assert (is_initialized ());

	return Mathematics :: Quaternion
		(vertical_camera_angle . get_value (),
			character -> get_movable_model () -> get_side_direction ())
		* character -> get_movable_model () -> get_orientation ();
}

void Player ::
	list_communication_options ()
{
	assert (is_initialized ());

	//	Note: no target gives no options.
	if (character_target . points_to_object () && character_target -> is_alive ())
	{
		if (character_target != character)
		{
			boost :: shared_ptr <set <TiXmlDocument> > behavior_files
				= character_target -> get_active_state ()
					. cast <Strategies :: Behaviors :: AI> () -> get_behavior_files ();

			for (set <TiXmlDocument> :: iterator file = behavior_files -> begin ();
				file != behavior_files -> end (); file ++)
			{
				load_communication_options (file -> RootElement ());
			}
		}
	}
}

void Player ::
	load_communication_options (const TiXmlElement * element)
{
	assert (element != NULL);
	
	if (element -> ValueStr () == "option")
	{
		Engines :: Mediator :: get () ->
			call_observers <Messages :: Communications :: Communication>
				(Reference <Messages :: Communications :: Communication>
					(new Messages :: Communications :: Converse
						(character, character_target, element)));
	}
	
	if ((element -> ValueStr () == "reactions"))
	{
		return;
	}
	
	if ((element -> ValueStr () == "if_npc")
		&& ! character_target -> get_active_state () -> evaluate_condition (element))
	{
		return;
	}
	
	if ((element -> ValueStr () == "if_player") && ! evaluate_condition (element))
	{
		return;
	}
	
	for (const TiXmlElement * sub_element = element -> FirstChildElement ();
		sub_element != NULL; sub_element = sub_element -> NextSiblingElement ())
	{
		load_communication_options (sub_element);
	}
}
