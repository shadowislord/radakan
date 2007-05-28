#include "alive_state.hpp"
#include "chat_state.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "settings.hpp"
#include "play_state.hpp"
#include "player_character.hpp"
#include "quit_state.hpp"

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
	Play_State (Ogre :: SceneManager & scene_manager, string tsl_path) :
	Object ("play state"),
	gui (Engines :: GUI_Engine :: get () . create_gui ("sector.cfg")),
	camera (* scene_manager . createCamera ("camera"))
{
	Engines :: Log :: trace (me, Play_State :: get_class_name (), "", "~scene_manager~", tsl_path);

	new Alive_State ();
	new Chat_State ();
	new Dead_State ();
	new Fight_State ();

	new World (scene_manager, tsl_path);

	camera . setNearClipDistance (0.001);
	camera . setFarClipDistance (80);

	assert (Play_State :: is_initialized ());
}

//  destructor
Play_State ::
	~Play_State ()
{
	Engines :: Log :: trace (me, Play_State :: get_class_name (), "~");
	assert (Play_State :: is_initialized ());

	forget_dependencies ();

	World :: destruct ();

	Fight_State :: destruct ();
	Dead_State :: destruct ();
	Chat_State :: destruct ();
	Alive_State :: destruct ();
}

//	virtual
bool Play_State ::
	is_initialized ()
	const
{
//	Engines :: Log :: trace (me, Play_State :: get_class_name (), "is_initialized");
	assert (Singleton <Play_State> :: is_initialized ());
	assert (Algorithm <Engines :: Game> :: is_initialized ());
	assert (Observable <Play_State> :: is_initialized ());

	return true;
}

//	virtual
Algorithm <Engines :: Game> & Play_State ::
	transit (Engines :: Game & owner, const Object & message)
{
	assert (is_initialized ());

	//	quit
	if (message == terminate)
	{
		return Quit_State :: get ();
	}

	//	dead
	if (Items :: Player_Character :: get () . is_dead ())
	{
		return Menu_State :: get ();
	}

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
	if (Engines :: Input_Engine :: get () . get_key ("space", false))
	{
		Items :: Player_Character :: get () . get_movable_model () . reset ();
	}

	//	pause
	if (Engines :: Input_Engine :: get () . get_key ("Escape", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Menu", true))
	{
		return Menu_State :: get ();
	}

	//	hit
	if (Engines :: Input_Engine :: get () . get_key ("h", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Hit", true))
	{
		Items :: Player_Character :: get ()
			. chat
				("Duel!", * World :: get () . get_active_state () . npcs . get_child ());
	}
	
	//	chat
	if (Engines :: Input_Engine :: get () . get_key ("c", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Chat", true))
	{
		Items :: Player_Character :: get ()
			. chat
				("Hello.", * World :: get () . get_active_state () . npcs . get_child ());
	}
	
	//	move the weapon
	if (Engines :: Input_Engine :: get () . get_key ("m", true)
		|| Engines :: Input_Engine :: get () . get_gui_button ("Move", true))
	{
		Items :: NPC & npc = * World :: get () . get_active_state () . npcs . get_child ();
		if (! Items :: Player_Character :: get () . hands . is_empty ())
		{
			assert (! Items :: Player_Character :: get () . hands . is_empty ());
			Items :: Player_Character :: get () . hands . move (* Items :: Player_Character :: get () . hands . get_child (), npc . hands);
			assert (Items :: Player_Character :: get () . hands . is_empty ());
			assert (! npc . hands . is_empty ());
			Engines :: Log :: show ("You gave your weapon to the ninja.");
		}
		else if (! npc . hands . is_empty ())
		{
			assert (! npc . hands . is_empty ());
			npc . hands . move (* npc . hands . get_child (), Items :: Player_Character :: get () . hands);
			assert (npc . hands . is_empty ());
			assert (! Items :: Player_Character :: get () . hands . is_empty ());
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
				(- World :: get () . get_last_turn_lenght () * mouse_position . y / 3);
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

	//	Return me or a child state.
	return owner . get_active_state ();
}

void Play_State ::
	enter (Engines :: Game & owner)
{
	Engines :: Log :: trace (me, Play_State :: get_class_name (), "enter", owner);
	assert (is_initialized ());

	Engines :: GUI_Engine :: get () . set_active_state (gui);
}
