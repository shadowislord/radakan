#include "tsl.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "peace_state.hpp"
#include "play_state.hpp"
#include "menu_state.hpp"

using namespace std;
using namespace tsl;

//  constructor
Play_State ::
	Play_State () :
	Object ("play state"),
	gui (GUI_Engine :: get () . create_gui ("sector.cfg"))
{
	assert (Singleton <Play_State> :: is_initialized ());
	assert (Algorithm <TSL> :: is_initialized ());
	assert (State_Machine <Sector> :: is_initialized ());

	new Dead_State ();
	new Fight_State ();
	new Peace_State ();

	bool check = add (Sector :: create ("Sector 1", TSL :: get () . new_scene_manager ()));
	assert (check);
	check = add (Sector :: create ("Sector 2", TSL :: get () . new_scene_manager ()));
	assert (check);

	assert (Play_State :: is_initialized ());
}

//  destructor
Play_State ::
	~Play_State ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (State_Machine <Sector> :: is_initialized ());

	clear ();
	
	delete & Dead_State :: get ();
	delete & Fight_State :: get ();
	delete & Peace_State :: get ();
}

//	virtual
bool Play_State ::
	is_initialized ()
	const
{
	assert (Singleton <Play_State> :: is_initialized ());
	assert (Algorithm <TSL> :: is_initialized ());
	assert (State_Machine <Sector> :: is_initialized ());
	
	return true;
}

//	static
string Play_State ::
	get_class_name ()
{
	return "Play_State";
}

//	virtual
void Play_State ::
	set_active_state (Sector & state)
{
	assert (state . is_type <Sector> ());
	
	if (state != get_active_state ())
	{
		get_active_state () . move (Player :: get () . to_type <Item> (), state);
		
		State_Machine <Sector> :: set_active_state (state);

		//	Update camera & scene manager:
		TSL :: get () . set_camera (state . get_camera ());
		GUI_Engine :: get () . set_scene_manager (state . get_scene_manager ());
	}
}

//	virtual
Algorithm <TSL> & Play_State ::
	transit (TSL & owner)
{
	assert (is_initialized ());

	Sector & sector = get_active_state ();
	
	//	stepSimulation (milliseconds_passed / 1000000.f);

	for (set <NPC *> :: const_iterator i = sector . npcs . begin ();
											i != sector . npcs . end (); i ++)
	{
		(* i) -> run ();
	}

	//	Handle movement
	//	Normal WASD keys don't work on all keyboard layouts, so we'll use ESDF for now.
	unsigned long turn_lenght = owner . get_last_turn_lenght ();
	if (Input_Engine :: get () . get_key ("e", false))
	{
		Player :: get () . get_representation () . move (0.2 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("d", false))
	{
		Player :: get () . get_representation () . move (- 0.1 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("s", false))
	{
		Player :: get () . get_representation () . turn (0.005 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("f", false))
	{
		Player :: get () . get_representation () . turn (- 0.005 * turn_lenght);
	}

	//	pause
	if (Input_Engine :: get () . get_key ("Escape", true)
					|| Input_Engine :: get () . get_gui_button ("Menu", true))
	{
		gui . show ("Menu (game paused)");
		return Menu_State :: get ();
	}

	//	hit
	if (Input_Engine :: get () . get_key ("h", true)
					|| Input_Engine :: get () . get_gui_button ("Hit", true))
	{
		NPC & npc = * * sector . npcs . begin ();
		if (! npc . is_dead ())
		{
			gui . show (Battle_Engine :: get () . hit (Player :: get (), npc));
		}
		else
		{
			gui . show ("Mutilating a dead body is *not* nice.");
		}
	}
	
	//	dead
	if (Player :: get () . is_dead ())
	{
		gui . show ("You died");
		return Menu_State :: get ();
	}

	//	move the weapon
	if (Input_Engine :: get () . get_key ("m", true)
					|| Input_Engine :: get () . get_gui_button ("Move", true))
	{
		//	Memo to self (Tinus):
		//	NPC npc = ...;  ->  that *copies* the NPC.
		NPC & npc =  * * sector . npcs . begin ();
		if (! Player :: get () . hands . is_empty ())
		{
			assert (! Player :: get () . hands . is_empty ());
			Player :: get () . hands . move (* Player :: get () . hands . get_child (), npc . hands);
			assert (Player :: get () . hands . is_empty ());
			assert (! npc . hands . is_empty ());
			gui . show ("You gave your weapon to the ninja.");
		}
		else if (! npc . hands . is_empty ())
		{
			assert (! npc . hands . is_empty ());
			npc . hands . move (* npc . hands . get_child (), Player :: get () . hands);
			assert (npc . hands . is_empty ());
			assert (! Player :: get () . hands . is_empty ());
			gui . show ("You took your weapon from the ninja.");
		}
		else
		{
			gui . show ("Both you and the ninja don't have a weapon.");
		}
	}

	if (Input_Engine :: get () . get_mouse_button
						(Input_Engine :: get () . middle_mouse_button, false))
	{
		float x_offset = Input_Engine :: get () . get_mouse_position (true) . first;
		float y_offset = Input_Engine :: get () . get_mouse_position (true) . second;

		debug () << "offset: " << x_offset << ", " << y_offset << endl;

		if (x_offset != 0)
		{
			x_offset = - 0.005 * owner . get_last_turn_lenght () * x_offset;

			Player :: get () . get_representation () . turn (x_offset);
		}
		vertical_camera_angle -= 0.001 * owner . get_last_turn_lenght () * y_offset;
	}

	if (Input_Engine :: get () . get_mouse_button
							(Input_Engine :: get () . right_mouse_button, true))
	{
		if (0.1 < Ogre :: Math :: RangeRandom (0, 1))
		{
			gui . show (owner . get_FPS ());
		}
		else
		{
			gui . show ("Trivia: there are 1961 trees in each forest.");
		}
	}

	if (Input_Engine :: get () . get_key ("1", true))
	{
		set_active_state (get_child ("Sector 1"));
		gui . show ("Sector 1");
	}

	if (Input_Engine :: get () . get_key ("2", true))
	{
		set_active_state (get_child ("Sector 2"));
		gui . show ("Sector 2");
	}

	sector . camera . setPosition
	(
		Player :: get () . get_representation () . get_position ()
		+ Player :: get () . camera_distance
		* Player :: get () . get_representation () . get_top_direction ()
	);
	sector . camera . setOrientation
	(
		make_quaternion
		(
			vertical_camera_angle,
			Player :: get () . get_representation () . get_side_direction ()
		)
		* Player :: get () . get_representation () . get_orientation ()
	);

	GUI_Engine :: get () . activate (gui);

	return * this;
}
