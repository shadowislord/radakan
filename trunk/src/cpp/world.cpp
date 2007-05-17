#include "dead_state.hpp"
#include "fight_state.hpp"
#include "game.hpp"
#include "gui.hpp"
#include "input_engine.hpp"
#include "menu_state.hpp"
#include "speech_state.hpp"
#include "world.hpp"

using namespace std;
using namespace TSL;

//	static
const string World ::
	get_class_name ()
{
	return "World";
}

//	static
const int World ::
	min_x (- 1);
	
//	static
const int World ::
	max_x (2);
	
//	static
const int World ::
	min_z (- 1);
	
//	static
const int World ::
	max_z (1);
	
//	static
const float World ::
	min_vertical_camera_angle (- Ogre :: Math :: HALF_PI);
	
//	static
const float World ::
	max_vertical_camera_angle (Ogre :: Math :: HALF_PI);

//	a few constants for our stepper
const float max_frame_time (0.1);
const float time_scale (1.0);
const float time_step (0.1);

//  constructor
World ::
	World (Ogre :: SceneManager & scene_manager, string tsl_path) :
	Object ("world"),
	Environment (scene_manager, Ogre :: Vector3 (0, - 9.81, 0)),
	OgreOde :: ExactVariableStepHandler
	(
		this,
		OgreOde :: StepHandler :: QuickStep,
		time_step,
		max_frame_time,
		time_scale
	),
	gui (GUI_Engine :: get () . create_gui ("sector.cfg")),
	camera (* scene_manager . createCamera ("world camera"))
{
	Log :: trace <World> (me, "", "~scene_manager~", tsl_path);
	assert (Singleton <World> :: is_initialized ());
	assert (Algorithm <Game> :: is_initialized ());
	assert (State_Machine <Tile> :: is_initialized ());
	assert (Environment :: is_initialized ());
	assert (Battle_Engine :: is_initialized ());

	camera . setNearClipDistance (0.001);
	camera . setFarClipDistance (80);

	new Alive_State ();
	new Chat_State ();
	new Dead_State ();
	new Fight_State ();

	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			pair <int, int> coordinates (x, z);
			Log :: log (me) << "tile position: (" << x << ", " << z << ")" << endl;
			tiles [coordinates] = new Tile (coordinates, tsl_path);
			Log :: log (me) << * tiles [coordinates] << endl;
			bool check = add (* tiles [coordinates]);
			assert (check);
		}
	}
	
	assert (Player :: is_instantiated ());

	player_model = & Player :: get () . get_movable_model ();

	set_active_state (* tiles [pair <int, int> (0, 0)]);

	setCollisionListener (this);

	assert (World :: is_initialized ());
}

//  destructor
World ::
	~World ()
{
	Log :: trace <World> (me, "~");
	assert (World :: is_initialized ());

	assert (Singleton <World> :: is_initialized ());
	assert (Algorithm <Game> :: is_initialized ());
	assert (State_Machine <Tile> :: is_initialized ());
	assert (Environment :: is_initialized ());
	assert (Battle_Engine :: is_initialized ());
}

//	virtual
bool World ::
	is_initialized ()
	const
{
//	Log :: trace <World> (me, "is_initialized");
	assert (Singleton <World> :: is_initialized ());
	assert (Algorithm <Game> :: is_initialized ());
	assert (State_Machine <Tile> :: is_initialized ());
	assert (Environment :: is_initialized ());
	assert (Battle_Engine :: is_initialized ());

	return true;
}

//	virtual
void World ::
	set_active_state (Tile & tile)
{
	assert (is_initialized ());
	assert (tile . is_type <Tile> ());
	
	if (tile != get_active_state ())
	{
		Log :: show ("current tile: " + tile);

		if (! tile . contains (* player_model))
		{
			assert (get_active_state () . contains (* player_model));
			get_active_state () . move (* player_model, tile);
		}

		State_Machine <Tile> :: set_active_state (tile);
	}
}

//	virtual
Algorithm <Game> & World ::
	transit (Game & owner)
{
	assert (is_initialized ());

	for (Tile * tile = get_child (); tile != NULL; tile = get_another_child ())
	{
		for (NPC * npc = tile -> npcs . get_child (); npc != NULL;
			npc = tile -> npcs . get_another_child ())
		{
			npc -> run ();	//	run the AI
		}
	}

	//	Handle movement
	//	Normal WASD keys don't work on all keyboard layouts, so we'll use ESDF for now.

	const float turn_lenght = owner . get_last_turn_lenght ();
	
	float top_speed = 0;
	if (Input_Engine :: get () . get_key ("e", false))
	{
		top_speed = 1;
	}
	else if (Input_Engine :: get () . get_key ("d", false))
	{
		top_speed = - 0.5;
	}
	player_model -> move (top_speed, turn_lenght);
	
	float top_angular_speed = 0;
	if (Input_Engine :: get () . get_key ("s", false))
	{
		top_angular_speed = 1;
	}
	else if (Input_Engine :: get () . get_key ("f", false))
	{
		top_angular_speed = - 1;
	}
	player_model -> turn (top_angular_speed, turn_lenght);
	
	//	reset your orientation
	if (Input_Engine :: get () . get_key ("space", false))
	{
		player_model -> reset ();
	}

	Ogre :: Vector3 position = player_model -> node . getPosition ();

	int x = int (floor (position . x / Tile :: side_length));
	int z = int (floor (position . z / Tile :: side_length));
	assert (Tile :: side_length * min_x <= x);
	assert (x < Tile :: side_length * (max_x + 1));
	assert (Tile :: side_length * min_z <= z);
	assert (z < Tile :: side_length * (max_z + 1));
	Tile * new_active_tile = tiles [pair <int, int> (x, z)];
	assert (new_active_tile != NULL);
	set_active_state (* new_active_tile);

	//	pause
	if (Input_Engine :: get () . get_key ("Escape", true)
					|| Input_Engine :: get () . get_gui_button ("Menu", true))
	{
		return Menu_State :: get ();
	}

	//	hit
	if (Input_Engine :: get () . get_key ("h", true)
					|| Input_Engine :: get () . get_gui_button ("Hit", true))
	{
		Battle_Engine :: hit (Player :: get (), * get_active_state () . npcs . get_child ());
	}
	
	//	chat
	if (Input_Engine :: get () . get_key ("c", true))
	{
		NPC & npc = * get_active_state () . npcs . get_child ();
		if (npc . is_dead ())
		{
			Log :: show ("You can't talk with the dead.");
		}
		else
		{
			npc . set_active_state (Speech_State :: create ("Hello world!"));
		}
	}
	
	//	dead
	if (Player :: get () . is_dead ())
	{
		return Menu_State :: get ();
	}

	//	move the weapon
	if (Input_Engine :: get () . get_key ("m", true)
					|| Input_Engine :: get () . get_gui_button ("Move", true))
	{
		NPC & npc = * get_active_state () . npcs . get_child ();
		if (! Player :: get () . hands . is_empty ())
		{
			assert (! Player :: get () . hands . is_empty ());
			Player :: get () . hands . move (* Player :: get () . hands . get_child (), npc . hands);
			assert (Player :: get () . hands . is_empty ());
			assert (! npc . hands . is_empty ());
			Log :: show ("You gave your weapon to the ninja.");
		}
		else if (! npc . hands . is_empty ())
		{
			assert (! npc . hands . is_empty ());
			npc . hands . move (* npc . hands . get_child (), Player :: get () . hands);
			assert (npc . hands . is_empty ());
			assert (! Player :: get () . hands . is_empty ());
			Log :: show ("You took your weapon from the ninja.");
		}
		else
		{
			Log :: show ("Both you and the ninja don't have a weapon.");
		}
	}

	if (Input_Engine :: get () . get_mouse_button
						(Input_Engine :: get () . middle_mouse_button, false))
	{
		const Ogre :: Vector3 & mouse_position = Input_Engine :: get () . get_mouse_position (Input_Engine :: relative);

		Log :: log (me) << "mouse offset: " << to_string (mouse_position) << endl;

		if (mouse_position . x != 0)
		{
			player_model -> turn
						(- Ogre :: Math :: Sign (mouse_position . x), /*Ogre :: Math :: Abs (mouse_position . x) * */turn_lenght);
		}
		vertical_camera_angle -= turn_lenght * mouse_position . y / 3;

		if (vertical_camera_angle < min_vertical_camera_angle)
		{
			vertical_camera_angle = min_vertical_camera_angle;
		}
		else if (max_vertical_camera_angle < vertical_camera_angle)
		{
			vertical_camera_angle = max_vertical_camera_angle;
		}
	}

	for (Tile * tile = get_child (); tile != NULL; tile = get_another_child ())
	{
		tile -> collide ();
	}
	
	OgreOde :: World :: step (0.1);
	updateDrawState ();
	synchronise ();
	clearContacts ();

	camera . setPosition
	(
		player_model -> node . getPosition ()
		+ player_model -> get_top_direction ()
		* (Player :: get () . camera_distance
			- Input_Engine :: get () . get_mouse_position () . z / 100)
	);
	camera . setOrientation
	(
		make_quaternion
		(
			vertical_camera_angle,
			player_model -> get_side_direction ()
		)
		* player_model -> node . getOrientation ()
	);

	return owner . get_active_state ();
}

void World ::
	enter (Game & owner)
{
	GUI_Engine :: get () . activate (gui);
}

bool World ::
	collision (OgreOde :: Contact * contact)
{
	// Check for collisions between things that are connected and ignore them
	OgreOde :: Geometry * g1 = contact -> getFirstGeometry ();
	OgreOde :: Geometry * g2 = contact -> getSecondGeometry ();

	if ((g1 == NULL) || (g2 == NULL))
	{
		return false;
	}
	
	contact -> setCoulombFriction (0.8);
	contact -> setBouncyness (0);

//	Log :: log (me) << "collision: " << g1 -> getClass () << " " << g2 -> getClass () << endl;
	return true;
	
	OgreOde :: Body * b1 = g1 -> getBody ();
	OgreOde :: Body * b2 = g2 -> getBody ();

	if ((b1 != NULL) && (b2 != NULL))
	{
		Log :: log (me) << "model-model collision: " << b1 -> getName () << " and " << b2 -> getName () << endl;
		Log :: log (me) << "model position: " << b1 -> getPosition () << endl;
		Log :: log (me) << "model position: " << b2 -> getPosition () << endl;
		Log :: log (me) << "geometry position: " << g1 -> getPosition () << endl;
		Log :: log (me) << "geometry position: " << g2 -> getPosition () << endl;

		// Set the friction at the contact
		contact -> setCoulombFriction (0.5);
		contact -> setBouncyness (0);
		return true;
	}
	else if ((b1 == NULL) && (b2 == NULL))
	{
		return false;
	}
	
	//	there can only be one ground geometry
	if
	(
		(
			(g1 -> getClass () == OgreOde :: Geometry :: Class_TriangleMesh)
			|| (g1 -> getClass () == OgreOde :: Geometry :: Class_InfinitePlane)
		)
		!=
		(
			(g2 -> getClass () != OgreOde :: Geometry :: Class_TriangleMesh)
			|| (g2 -> getClass () != OgreOde :: Geometry :: Class_InfinitePlane)
		)
	)
	{
		return false;
	}

	if
	(
		(g1 -> getClass () == OgreOde :: Geometry :: Class_TriangleMesh)
		|| (g1 -> getClass () == OgreOde :: Geometry :: Class_InfinitePlane)
	)
	{
		//	swap everything
		
		OgreOde :: Geometry * temp = g2;
		g2 = g1;
		g1 = temp;

		b1 = b2;
	}

	Log :: log (me) << "model-ground collision: " << b1 -> getName () << endl;
	Log :: log (me) << "model position: " << b1 -> getPosition () << endl;
	Log :: log (me) << "geometry position: " << g1 -> getPosition () << endl;
	Log :: log (me) << "geometry position: " << g2 -> getPosition () << endl;
	contact -> setCoulombFriction (0.5);
	contact -> setBouncyness (0);
	return true;
}
