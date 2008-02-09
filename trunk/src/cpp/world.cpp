#include "engines/log.hpp"
#include "engines/render_engine.hpp"
#include "map.hpp"
//	#include "messages/message.hpp"
#include "model.hpp"
#include "tile.hpp"
#include "world.hpp"

#include <OgreSceneManager.h>

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include <OgreOdeBody.h>	//	inclusion before 'OgreOdeWorld.h' to evade errors
	#include <OgreOdeGeometry.h>	//	inclusion before 'OgreOdeWorld.h' to evade errors
	#include <OgreOdeWorld.h>	//	inclusion before 'OgreOdeStepper.h' to evade errors
	#include <OgreOdeSpace.h>
	#include <OgreOdeStepper.h>
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	#include <btBulletDynamicsCommon.h>
#else
	#include <pal.h>
#endif


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
	Object ("world", "singleton"),
	root_node (Engines :: Render_Engine :: get () -> get_scene_manager ()
		-> getRootSceneNode ()),
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	ogre_ode_world
		(new OgreOde :: World
			(Engines :: Render_Engine :: get () -> get_scene_manager () . get ())),
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
	),
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	collision_configuration (new btDefaultCollisionConfiguration),
	dispatcher (new btCollisionDispatcher (collision_configuration . get ())),
	broadphase (new btSimpleBroadphase),
	constraint_solver (new btSequentialImpulseConstraintSolver),
	bullet_world (new btDiscreteDynamicsWorld
	(
		dispatcher . get (),
		broadphase . get (),
		constraint_solver . get (),
		collision_configuration . get ()
	)),
#else
#endif
	tiles (new Map <Mathematics :: Vector_3D, Tile> ("tiles"))
{
	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			Mathematics :: Vector_3D coordinates (x, 0, z);

			Reference <Tile> tile
			(
				new Tile
				(
					coordinates
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
					,
					boost :: shared_ptr <OgreOde :: SimpleSpace>
					(
						new OgreOde :: SimpleSpace
						(
							ogre_ode_world . get (),
							ogre_ode_world -> getDefaultSpace ()
						)
					)
#endif
				)
			);

			Engines :: Log :: log (me) << "coordinates: " << coordinates << endl;

			tiles -> add
			(
				Pair <Mathematics :: Vector_3D, Tile> :: create (coordinates, tile)
			);
		}
	}

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
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
#else
	bullet_world -> setGravity (btVector3 (0, - 9.81, 0));
#endif
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

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			tiles -> look_up (Mathematics :: Vector_3D (x, 0, z)) -> space -> collide ();
		}
	}

	ogre_ode_world -> step (time_step);
	ogre_ode_world -> updateDrawState ();
	ogre_ode_world -> synchronise ();
	ogre_ode_world -> clearContacts ();
#else
	bullet_world -> stepSimulation (time_step);

	Pointer <Tile> tile;
	for (int x = min_x; x <= max_x; x ++)
	{
		for (int z = min_z; z <= max_z; z ++)
		{
			tile = tiles -> look_up (Mathematics :: Vector_3D (x, 0, z));
			for (Pointer <Model> model = tile -> get_child (); model . points_to_object ();
				model = tile -> get_another_child ())
			{
				model -> sync ();
			}
		}
	}
#endif
}

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
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
#endif
