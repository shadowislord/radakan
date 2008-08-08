#ifndef RADAKAN_WORLD_HPP
#define RADAKAN_WORLD_HPP

#include "singleton.hpp"
#include "state_machine.hpp"

#include <OgreOdeCollision.h>

namespace OgreOde
{
	class ForwardFixedInterpolatedStepHandler;
}

namespace Radakan
{
	template <typename T, class U> class Map;
	class Model;
	class Tile;
	
	///	Ogre_Ode_World contains all basic 'environment' data.
	class Ogre_Ode_World :
		public Singleton <Ogre_Ode_World>,
		public World,
		public OgreOde :: CollisionListener
	{
		public:
			static string get_class_name ();
			
			static void create ();
			
			Ogre_Ode_World ();
			virtual ~Ogre_Ode_World ();
			virtual bool is_initialized () const;
			
			///	Called by OgreOde whenever a collision occurs,
			///	so that we can modify the contact parameters.
			virtual bool collision (OgreOde :: Contact * contact);
			
			virtual void update ();

			///	'get_colse_items (...)' returns all close models, but not the argument.
			Reference <Container <Model> > get_close_models (Reference <Model> by);
			
			boost :: scoped_ptr <Ogre :: SceneNode> root_node;
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
			boost :: scoped_ptr <OgreOde :: Ogre_Ode_World> ogre_ode_world;
#endif
			
		private :
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
			///	enables fps control
			boost :: scoped_ptr <OgreOde :: ForwardFixedInterpolatedStepHandler>
				step_handler;
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
			boost :: scoped_ptr <btCollisionConfiguration> collision_configuration;
			boost :: scoped_ptr <btDispatcher> dispatcher;
			boost :: scoped_ptr <btBroadphaseInterface> broadphase;
			boost :: scoped_ptr <btConstraintSolver> constraint_solver;

		public :
			boost :: scoped_ptr <btDiscreteDynamicsOgre_Ode_World> bullet_world;

		private :
#endif
		
			Reference <Map <Mathematics :: Vector_3D, Tile> > tiles;
	};
}

#endif	// RADAKAN_WORLD_HPP
