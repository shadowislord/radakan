#ifndef TSL_WORLD_HPP
#define TSL_WORLD_HPP

#include "singleton.hpp"
#include "state_machine.hpp"
#include "tile.hpp"
#include "movable_model.hpp"

#include <OgreSceneManager.h>

namespace TSL
{

	///	World contains all basic 'environment' data.
	class World :
		public Singleton <World>,
		//	public virtual Object,
		public OgreOde :: World,
		private OgreOde :: ForwardFixedInterpolatedStepHandler,	//	choice of step handler: Enables fps control
		public OgreOde :: CollisionListener,
		public State_Machine <Tile>
	{
		public:
			World (Ogre :: SceneManager & scene_manager);
			virtual ~World ();
			virtual bool is_initialized () const;

			static const string get_class_name ();

			void set_active_tile (Tile & tile);

			///	Called by OgreOde whenever a collision occurs,
			///	so that we can modify the contact parameters.
			virtual bool collision (OgreOde :: Contact * contact);

			void update ();

			const unsigned int & get_turn () const;

			///	in seconds
			const float & get_last_turn_lenght () const;

			string get_FPS () const;

			Ogre :: SceneNode & root_node;

		private :
			unsigned int turn;
			float last_turn_lenght;
			Ogre :: Timer turn_lenght_timer;

			//	The coordinates of a tile are expressed as (x, z).
			map <pair <int, int>, Tile *> tiles;

			//	These are the tile position limits, not the item position limits!
			static const int min_x;
			static const int max_x;
			static const int min_z;
			static const int max_z;
	};
}

#endif	// TSL_WORLD_HPP
