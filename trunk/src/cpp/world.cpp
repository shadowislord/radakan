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

const int World :: min_x = - 1;
const int World :: max_x = 2;
const int World :: min_z = - 1;
const int World :: max_z = 1;
const int World :: min_vertical_camera_angle = - 90;
const int World :: max_vertical_camera_angle = 90;

//	a few constants for our stepper
const float max_frame_time = 0.1;
const float time_scale = 1.0;

//	raeez: If we set this to a normal value (say 0.1) we get sucked way too
//	quickly to be able  to see what is going on
const float time_step = 0.9;

//  constructor
World ::
	World (GUI & new_gui, Ogre :: SceneManager & scene_manager, string tsl_path) :
	Object ("world"),
	OgreOde :: World (& scene_manager),
	OgreOde :: ExactVariableStepHandler
	(
		this,   //	TODO: don't use 'this' in the constructor.
		OgreOde :: StepHandler :: QuickStep,
		time_step,
		max_frame_time,
		time_scale
	),
	gui (new_gui),
	camera (* scene_manager . createCamera ("world camera"))
{
	log (TSL_DEBUG) << get_class_name () << " (" << new_gui << "~scene_manager~, " << tsl_path << ")" << endl;
	assert (Singleton <World> :: is_initialized ());
	assert (Algorithm <TSL> :: is_initialized ());
	assert (State_Machine <Tile> :: is_initialized ());

	camera . setNearClipDistance (0.01);
	camera . setFarClipDistance (80);

	setShowDebugGeometries (true);

	log (TSL_DEBUG) << "ERP: " << getERP () << endl;
	log (TSL_DEBUG) << "CFM: " << getCFM () << endl;

	setGravity (Ogre :: Vector3 (0, - 9.81, 0));

	//	TODO make the next line work.
	getSceneManager () -> setSkyDome (true, "Peaceful", 10, 5);

	new Dead_State ();
	new Fight_State ();
	new Peace_State ();

	Item :: scene_manager = getSceneManager ();

	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			pair <int, int> coordinates (x, z);
			log (TSL_DEBUG) << "tile position: (" << x << ", " << z << ")" << endl;
			log (TSL_DEBUG) << get_class_name () << " (" << new_gui << "~scene_manager~, " << tsl_path << ") F" << endl;
			tiles [coordinates] = new Tile (* this, coordinates, tsl_path);
			log (TSL_DEBUG) << get_class_name () << " (" << new_gui << "~scene_manager~, " << tsl_path << ") G" << endl;
			log (TSL_DEBUG) << * tiles [coordinates] << endl;
			log (TSL_DEBUG) << get_class_name () << " (" << new_gui << "~scene_manager~, " << tsl_path << ") H" << endl;
			bool check = add (* tiles [coordinates]);
			log (TSL_DEBUG) << get_class_name () << " (" << new_gui << "~scene_manager~, " << tsl_path << ") I" << endl;
			assert (check);
		}
	}

	set_active_state (* tiles [pair <int, int> (0, 0)]);

	assert (World :: is_initialized ());
}

//  destructor
World ::
	~World ()
{
	log (TSL_DEBUG) << "~" << get_class_name () << " ()" << endl;
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
	assert (Item :: scene_manager == to_type <World> () . getSceneManager ());

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
		show () << "current tile:" << tile;
		
		if (Player :: is_instantiated ())
		{
			get_active_state () . move (Player :: get () . to_type <Item> (), tile);
		}
		else
		{
			tile . add (Player :: create ("Player", "ninja.mesh", 80, 65));
			Player :: get () . get_body () . setPosition
											(Ogre :: Vector3 (10.5, 4, 10.5));
			Player :: get () . get_body () . set_scale (0.004);
		}
		
		State_Machine <Tile> :: set_active_state (tile);
	}
}

//	virtual
Algorithm <TSL> & World ::
	transit (TSL & owner)
{
	assert (is_initialized ());

	for (Tile * tile = get_child (); tile != NULL; tile = get_another_child ())
	{
		for (NPC_iterator npc = tile -> npcs . begin (); npc != tile -> npcs . end (); npc ++)
		{
			(* npc) -> run ();
		}
	}

	//	Handle movement
	//	Normal WASD keys don't work on all keyboard layouts, so we'll use ESDF for now.
	const float turn_lenght = owner . get_last_turn_lenght ();
	if (Input_Engine :: get () . get_key ("e", false))
	{
		Player :: get () . get_body () . move (8 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("d", false))
	{
		Player :: get () . get_body () . move (- 4 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("s", false))
	{
		Player :: get () . get_body () . turn (5 * turn_lenght);
	}
	if (Input_Engine :: get () . get_key ("f", false))
	{
		Player :: get () . get_body () . turn (- 5 * turn_lenght);
	}

	Ogre :: Vector3 position = Player :: get () . get_body () . getPosition ();

	int x = int (floor (position . x / Tile :: side_length));
	int z = int (floor (position . z / Tile :: side_length));
	log (TSL_DEBUG) << "position: " << to_string (position) << " -> (" << x << ", " << z << ")" << endl;
	// We are being sucked somewhere by a force... this takes us out of possible space
	// and causes the assertions below to fail. And of course if the assertions are failing
	// it means we would be translated somewhere where there are no tiles.
	// temporary measure of placing you back a notch only to be sucked ober and over and over again :)
	assert (Tile :: side_length * min_x <= x);
	assert (x < Tile :: side_length * (max_x + 1));
	assert (Tile :: side_length * min_z < z);
	assert (z < Tile :: side_length * (max_z + 1));
	Tile * new_active_tile = tiles [pair <int, int> (x, z)];
	assert (new_active_tile != NULL);
	set_active_state (* new_active_tile);

	//	pause
	if (Input_Engine :: get () . get_key ("Escape", true)
					|| Input_Engine :: get () . get_gui_button ("Menu", true))
	{
		show () << "Menu (game paused)";
		return Menu_State :: get ();
	}

	//	hit
	if (Input_Engine :: get () . get_key ("h", true)
					|| Input_Engine :: get () . get_gui_button ("Hit", true))
	{
		NPC & npc = * * get_active_state () . npcs . begin ();
		if (! npc . is_dead ())
		{
			Battle_Engine :: get () . hit (Player :: get (), npc);
		}
		else
		{
			show () << "Mutilating a dead body is *not* nice.";
		}
	}
	
	//	dead
	if (Player :: get () . is_dead ())
	{
		show () << "You died";
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
			show () << "You gave your weapon to the ninja.";
		}
		else if (! npc . hands . is_empty ())
		{
			assert (! npc . hands . is_empty ());
			npc . hands . move (* npc . hands . get_child (), Player :: get () . hands);
			assert (npc . hands . is_empty ());
			assert (! Player :: get () . hands . is_empty ());
			show () << "You took your weapon from the ninja.";
		}
		else
		{
			show () << "Both you and the ninja don't have a weapon.";
		}
	}

	if (Input_Engine :: get () . get_mouse_button
						(Input_Engine :: get () . middle_mouse_button, false))
	{
		const Ogre :: Vector3 & mouse_position = Input_Engine :: get () . get_mouse_position (true);

		log (TSL_DEBUG) << "mouse offset: " << to_string (mouse_position) << endl;

		if (mouse_position . x != 0)
		{
			Player :: get () . get_body () . turn
						(- 5 * turn_lenght * mouse_position . x);
		}
		vertical_camera_angle -= turn_lenght * mouse_position . y;

		if (vertical_camera_angle < min_vertical_camera_angle)
		{
			vertical_camera_angle = min_vertical_camera_angle;
		}
		else if (max_vertical_camera_angle < vertical_camera_angle)
		{
			vertical_camera_angle = max_vertical_camera_angle;
		}
	}

	if (Input_Engine :: get () . get_mouse_button
							(Input_Engine :: get () . right_mouse_button, true))
	{
		if (0.1 < Ogre :: Math :: RangeRandom (0, 1))
		{
			show () << owner . get_FPS ();
		}
		else
		{
			show () << "Trivia: there were 1961 trees in each forest.";
		}
	}

	Ogre :: Vector3 pre_position = Player :: get () . get_body () . getPosition ();
	OgreOde :: StepHandler :: step (turn_lenght);
	OgreOde :: World :: synchronise ();
	Ogre :: Vector3 displacement = Player :: get () . get_body () . getPosition () - pre_position;
	if (displacement != zero)
	{
		log (TSL_DEBUG) << "Physics displacement: " << to_string (displacement) << endl;
	}
	
	camera . setPosition
	(
		Player :: get () . get_body () . getPosition ()
		+ Player :: get () . get_body () . get_top_direction ()
		* (Player :: get () . camera_distance
			- Input_Engine :: get () . get_mouse_position (false) . z / 100)
	);
	camera . setOrientation
	(
		make_quaternion
		(
			vertical_camera_angle,
			Player :: get () . get_body () . get_side_direction ()
		)
		* Player :: get () . get_body () . getOrientation ()
	);

	GUI_Engine :: get () . activate (gui);

	return * this;
}
