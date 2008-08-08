#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include "engines/log.hpp"
	#include "engines/render_engine.hpp"
	#include "items/item.hpp"
	#include "map.hpp"
	//	#include "messages/message.hpp"
	#include "tile.hpp"
	#include "Ogre_Ode_World.hpp"

	#include <OgreSceneManager.h>

	#include <OgreOdeBody.h>	//	inclusion before 'OgreOdeOgre_Ode_World.h' to evade errors
	#include <OgreOdeGeometry.h>	//	inclusion before 'OgreOdeOgre_Ode_World.h' to evade errors
	#include <OgreOdeOgre_Ode_World.h>	//	inclusion before 'OgreOdeStepper.h' to evade errors
	#include <OgreOdeSpace.h>
	#include <OgreOdeStepper.h>

	using namespace std;
	using namespace Radakan;

	//	static
	string Ogre_Ode_World ::
		get_class_name ()
	{
		return "Ogre_Ode_World";
	}

	Ogre_Ode_World ::
		Ogre_Ode_World () :
		Object ("Ogre_Ode_World", "singleton"),
		ogre_ode_world
			(new OgreOde :: Ogre_Ode_World
				(Engines :: Render_Engine :: get () -> get_scene_manager () . get ())),
		step_handler
		(
			new OgreOde :: ForwardFixedInterpolatedStepHandler
			(
				ogre_ode_Ogre_Ode_World . get (),
				OgreOde :: StepHandler :: QuickStep,
				time_step,
				frame_rate,
				max_frame_time,
				time_scale
			)
		)
	{
		ogre_ode_Ogre_Ode_World -> setCollisionListener (this);

		ogre_ode_Ogre_Ode_World -> setGravity (Ogre :: Vector3 (0, - 9.81, 0));
		//	ogre_ode_Ogre_Ode_World -> setCFM (10e-5);	//	fine tune to make a realistic simulation
		//	ogre_ode_Ogre_Ode_World -> setERP (0.8);
		ogre_ode_Ogre_Ode_World -> setAutoSleep (true);
		ogre_ode_Ogre_Ode_World -> setContactCorrectionVelocity (1.0);
		//	The following line causes an error in Ogre.
		//	ogre_ode_Ogre_Ode_World -> setShowDebugGeometries (true);

		Engines :: Log :: log (me) << "ERP: " << ogre_ode_Ogre_Ode_World -> getERP () << endl;
		Engines :: Log :: log (me) << "CFM: " << ogre_ode_Ogre_Ode_World -> getCFM () << endl;
		
		for (int x = min_x; x <= max_x; x ++)
		{
			for (int z = min_z; z <= max_z; z ++)
			{
				Mathematics :: Vector_3D coordinates (x, 0, z);

				Reference <Tile> tile
				(
					new Tile
					(
						coordinates,
						boost :: shared_ptr <OgreOde :: SimpleSpace>
						(
							new OgreOde :: SimpleSpace
							(
								ogre_ode_Ogre_Ode_World . get (),
								ogre_ode_Ogre_Ode_World -> getDefaultSpace ()
							)
						)
					)
				);

				Engines :: Log :: log (me) << "coordinates: " << coordinates << endl;

				tiles -> add
				(
					Pair <Mathematics :: Vector_3D, Tile> :: create (coordinates, tile)
				);
			}
		}
	}

	Ogre_Ode_World ::
		~Ogre_Ode_World ()
	{
		Engines :: Log :: trace (me, Ogre_Ode_World :: get_class_name (), "~");
		assert (Ogre_Ode_World :: is_initialized ());

		prepare_for_destruction ();
	}

	//	virtual
	bool Ogre_Ode_World ::
		is_initialized ()
		const
	{
		return Object :: is_initialized ();
	}

	void Ogre_Ode_World ::
		update ()
	{
		assert (is_initialized ());

		for (int x = min_x; x <= max_x; x ++)
		{
			for (int z = min_z; z <= max_z; z ++)
			{
				tiles -> look_up (Mathematics :: Vector_3D (x, 0, z)) -> space -> collide ();
			}
		}

		ogre_ode_Ogre_Ode_World -> step (time_step);
		ogre_ode_Ogre_Ode_World -> updateDrawState ();
		ogre_ode_Ogre_Ode_World -> synchronise ();
		ogre_ode_Ogre_Ode_World -> clearContacts ();
	}

	bool Ogre_Ode_World ::
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
