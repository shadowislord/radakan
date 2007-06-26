#include "alive_state.hpp"
#include "chat_state.hpp"
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
using namespace TSL;
using namespace TSL :: Algorithms;

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
	camera . setFarClipDistance (80);

	CEGUI :: Window * temp = NULL;
	try
	{
		temp = gui . root_window . getChild ("play-chat");
		assert (temp != NULL);
	}
	catch (CEGUI :: UnknownObjectException & exception)
	{
		Engines :: Log :: error (me) << "No log window found." << endl;
		abort ();
	}

	chat_window = dynamic_cast <CEGUI :: Listbox *> (temp);
	assert (chat_window != NULL);

	TiXmlDocument behavior (Engines :: Settings :: get () . tsl_path + "/data/behavior.xml");
	bool check = behavior . LoadFile ();
	if ((! check) || behavior . Error ())
	{
		Engines :: Log :: error (me) << behavior . ErrorDesc () << endl;
		abort ();
	}

	TiXmlElement * root = behavior . RootElement ();
	assert (root != NULL);
	TiXmlElement * peace_state = root -> FirstChildElement ();
	assert (peace_state != NULL);
	TiXmlElement * options = peace_state -> FirstChildElement ();
	assert (options != NULL);

	for (TiXmlElement * option = options -> FirstChildElement ("option");
				option != NULL; option = option -> NextSiblingElement ("option"))
	{
		string message = option -> Attribute ("say");
		assert (! message . empty ());
		
		CEGUI :: ListboxTextItem * item = new CEGUI :: ListboxTextItem (message);
		chat_window -> addItem (item);
		chat_window -> ensureItemIsVisible (chat_window -> getItemCount ());
	}

	assert (Play_State :: is_initialized ());
}

//  destructor
Play_State ::
	~Play_State ()
{
	Engines :: Log :: trace (me, Play_State :: get_class_name (), "~");
	assert (Play_State :: is_initialized ());

	forget_dependencies ();
}

//	virtual
bool Play_State ::
	is_initialized ()
	const
{
//	Engines :: Log :: trace (me, Play_State :: get_class_name (), "is_initialized");
	assert (Singleton <Play_State> :: is_initialized ());
	assert (Algorithm :: is_initialized ());

	return true;
}

//	virtual
Algorithm * Play_State ::
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
		|| Engines :: Input_Engine :: get () . get_key ("Escape", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Menu", true))
	{
		return & Menu_State :: get ();
	}

	Engines :: GUI_Engine :: get () . set_active_gui (gui);

	World :: get () . update ();

	//	Handle movement
	//	Normal WASD keys don't work on all keyboard layouts, so we'll use ESDF for now.

	float top_speed = 0;
	if (Engines :: Input_Engine :: get () . get_key ("e", false))
	{
		top_speed = 1;
	}
	else if (Engines :: Input_Engine :: get () . get_key ("d", false))
	{
		top_speed = - 0.5;
	}
	Items :: Player_Character :: get () . get_movable_model () . move (top_speed);
	
	float top_angular_speed = 0;
	if (Engines :: Input_Engine :: get () . get_key ("s", false))
	{
		top_angular_speed = 1;
	}
	else if (Engines :: Input_Engine :: get () . get_key ("f", false))
	{
		top_angular_speed = - 1;
	}
	Items :: Player_Character :: get () . get_movable_model () . turn (top_angular_speed);
	
	//	reset your orientation
	if (Engines :: Input_Engine :: get () . get_key ("space", false)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Reset", true))
	{
		Items :: Player_Character :: get () . get_movable_model () . reset ();
	}

	Items :: NPC & closest_npc = * World :: get () . get_active_state () . npcs . get_child ();

	//	hit
	if (Engines :: Input_Engine :: get () . get_key ("h", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Hit", true))
	{
		Items :: Player_Character :: get () . hit ("agressive", closest_npc);
	}
	
	//	move the weapon
	if (Engines :: Input_Engine :: get () . get_key ("m", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Move", true))
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
		Engines :: Settings :: get () . increase_camera_distance (- mouse_position . z / 500);
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
