#include <OgreOdeBody.h>	//	inclusion before 'OgreOdeWorld.h' to evade errors
#include <OgreOdeGeometry.h>	//	inclusion before 'OgreOdeWorld.h' to evade errors
#include <OgreOdeWorld.h>	//	inclusion before 'OgreOdeStepper.h' to evade errors
#include <OgreOdeSpace.h>
#include <OgreOdeStepper.h>

#include "engines/log.hpp"
#include "items/player_character.hpp"
#include "messages/message.hpp"
#include "movable_model.hpp"
#include "tile.hpp"
#include "world.hpp"

using namespace std;
using namespace Radakan;

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

unsigned int World ::
	turn (0);

//	a few constants for our stepper
const Ogre :: Real frame_rate = (1.0 / 60);   //aiming for 60 fps
const Ogre :: Real max_frame_time (1.0 / 4);
const Ogre :: Real time_scale (1.0);
const Ogre :: Real time_step (0.01);

//	static
string World ::
	get_class_name ()
{
	return "World";
}

const unsigned int & World ::
	get_turn ()
{
	return turn;
}

World ::
	World (boost :: shared_ptr <Ogre :: SceneManager> scene_manager) :
	Object ("world"),
	Singleton <World> (),
	State_Machine <Tile> (),
	OgreOde :: CollisionListener (),
	root_node (scene_manager -> getRootSceneNode ()),
	ogre_ode_world (new OgreOde :: World (scene_manager . get ())),
	last_turn_length (0),
	turn_lenght_timer (new Ogre :: Timer ()),
	step_handler
	(
		new OgreOde :: ForwardFixedInterpolatedStepHandler
		(
			ogre_ode_world . get (),
			OgreOde :: StepHandler :: QuickStep,
			time_step,
			frame_rate,
			max_frame_time,
			time_scale
		)
	)
{
	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			pair <int, int> coordinates (x, z);
			tiles [coordinates] = new Tile (coordinates);
		}
	}

	set_active_tile (tiles [pair <int, int> (0, 0)]);

	ogre_ode_world -> setCollisionListener (this);

    ogre_ode_world -> setGravity (Ogre :: Vector3 (0, - 9.81, 0));
    //	ogre_ode_world -> setCFM (10e-5);	//	fine tune to make a realistic simulation
    //	ogre_ode_world -> setERP (0.8);
    ogre_ode_world -> setAutoSleep (true);
    ogre_ode_world -> setContactCorrectionVelocity (1.0);
	//	The following line causes an error in Ogre.
	//	ogre_ode_world -> setShowDebugGeometries (true);

	Engines :: Log :: log (me) << "ERP: " << ogre_ode_world -> getERP () << endl;
	Engines :: Log :: log (me) << "CFM: " << ogre_ode_world -> getCFM () << endl;

	ogre_ode_world -> getSceneManager () -> setSkyDome (true, "Examples/CloudySky", 5, 8, 100);
}

World ::
	~World ()
{
	Engines :: Log :: trace (me, World :: get_class_name (), "~");
	assert (World :: is_initialized ());

	prepare_for_destruction ();
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
	set_active_tile (Reference <Tile> tile)
{
	assert (is_initialized ());

	if ((! has_active_state ()) || (tile != get_active_state ()))
	{
		if (has_active_state () && Items :: Player_Character :: is_instantiated ())
		{
			if (! tile -> contains (Items :: Player_Character :: get () -> get_movable_model ()))
			{
				get_active_state () -> move
					(Items :: Player_Character :: get () -> get_movable_model (), tile);
			}
		}

		State_Machine <Tile> :: set_active_state (tile);
	}
}

void World ::
	update ()
{
	assert (is_initialized ());

	//	Here, 'last_turn_length' contains the lenth of the current turn.
	last_turn_length = float (turn_lenght_timer -> getMilliseconds ()) / 1000;

	//	Reset the timer, not the reference.
	turn_lenght_timer -> reset ();

	#ifdef RADAKAN_DEBUG
		Engines :: Log :: log (me) << "Turn lenght: " << last_turn_length << " seconds" << endl;

		turn ++;

		Engines :: Log :: log (me) << "Turn " << turn << " started" << endl;
	#endif

	Ogre :: Vector3 position = Items :: Player_Character :: get () -> get_movable_model () -> node -> getPosition ();

	int x = int (floor (position . x / Tile :: side_length));
	int z = int (floor (position . z / Tile :: side_length));
	assert (Tile :: side_length * min_x <= x);
	assert (x < Tile :: side_length * (max_x + 1));
	assert (Tile :: side_length * min_z <= z);
	assert (z < Tile :: side_length * (max_z + 1));
	Reference <Tile> new_active_tile = tiles [pair <int, int> (x, z)];
	assert (new_active_tile . points_to_object ());
	set_active_tile (new_active_tile);

	for (map <pair <int, int>, Reference <Tile> > :: iterator i = tiles . begin ();
		i != tiles . end (); i ++)
	{
		i -> second -> space -> collide ();
	}

	ogre_ode_world -> step (time_step);
	ogre_ode_world -> updateDrawState ();
	ogre_ode_world -> synchronise ();
	ogre_ode_world -> clearContacts ();

	//	run the AI for all nearby NPCs
	Items :: Player_Character :: get () -> call_observers
		(Messages :: Message <Items :: Character> :: update);
}
const float & World ::
	get_last_turn_length ()
	const
{
	assert (is_initialized ());

	return last_turn_length;
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

	contact -> setCoulombFriction (0.2);
	//contact -> setBouncyness (0);

//	Log :: log (me) << "collision: " << g1 -> getClass () << " " << g2 -> getClass () << endl;
	return true;
}

string World ::
	get_FPS () const
{
	assert (is_initialized ());

	return "FPS: " + to_string (1.0 / World :: get () -> get_last_turn_length ());
}
