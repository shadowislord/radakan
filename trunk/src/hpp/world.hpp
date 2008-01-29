#ifndef RADAKAN_WORLD_HPP
#define RADAKAN_WORLD_HPP

#include <OgreOdeCollision.h>

#include "singleton.hpp"
#include "state_machine.hpp"

namespace OgreOde
{
	class ForwardFixedInterpolatedStepHandler;
}

namespace Radakan
{
	template <typename T, class U> class Map;
	class Model;
	class Tile;
	
	///	World contains all basic 'environment' data.
	class World :
		public Singleton <World>,
		public OgreOde :: CollisionListener
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
			
			///	Called by OgreOde whenever a collision occurs,
			///	so that we can modify the contact parameters.
			virtual bool collision (OgreOde :: Contact * contact);
			
			void update ();

			///	'get_colse_items (...)' returns all close models, but not the argument.
			Reference <Container <Model> > get_close_models (Reference <Model> by);
			
			boost :: scoped_ptr <Ogre :: SceneNode> root_node;
			boost :: scoped_ptr <OgreOde :: World> ogre_ode_world;
			
		private :
			//	The coordinates of a tile are expressed as (x, z).
			Reference <Map <pair <int, int>, Tile> > tiles;
			
			///	enables fps control
			boost :: scoped_ptr <OgreOde :: ForwardFixedInterpolatedStepHandler>
				step_handler;
	};
}

#endif	// RADAKAN_WORLD_HPP
