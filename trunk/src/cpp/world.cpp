#include "tsl.hpp"
#include "battle_engine.hpp"
#include "dead_state.hpp"
#include "fight_state.hpp"
#include "peace_state.hpp"
#include "gui.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "world.hpp"

using namespace std;
using namespace tsl;

const int World :: min_x = 0;
const int World :: max_x = 2;
const int World :: min_z = 0;
const int World :: max_z = 1;

//  constructor
World ::
	World (GUI & new_gui, Ogre :: SceneManager & scene_manager, string tsl_path) :
	Object ("world"),
	OgreOde :: World (& scene_manager),
	gui (new_gui),
	camera (* scene_manager . createCamera ("world camera"))
{
	assert (Singleton <World> :: is_initialized ());
	assert (Algorithm <TSL> :: is_initialized ());
	assert (State_Machine <Tile> :: is_initialized ());

	camera . setNearClipDistance (0.01);
	camera . setFarClipDistance (80);

	setShowDebugGeometries (true);

	debug () << "ERP: " << getERP () << endl;
	debug () << "CFM: " << getCFM () << endl;

	setGravity (Ogre :: Vector3 (0, - 9.81, 0));

	//	TODO make the next line work.
	getSceneManager () -> setSkyDome (true, "Peaceful", 10, 5);

	new Dead_State ();
	new Fight_State ();
	new Peace_State ();

	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			pair <int, int> coordinates (x, z);
			trace () << "tile position: (" << x << ", " << z << ")" << endl;
			tiles [coordinates] = new Tile (* this, coordinates, tsl_path);
			bool check = add (* tiles [coordinates]);
			assert (check);
		}
	}

	tiles [pair <int, int> (0, 0)] -> add (Player :: create ("Player", "ninja.mesh", 80, 65));
	Player :: get () . get_representation () . setPosition (Ogre :: Vector3 (10.5, 0, 10.5));
	Player :: get () . get_representation () . set_scale (0.004);

	assert (World :: is_initialized ());
}

//  destructor
World ::
	~World ()
{
	trace () << "~" << get_class_name () << " ()" << endl;
	assert (State_Machine <Tile> :: is_initialized ());
}

//	virtual
bool World ::
	is_initialized ()
	const
{
	assert (Singleton <World> :: is_initialized ());
	assert (Algorithm <TSL> :: is_initialized ());
	assert (State_Machine <Tile> :: is_initialized ());
	
	return true;
}

//	static
string World ::
	get_class_name ()
{
	return "World";
}

//	virtual
void World ::
	set_active_state (Tile & tile)
{
	assert (tile . is_type <Tile> ());
	
	if (tile != get_active_state ())
	{
		gui . show (tile);
		get_active_state () . move (Player :: get () . to_type <Item> (), tile);
		
		State_Machine <Tile> :: set_active_state (tile);
	}
}

//	virtual
Algorithm <TSL> & World ::
	transit (TSL & owner)
{
	assert (is_initialized ());

/*	Player :: get (). get_representation () . addForce (Ogre :: Vector3 (1, 1, 1));
	cout << "force: " << Player :: get (). get_representation () . getForce () << endl;
	const int steps = 10;
	for (int i = 0; i < steps; i ++)
	{
		quickStep (1000 * owner . get_last_turn_lenght () / steps);
		Player :: get (). get_representation () . addForce (Ogre :: Vector3 (0, - 9.81, 0));
		cout << "force: " << Player :: get (). get_representation () . getForce () << endl;
		Player :: get (). get_representation () . move (0);
		
	}*/

	for (Tile * tile = get_child (); tile != NULL; tile = get_another_child ())
	{
		for (NPC_iterator npc = tile -> npcs . begin (); npc != tile -> npcs . end (); npc ++)
		{
			(* npc) -> run ();
		}
	}

	//	Handle movement
	//	Normal WASD keys don't work on all keyboard layouts, so we'll use ESDF for now.
	unsigned long turn_lenght = owner . get_last_turn_lenght ();
	if (Input_Engine :: get () . get_key ("e", false))
	{
		Player :: get () . get_representation () . move (0.008 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("d", false))
	{
		Player :: get () . get_representation () . move (- 0.004 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("s", false))
	{
		Player :: get () . get_representation () . turn (0.005 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("f", false))
	{
		Player :: get () . get_representation () . turn (- 0.005 * turn_lenght);
	}

	Ogre :: Vector3 position = Player :: get () . get_representation () . getPosition ();
	int x = int (position . x) / Tile :: side_length;
	int z = int (position . z) / Tile :: side_length;
	assert (min_x <= x);
	assert (x <= max_x);
	assert (min_z <= z);
	assert (z <= max_z);
//	debug () << "tile at (" << x << ", 0, " << z << ")" << endl;
	Tile * new_active_tile = tiles [pair <int, int> (x, z)];
	assert (new_active_tile != NULL);
	set_active_state (* new_active_tile);

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
		NPC & npc = * * get_active_state () . npcs . begin ();
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
		NPC & npc = * * get_active_state () . npcs . begin ();
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
		const Ogre :: Vector3 & mouse_position = Input_Engine :: get () . get_mouse_position (true);

		debug () << "offset: " << to_string (mouse_position)  << endl;

		if (mouse_position .x != 0)
		{
			Player :: get () . get_representation () . turn
						(- 0.005 * owner . get_last_turn_lenght () * mouse_position .x);
		}
		vertical_camera_angle -= 0.001 * owner . get_last_turn_lenght () * mouse_position . y;
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

	camera . setPosition
	(
		Player :: get () . get_representation () . getPosition ()
		+ Player :: get () . get_representation () . get_top_direction ()
		* (Player :: get () . camera_distance
			- Input_Engine :: get () . get_mouse_position (false) . z / 100)
	);
	camera . setOrientation
	(
		make_quaternion
		(
			vertical_camera_angle,
			Player :: get () . get_representation () . get_side_direction ()
		)
		* Player :: get () . get_representation () . getOrientation ()
	);

	GUI_Engine :: get () . activate (gui);

	return * this;
}
