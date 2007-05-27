#include "log.hpp"
#include "message.hpp"
#include "player_character.hpp"
#include "world.hpp"

using namespace std;
using namespace TSL;

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

//	a few constants for our stepper
const float max_frame_time (0.1);
const float time_scale (1.0);
const float time_step (0.1);

//	static
const string World ::
	get_class_name ()
{
	return "World";
}

World ::
	World (Ogre :: SceneManager & scene_manager, string tsl_path) :
	Object ("environment"),
	OgreOde :: World (& scene_manager),
	OgreOde :: ExactVariableStepHandler
	(
		this,
		OgreOde :: StepHandler :: QuickStep,
		time_step,
		max_frame_time,
		time_scale
	),
	root_node (* scene_manager . getRootSceneNode ()),
	#ifdef TSL_DEBUG
		turn (0),
	#endif
	last_turn_lenght (0)
{
	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			pair <int, int> coordinates (x, z);
			Engines :: Log :: log (me) << "tile position: (" << x << ", " << z << ")" << endl;
			tiles [coordinates] = new Tile (coordinates, tsl_path);
			Engines :: Log :: log (me) << * tiles [coordinates] << endl;
			bool check = add (* tiles [coordinates]);
			assert (check);
		}
	}

	set_active_state (* tiles [pair <int, int> (0, 0)]);

	setCollisionListener (this);

	setGravity (Ogre :: Vector3 (0, - 9.81, 0));

	//	The following line causes an error in Ogre.
	//	setShowDebugGeometries (true);

	Engines :: Log :: log (me) << "ERP: " << getERP () << endl;
	Engines :: Log :: log (me) << "CFM: " << getCFM () << endl;

	//	TODO make the next line work.
	//	getSceneManager () -> setSkyDome (true, "Peaceful", 10, 5);
}

World ::
	~World ()
{
	Engines :: Log :: trace <World> (me, "~");
	assert (World :: is_initialized ());

	//	Do nothing.
}

//	virtual
bool World ::
	is_initialized ()
	const
{
	return Object :: is_initialized ();
}

//	virtual
void World ::
	set_active_state (Tile & tile)
{
	assert (is_initialized ());

	if (tile != get_active_state ())
	{
		Engines :: Log :: show ("current tile: " + tile);

		//	The first time, 'tile' will already contain the player model.
		if (! tile . contains (Items :: Player_Character :: get () . get_movable_model ()))
		{
			get_active_state () . move (Items :: Player_Character :: get () . get_movable_model (), tile);
		}

		State_Machines :: State_Machine <Tile> :: set_active_state (tile);
	}
}

void World ::
	update ()
{
	assert (is_initialized ());
	
	//	Here, 'last_turn_lenght' contains the lenth of the current turn.
	last_turn_lenght = float (turn_lenght_timer . getMilliseconds ()) / 1000;
	turn_lenght_timer . reset ();

	#ifdef TSL_DEBUG
		Engines :: Log :: log (me) << "Turn lenght: " << last_turn_lenght << " seconds" << endl;

		turn ++;

		Engines :: Log :: log (me) << "Turn " << turn << " started" << endl;
	#endif
	
	Ogre :: Vector3 position = Items :: Player_Character :: get () . get_movable_model () . node . getPosition ();

	int x = int (floor (position . x / Tile :: side_length));
	int z = int (floor (position . z / Tile :: side_length));
	assert (Tile :: side_length * min_x <= x);
	assert (x < Tile :: side_length * (max_x + 1));
	assert (Tile :: side_length * min_z <= z);
	assert (z < Tile :: side_length * (max_z + 1));
	Tile * new_active_tile = tiles [pair <int, int> (x, z)];
	assert (new_active_tile != NULL);
	World :: get () . set_active_state (* new_active_tile);

	for (Tile * tile = get_child (); tile != NULL; tile = get_another_child ())
	{
		tile -> collide ();
	}
	
	OgreOde :: World :: step (0.1);
	updateDrawState ();
	synchronise ();
	clearContacts ();

	//	run the AI for all nearby NPCs
	Items :: Player_Character :: get () . call_observers (Object :: update);
}

const unsigned int & World ::
	get_turn ()
	const
{
	assert (is_initialized ());
	
	return turn;
}

const float & World ::
	get_last_turn_lenght ()
	const
{
	assert (is_initialized ());
	
	return last_turn_lenght;
}

bool World ::
	collision (OgreOde :: Contact * contact)
{
	//	Don't check for initialization here, as that might slow things down to much.

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
}
