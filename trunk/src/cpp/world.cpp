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
const float time_step = 0.1;

//  constructor
World ::
	World (GUI & new_gui, string tsl_path) :
	Object ("world"),
	OgreOde :: ExactVariableStepHandler
	(
		& Environment :: get (),
		OgreOde :: StepHandler :: QuickStep,
		time_step,
		max_frame_time,
		time_scale
	),
	gui (new_gui),
	camera (* Environment :: get () . getSceneManager () -> createCamera ("world camera"))
{
	log (debugging) << get_class_name () << " (" << new_gui << "~scene_manager~, " << tsl_path << ")" << endl;
	assert (Singleton <World> :: is_initialized ());
	assert (Algorithm <TSL> :: is_initialized ());
	assert (State_Machine <Tile> :: is_initialized ());
	assert (Environment :: is_instantiated ());
	assert (Environment :: get () . is_initialized ());

	camera . setNearClipDistance (0.001);
	camera . setFarClipDistance (80);

	new Dead_State ();
	new Fight_State ();
	new Peace_State ();

	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			pair <int, int> coordinates (x, z);
			log (debugging) << "tile position: (" << x << ", " << z << ")" << endl;
			tiles [coordinates] = new Tile (coordinates, tsl_path);
			log (debugging) << * tiles [coordinates] << endl;
			bool check = add (* tiles [coordinates]);
			assert (check);
		}
	}
	
	assert (Player :: is_instantiated ());

	set_active_state (* tiles [pair <int, int> (0, 0)]);

	Environment :: get () . setCollisionListener (this);

	assert (World :: is_initialized ());
}

//  destructor
World ::
	~World ()
{
	log (debugging) << "~" << get_class_name () << " ()" << endl;
	assert (State_Machine <Tile> :: is_initialized ());

	assert (World :: is_initialized ());
}

//	virtual
bool World ::
	is_initialized ()
	const
{
//	Object :: log (debugging) << get_class_name () << " :: is_initialized ()" << endl;
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
	assert (is_initialized ());
	assert (tile . is_type <Tile> ());
	
	if (tile != get_active_state ())
	{
		show () << "current tile:" << tile;

		if (! tile . contains (Player :: get () . get_body ()))
		{
			assert (get_active_state () . contains (Player :: get () . get_body ()));
			get_active_state () . move (Player :: get () . get_body (), tile);
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

	Ogre :: Vector3 position = Player :: get () . get_body () . node . getPosition ();

	int x = int (floor (position . x / Tile :: side_length));
	int z = int (floor (position . z / Tile :: side_length));
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

		log (debugging) << "mouse offset: " << to_string (mouse_position) << endl;

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

	Ogre :: Vector3 pre_position = Player :: get () . get_body () . node . getPosition ();
//	OgreOde :: StepHandler :: step (turn_lenght);
	Environment :: get () . getDefaultSpace () -> collide ();
	Environment :: get () . step (turn_lenght);
	Environment :: get () . updateDrawState ();
	Environment :: get () . synchronise ();
	Environment :: get () . clearContacts ();

	Ogre :: Vector3 displacement = Player :: get () . get_body () . node . getPosition () - pre_position;
	if (displacement != zero_vector)
	{
		log (debugging) << "Physics displacement: " << to_string (displacement) << endl;
	}
	
	camera . setPosition
	(
		Player :: get () . get_body () . node . getPosition ()
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
		* Player :: get () . get_body () . node . getOrientation ()
	);

	GUI_Engine :: get () . activate (gui);

	return * this;
}

bool World ::
	collision (OgreOde :: Contact * contact)
{
	show () << "collision! ";

	// Check for collisions between things that are connected and ignore them
	OgreOde :: Geometry * const g1 = contact -> getFirstGeometry ();
	OgreOde :: Geometry * const g2 = contact -> getSecondGeometry ();

	if (g1 && g2)
	{
		const OgreOde :: Body * const b1 = g1 -> getBody ();
		const OgreOde :: Body * const  b2 = g2 -> getBody ();
	}

	// Set the friction at the contact
	contact -> setCoulombFriction (OgreOde :: Utility :: Infinity);
	contact -> setBouncyness (0.1);

	// Yes, this collision is valid
	return true;
}
