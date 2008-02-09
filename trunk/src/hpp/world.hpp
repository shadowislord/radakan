#ifndef RADAKAN_WORLD_HPP
#define RADAKAN_WORLD_HPP

#include "singleton.hpp"
#include "state_machine.hpp"

#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
	#include <OgreOdeCollision.h>

	namespace OgreOde
	{
		class ForwardFixedInterpolatedStepHandler;
	}
#elif RADAKAN_PHYSICS_MODE == RADAKAN_BULLET_MODE
	class btBroadphaseInterface;
	class btCollisionConfiguration;
	class btConstraintSolver;
	class btDiscreteDynamicsWorld;
	class btDispatcher;
#else
#endif

namespace Radakan
{
	template <typename T, class U> class Map;
	class Model;
	class Tile;
	
	///	World contains all basic 'environment' data.
	class World :
		public Singleton <World>
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
		,
		public OgreOde :: CollisionListener
#else
#endif
	{
		public:
			///	These are the tile position limits, not the item position limits.
			static const int min_x;
			static const int max_x;
			static const int min_z;
			static const int max_z;
			
			World ();
			virtual ~World ();
			virtual bool is_initialized () const;
			
			static string get_class_name ();
			
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
			///	Called by OgreOde whenever a collision occurs,
			///	so that we can modify the contact parameters.
			virtual bool collision (OgreOde :: Contact * contact);
#else
#endif
			
			void update ();

			///	'get_colse_items (...)' returns all close models, but not the argument.
			Reference <Container <Model> > get_close_models (Reference <Model> by);
			
			boost :: scoped_ptr <Ogre :: SceneNode> root_node;
#if RADAKAN_PHYSICS_MODE == RADAKAN_OGREODE_MODE
			boost :: scoped_ptr <OgreOde :: World> ogre_ode_world;
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
			boost :: scoped_ptr <btDiscreteDynamicsWorld> bullet_world;

		private :
#endif
		
			Reference <Map <Mathematics :: Vector_3D, Tile> > tiles;
	};
}

#endif	// RADAKAN_WORLD_HPP
