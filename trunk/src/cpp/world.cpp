#include <OgreOdeBody.h>	//	inclusion before 'OgreOdeWorld.h' to evade errors
#include <OgreOdeGeometry.h>	//	inclusion before 'OgreOdeWorld.h' to evade errors
#include <OgreOdeWorld.h>	//	inclusion before 'OgreOdeStepper.h' to evade errors
#include <OgreOdeSpace.h>
#include <OgreOdeStepper.h>

#include "engines/log.hpp"
#include "engines/render_engine.hpp"
#include "items/character.hpp"
#include "map.hpp"
//	#include "messages/message.hpp"
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

World ::
	World () :
	Object ("world", true),	//	Here 'true' means 'prevent automatic destruction'.
	root_node
		(Engines :: Render_Engine :: get () -> get_scene_manager () -> getRootSceneNode ()),
	ogre_ode_world
		(new OgreOde :: World (Engines :: Render_Engine :: get () -> get_scene_manager () . get ())),
	tiles (new Map <pair <int, int>, Tile> ("tiles")),
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
			tiles -> add
			(
				Pair <pair <int, int>, Tile> :: create
				(
					coordinates,
					Reference <Tile>
					(
						new Tile
						(
							coordinates,
							boost :: shared_ptr <OgreOde :: SimpleSpace>
							(
								new OgreOde :: SimpleSpace
								(
									ogre_ode_world . get (),
									ogre_ode_world -> getDefaultSpace ()
								)
							)
						)
					)
				)
			);
		}
	}

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

void World ::
	update ()
{
	assert (is_initialized ());

	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			tiles -> look_up (pair <int, int> (x, z)) -> space -> collide ();
		}
	}

	ogre_ode_world -> step (time_step);
	ogre_ode_world -> updateDrawState ();
	ogre_ode_world -> synchronise ();
	ogre_ode_world -> clearContacts ();
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
