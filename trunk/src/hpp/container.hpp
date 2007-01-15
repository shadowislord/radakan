#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "weapon.hpp"
#include "set.hpp"

using namespace std;

namespace tsl
{

	///	Container holds the data of one in-game container.

	class Container :
		public Entity,
		public Set <Entity>
	{
		public :
			Container
				(bool new_movable,
				bool new_solid,
				bool new_visible,
				float new_volume,
				float new_weight,
				btVector3 new_position,
				Ogre :: SceneNode & new_node);
			virtual ~Container ();
			virtual bool is_initialized () const;
			static string get_type_name ();
			virtual float get_total_weight () const;
			virtual bool add (Entity & entity);
			virtual bool move_to (Entity & entity, Set <Entity> & other_set);
		
		private :
			//	Copies are not allowed.
			Container (const Container & container);
	};
}

#endif
